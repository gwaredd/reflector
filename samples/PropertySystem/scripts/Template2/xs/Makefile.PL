# $Id: Makefile.PL 351 2002-04-15 13:38:47Z abw $

use lib qw( ../lib ./lib );
use ExtUtils::MakeMaker;
use Template;

WriteMakefile(
    'NAME'     => 'Template::Stash::XS',
    'C'        => [ qw( Stash.c ) ],
    'XS'       => { 'Stash.xs' => 'Stash.c' },
    'OBJECT'   => 'Stash.o',
    'VERSION'  => $Template::VERSION,
    'NORECURS' => 1,
);

