set(LLVM_LINK_COMPONENTS mc)

add_clang_library(clangBasic
  Builtins.cpp
  ConvertUTF.c
  Diagnostic.cpp
  DiagnosticIDs.cpp
  FileManager.cpp
  FileSystemStatCache.cpp
  IdentifierTable.cpp
  LangOptions.cpp
  Module.cpp
  SourceLocation.cpp
  SourceManager.cpp
  TargetInfo.cpp
  Targets.cpp
  TokenKinds.cpp
  Version.cpp
  VersionTuple.cpp
  )

# Determine Subversion revision.
# FIXME: This only gets updated when CMake is run, so this revision number
# may be out-of-date!
set_source_files_properties(Version.cpp
  PROPERTIES COMPILE_DEFINITIONS "SVN_REVISION=\"${SVN_REVISION}\"")

add_dependencies(clangBasic 
                 ClangARMNeon
                 ClangAttrList
                 ClangDiagnosticAnalysis
                 ClangDiagnosticAST
                 ClangDiagnosticCommon
                 ClangDiagnosticDriver
                 ClangDiagnosticFrontend
                 ClangDiagnosticGroups
                 ClangDiagnosticLex
                 ClangDiagnosticParse
                 ClangDiagnosticSema
                 ClangDiagnosticSerialization
                 ClangDiagnosticIndexName)
                 
