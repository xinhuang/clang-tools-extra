set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyModernizeModule
  LoopConvertCheck.cpp
  LoopConvertUtils.cpp
  ModernizeTidyModule.cpp
  PassByValueCheck.cpp
  ReplaceAutoPtrCheck.cpp
  UseAutoCheck.cpp
  UseNullptrCheck.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  clangTidy
  clangTidyReadabilityModule
  clangTidyUtils
  )
