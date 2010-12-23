#!perl -w
################################################################################

use strict;

use Template;
use File::Copy;
use XML::Parser;
use XML::SimpleObject;
use Getopt::Long;
use Data::Dumper;

our %g_bookmarks = ();

our $g_tt = new Template({
                            INTERPOLATE     => 1,
                            PRE_CHOMP       => 1,
                            POST_CHOMP      => 1,
                            TRIM            => 0,
                        }) or die $Template::ERROR, "\n";

our $g_tmpl     = "templates/Accessors.tt";
our $g_replace  = 0;

################################################################################
# find a class or struct that has an "accessors" bookmark

sub ExtractBookmarks
{
    my $root = shift;

    foreach my $node ($root->children())
    {
        next unless $node->name eq "struct" or $node->name eq "class";

        foreach my $c ( $node->children() )
        {
            if( $c->name eq "bookmark" )
            {
                my $name = $c->attribute('name');
                my $type = $c->attribute('type');
                my $file = $c->attribute('file');
                my $line = $c->attribute('line');

                $name = "" unless $name;
                $type = "" unless $type;
                $file = "unknown" unless $file;
                $line = -1 unless $file;

                # only process <accessors> bookmarks (or end regions as they aren't named)

                next unless $name eq "accessors" or $type eq "region_end";

                # add accessors bookmark to list

                $g_bookmarks{$file} = [] unless $g_bookmarks{$file};
                push @{$g_bookmarks{$file}}, { line => $line, name => $name, type => $type, node => $node };
            }
        }


        # TODO: remove accessors entries from XML within region
        # TODO: remove vars that already have custom accessors
        # ^--- this will require an XML parser that returns nodes in order they appear in the file, XML::SimpleObject doesn't :/
    }
}


################################################################################
# open file and process bookmarked lines

sub ProcessFile
{
    my ( $file, $bookmarks ) = @_;

    # get first bookmark in list

    my $bm = shift @{$bookmarks};


    # foreach line of the file

    my $skip = 0;

    my $fileout = $file.".tmp";

    open( FIN,  "<$file" ) or die "Failed to open '$file'\n";
    open( FOUT, ">$fileout" ) or die "Failed to open '$fileout'\n";

    while( <FIN> )
    {
        # if the current line is bookmarked ...

        if( defined($bm) and $bm->{'line'} == $. )
        {
            if( $bm->{'type'} eq "region_end" )
            {
                if( $skip )
                {
                    $skip = 0;
                }
                else
                {
                    print FOUT $_;
                }
            }
            else
            {
                if( $bm->{'type'} eq "region_start" )
                {
                    $skip = 1;
                }
                else
                {
                    $skip = 0;
                }


                # expand accessors template

                my ($indent) = ($_ =~ m/^(\s+)/);

                my %vars = (
                        node    => $bm->{'node'},
                        indent  => $indent,
                    );

                $g_tt->process( $g_tmpl, \%vars, \*FOUT, { binmode => 1 } ) or die $g_tt->error();
            }

            # next bookmark

            $bm = shift @{$bookmarks};
        }
        elsif( !$skip )
        {
            print FOUT $_;
        }
    }

    close(FOUT);
    close(FIN);

    # replace current file with output file

    if( $g_replace )
    {
        my $filebak = "$file.bak";

        unlink $filebak if -f $filebak;
        move( $file, $filebak );
        move( $fileout, $file );
    }
}


################################################################################
# main

sub Main
{
    # read command line

    my $xml = "";

    GetOptions( 
        "xml=s"     => \$xml,
        "replace"   => \$g_replace,
    );

    die "Usage: perl $0 --xml=<xml>\n" unless $xml;

    # load xml

    my $parser  = new XML::Parser( ErrorContext => 2, Style => "Tree" );
    my $xso     = new XML::SimpleObject( $parser->parsefile( $xml ) );
    my $root    = $xso->child( 'reflector' ) or die "Not a reflector file\n";


    ExtractBookmarks( $root );


    # for each file

    while( my( $file, $bookmarks ) = each %g_bookmarks )
    {
        print "Processing '$file'\n";
        ProcessFile( $file, $bookmarks );
    }
}

Main();

