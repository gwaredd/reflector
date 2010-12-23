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

            my $name = $bm->attribute('name');
            my $type = $bm->attribute('type');
            my $file = $bm->attribute('file');
            my $line = $bm->attribute('line');

            $name = "" unless $name;
            $type = "" unless $type;
            $file = "unknown" unless $file;
            $line = -1 unless $file;

            $g_bookmarks{$file} = [] unless $g_bookmarks{$file};

            push @{$g_bookmarks{$file}}, { line => $line, name => $name, type => $type, node => $node };
        }
    }

#    # sort by line number
#    # (probably unecessary as they will exist in order in the xml)
#
#    foreach my $k (keys %g_bookmarks)
#    {
#        my @s = sort { $a->{'line'} <=> $b->{'line'} } @{$g_bookmarks{$k}};;
#    }
}


################################################################################

sub ExpandNode
{
    my ( $fh, $indent, $node, $close ) = @_;

    print $fh $indent."<%auto\n";

    print $fh $indent."DECLARE_RTTI\n\n";

    foreach my $var ($node->children())
    {
        next unless $var->name eq "var";

        my $name = $var->attribute('name');
        my $type = $var->attribute('type');

        printf( $fh "%s%-10s Get%s();\n", $indent, "$type&", ucfirst($name) );
        printf( $fh "%s%-10s Set%s( $type& );\n", $indent, "void", ucfirst($name) );
    }

    if( $close )
    {
        print $fh $indent."%>\n";
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
        # if the current line is bookmarked ...

        if( defined($bm) and $bm->{'line'} == $. )
        {
            # what bookmark type is it?

            if( $bm->{'type'} eq "region_end" )
            {
                print FOUT $_;
                $skip = 0;
            }
            elsif( $bm->{'name'} eq "auto" )
            {
                my ($indent) = ($_ =~ m/^(\s+)/);

                ExpandNode( *FOUT, $indent, $bm->{'node'}, $bm->{'type'} eq "region_start" ? 0 : 1 );

                if( $bm->{'type'} eq "region_start" )
                {
                    $skip = 1;
                }
            }
            else
            {
                warn "Unknown bookmark type '$bm->{name}'\n";
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

