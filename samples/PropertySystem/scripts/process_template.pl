#!perl -w
################################################################################

use strict;

use Template;
use XML::Parser;
use XML::SimpleObject;
use Getopt::Long;


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

    return sprintf( "0x%X", $hash );
}

################################################################################
# main

sub Main
{
    # read command line

    my $xml = "";
    my $in  = "";
    my $out = "";

    GetOptions( 
        "xml=s" => \$xml,
        "in=s"  => \$in,
        "out=s" => \$out,
    );

    die "Usage: perl $0 --xml=<xml> --in=<in> --out=<out>\n" unless $xml and $in and $out;

    # load xml

    print "Loading '$xml'\n";

    my $parser  = new XML::Parser( ErrorContext => 2, Style => "Tree" );
    my $xso     = new XML::SimpleObject( $parser->parsefile( $xml ) );
    my $root    = $xso->child( 'reflector' ) or die "Not a reflector file\n";


    # process template

    my $tt = new Template({
                            INTERPOLATE  => 1,
                            POST_CHOMP   => 1,
                            EVAL_PERL    => 1,
                        }) or die $Template::ERROR, "\n";

    print "Creating '$out'\n";
    $tt->process( $in, { root => $root, hash => \&DJB2 }, $out, { binmode => 1 } ) or die $tt->error();
}

Main();

