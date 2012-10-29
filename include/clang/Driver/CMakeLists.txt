llvm_tablegen(Options.inc -gen-opt-parser-defs
  SOURCE Options.td
  TARGET ClangDriverOptions)

llvm_tablegen(CC1AsOptions.inc -gen-opt-parser-defs
  SOURCE CC1AsOptions.td
  TARGET ClangCC1AsOptions)
