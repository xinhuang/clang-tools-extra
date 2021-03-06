//===--- ReplaceAutoPtrCheck.h - clang-tidy----------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MODERNIZE_REPLACE_AUTO_PTR_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MODERNIZE_REPLACE_AUTO_PTR_H

#include "../ClangTidy.h"
#include "../utils/IncludeInserter.h"

namespace clang {
namespace tidy {
namespace modernize {

/// \brief Transforms the deprecated \c std::auto_ptr into the C++11 \c
/// std::unique_ptr.
///
/// Note that both the \c std::auto_ptr type and the transfer of ownership are
/// transformed. \c std::auto_ptr provides two ways to transfer the ownership,
/// the copy-constructor and the assignment operator. Unlike most classes these
/// operations do not 'copy' the resource but they 'steal' it.
/// \c std::unique_ptr uses move semantics instead, which makes the intent of
/// transferring the resource explicit. This difference between the two smart
/// pointers requeres to wrap the copy-ctor and assign-operator with
/// \c std::move().
///
/// For example, given:
/// \code
///   std::auto_ptr<int> i, j;
///   i = j;
/// \endcode
/// This code is transformed to:
/// \code
///   std::unique_ptr<in> i, j;
///   i = std::move(j);
/// \endcode
class ReplaceAutoPtrCheck : public ClangTidyCheck {
public:
  ReplaceAutoPtrCheck(StringRef Name, ClangTidyContext *Context);
  void storeOptions(ClangTidyOptions::OptionMap &Opts) override;
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void registerPPCallbacks(CompilerInstance &Compiler) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::unique_ptr<IncludeInserter> Inserter;
  const IncludeSorter::IncludeStyle IncludeStyle;
};

} // namespace modernize
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_MODERNIZE_REPLACE_AUTO_PTR_H
