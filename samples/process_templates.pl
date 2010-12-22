#!perl -w
################################################################################

use strict;
use lib 'Template2/lib'; # include Template::Toolkit

use Template;
use XML::Parser;
use XML::SimpleObject;
use Data::Dumper;

our $g_dir  = ".";
our $g_xml  = "shapes.xml";


################################################################################

sub DJB2
{
    use integer qw(32bit unsigned);

    my $hash = 5381;

    foreach( split //, shift )
    {
        $hash = 31 * $hash + ord($_);
        $hash = $hash & 0xFFFFFFFF;
    }

    return $hash;
}

################################################################################
# main

sub Main
{
    # load xml

    my $parser  = new XML::Parser( ErrorContext => 2, Style => "Tree" );
    my $xso     = new XML::SimpleObject( $parser->parsefile( $g_xml ) );
    my $root    = $xso->child( 'reflector' ) or die "Not a reflector file\n";


    # process all .tt files in the current directory

    my $tt = new Template({
                            INTERPOLATE  => 1,
                            POST_CHOMP   => 1,
                            EVAL_PERL    => 1,
                        }) or die $Template::ERROR, "\n";

    opendir( D, $g_dir ) or die "Failed to open directory\n";

    foreach( readdir(D) )
    {
        if( m/(.*)\.tt$/i )
        {
            print "Creating '$1'\n";
            $tt->process( $_, { root => $root, hash => \&DJB2 }, $1 ) or die $tt->error();
        }
    }

    closedir(D);
}

Main();

