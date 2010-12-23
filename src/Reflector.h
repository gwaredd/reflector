#ifndef Reflector_h
#define Reflector_h
//////////////////////////////////////////////////////////////////////////
// include various global headers, etc

#include <string>
#include <stdio.h>

// clang barfs warnings on even a moderate setting :/
// best just to close your eyes and pretend it doesn't happen

#pragma warning( disable: 4800 ) // forcing value to bool 'true' or 'false' (performance warning)
#pragma warning( disable: 4146 ) // unary minus operator applied to unsigned type, result still unsigned
#pragma warning( disable: 4355 ) // 'this' : used in base member initializer list
#pragma warning( disable: 4291 ) // 'void *operator new(size_t,void *) throw()' : no matching operator delete found
#pragma warning( disable: 4624 ) // destructor could not be generated because a base class destructor is inaccessible
#pragma warning( disable: 4244 ) // 'return' : conversion from 'uint64_t' to 'unsigned int', possible loss of data
#pragma warning( disable: 4351 ) // new behavior: elements of array 'clang::Diagnostic::DiagMappings::Values' will be default initialized

#include "llvm/Config/config.h"
#include "llvm/Support/FormattedStream.h"

#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/FileSystemOptions.h"
#include "clang/Basic/IdentifierTable.h"

#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/Preprocessor.h"

#include "clang/Frontend/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/PreprocessorOptions.h"
#include "clang/Frontend/HeaderSearchOptions.h"
#include "clang/Frontend/FrontendOptions.h"
#include "clang/Frontend/Utils.h"
#include "clang/Frontend/DocumentXML.h"

#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/DeclGroup.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/Type.h"

#include "clang/Sema/AttributeList.h"

using namespace clang;
using namespace llvm;

#define rfNAME      "reflector"
#define rfVERSION   "1.0"

#ifdef _DEBUG
    #include <assert.h>
    #define ASSERT assert
#else
    #define ASSERT(x)
#endif


//////////////////////////////////////////////////////////////////////////
#endif // Reflector_h


