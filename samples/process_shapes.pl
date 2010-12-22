#!perl -w
################################################################################

use strict;

use XML::Parser;
use XML::SimpleObject;
use Data::Dumper;

our $g_dir  = ".";
our $g_xml  = "shapes.xml";

our %g_bookmarks = ();


################################################################################

sub ExtractBookmarks
{
    my $root = shift;

    # walk tree and extract bookmarks

    foreach my $node ($root->children())
    {
        next unless $node->name eq "struct" or $node->name eq "class";

        foreach my $bm ( $node->children() )
        {
            next unless $bm->name eq "bookmark";

            my $file = $bm->attribute('file');
            my $name = $bm->attribute('name');
            my $line = $bm->attribute('line');

            $file = "unknown" unless $file;
            $name = "" unless $name;
            $line = -1 unless $file;

            $g_bookmarks{$file} = [] unless $g_bookmarks{$file};

            push @{$g_bookmarks{$file}}, [ $line, $name, $node ];
        }
    }

    # sort by line number
    # (probably unecessary as they will exist in order in the xml)

    foreach my $k (keys %g_bookmarks)
    {
        sort { $a->[0] <=> $b->[0] } @{$g_bookmarks{$k}};
    }
}


################################################################################

sub ExpandNode
{
    my ( $fh, $indent, $node) = @_;

    print $fh $indent."DECLARE_RTTI\n\n";


    foreach my $var ($node->children())
    {
        next unless $var->name eq "var";

        my $name = $var->attribute('name');
        my $type = $var->attribute('type');

        printf( $fh "%s%-10s Get%s();\n", $indent, "$type&", ucfirst($name) );
        printf( $fh "%s%-10s Set%s( $type& );\n", $indent, "void", ucfirst($name) );
    }
}


################################################################################
#

sub ProcessBookmarks
{
    my ($file, $bookmarks) = @_;

    # get first bookmark in list

    my $bm = shift @{$bookmarks};


    # foreach line of the file

    my $skip = 0;

    open( FIN,  "<$file" ) or die "Failed to open '$file'\n";
    open( FOUT, ">$file.out" ) or die "Failed to open '$file.out'\n";

    while( <FIN> )
    {
        if( !$skip )
        {
            print FOUT $_;
        }

        # if the current line is bookmarked ...

        if( defined($bm) and $bm->[0] == $. )
        {
            # what bookmark type is it?

            if( $bm->[1] eq "end" )
            {
                print FOUT $_;
                $skip = 0;
            }
            elsif( $bm->[1] eq "auto" )
            {
                my ($indent) = ($_ =~ m/^(\s+)/);

                ExpandNode( *FOUT, $indent, $bm->[2] );
                $skip = 1;
            }
            else
            {
                warn "Unknown bookmark type '$bm->[1]'\n";
            }


            # next bookmark

            $bm = shift @{$bookmarks};
        }
    }

    close(FOUT);
    close(FIN);
}


################################################################################
# main

sub Main
{
    # load xml

    print "Loading '$g_xml'\n";

    my $parser  = new XML::Parser( ErrorContext => 2, Style => "Tree" );
    my $xso     = new XML::SimpleObject( $parser->parsefile( $g_xml ) );
    my $root    = $xso->child( 'reflector' ) or die "Not a reflector file\n";


    ExtractBookmarks( $root );


    # for each file

    while( my( $file, $bookmarks ) = each %g_bookmarks )
    {
        ProcessBookmarks( $file, $bookmarks );
    }
}

Main();

