
About
--------------------------------------------------------------------------------

    reflector parses C++ code and exports (reflects) declaration details to an
    XML file.
    
    special "commands" can be inserted into the source code to control what is
    exported and to tag declarations with additional metadata.



Usage
--------------------------------------------------------------------------------

    reflector.exe [options] [input files]
    
    reflector.exe --help
    reflector.exe -std=c++98 -DMYDEFINE -o output.xml myfile.cpp
    

Reflector Commands
--------------------------------------------------------------------------------
    see test/test_02_reflect.cpp for examples

    reflector processing instructions come in two flavours - comments or using
    the gnu "annotate" attribute (both are identical functionally)
    
    For example
    
        // %reflect
        
    or     
    
        __attribute__((annotate("%reflect")));
        

    commands are applied to a declaration on the same line or next one in the file
    
    these are identical
    
        int this_is_exported; // %reflect
        int this_is_not_exported;
    
    
        // %reflect
        int this_is_exported;
        int this_is_not_exported;
    
    
    commands must start on a newline,
    
        // -- %reflect  <-- this will be ignored as the line does not start with "%reflect" 


    commands can be applied to the current lexical scope with %%reflect
    
        // %%reflect
        int this_is_exported;
        int so_is_this;


    commands are inherited by default
    
        // %reflect
        class Base {};
        
        class Child : public Base {};   // child will also be exported as it inherits "Base"
    

    multiple commands can be separated with a ',', e.g.
    
        // %reflect write namespace,explicit


    commands can be negated by putting "no" infront, e.g.
    
        // %reflect no inherit
    

    commands are 
    
        write FQN           write "fully qualified name" with declaration
        write namespace     write namespace with declaration (enabled by default)
        write access        write access specifier (public/protected/private) with declaration
        write location      write source file and line number with declaration

        inherit             child classes will inherit parent reflection properties (enabled by default)
        explicit            declarations must be explicitly marked up
        no                  invert option (or don't reflect if on own)
        ignore              ignore this declaration
        operators           export operators (e.g. +,-,[],etc, false by default)
        
        access [public|private|protected] set access level to export (public by default)
        
        attribute [k=v[;k=v]]   add metadata to declaration, e.g. attribute min=3;max=10

        bm [name]           export source code bookmark
        
        debug               turn on debug


    

Compiling
--------------------------------------------------------------------------------

    Source code does not include external libraries. These can be got from ...

    http://sourceforge.net/projects/tinyxml/
    http://clang.llvm.org/get_started.html      (change list 121482)



