//===--- MoveConstructorInitCheck.cpp - clang-tidy-------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MoveConstructorInitCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {

void MoveConstructorInitCheck::registerMatchers(MatchFinder *Finder) {
  // Only register the matchers for C++11; the functionality currently does not
  // provide any benefit to other languages, despite being benign.
  if (getLangOpts().CPlusPlus11) {
    Finder->addMatcher(
      constructorDecl(unless(isImplicit()), allOf(
        isMoveConstructor(),
        hasAnyConstructorInitializer(
          ctorInitializer(withInitializer(constructExpr(hasDeclaration(
            constructorDecl(isCopyConstructor()).bind("ctor")
            )))).bind("init")
          )
        )), this);
  }
}

void MoveConstructorInitCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *CopyCtor = Result.Nodes.getNodeAs<CXXConstructorDecl>("ctor");
  const auto *Initializer = Result.Nodes.getNodeAs<CXXCtorInitializer>("init");

  // Do not diagnose if the expression used to perform the initialization is a
  // trivially-copyable type.
  QualType QT = Initializer->getInit()->getType();
  if (QT.isTriviallyCopyableType(*Result.Context))
    return;
  
  const auto *RD = QT->getAsCXXRecordDecl();
  if (RD && RD->isTriviallyCopyable())
    return;

  // Diagnose when the class type has a move constructor available, but the
  // ctor-initializer uses the copy constructor instead.
  const CXXConstructorDecl *Candidate = nullptr;
  for (const auto *Ctor : CopyCtor->getParent()->ctors()) {
    if (Ctor->isMoveConstructor() && Ctor->getAccess() <= AS_protected &&
        !Ctor->isDeleted()) {
      // The type has a move constructor that is at least accessible to the
      // initializer.
      //
      // FIXME: Determine whether the move constructor is a viable candidate
      // for the ctor-initializer, perhaps provide a fixit that suggests
      // using std::move().
      Candidate = Ctor;
      break;
    }
  }

  if (Candidate) {
    // There's a move constructor candidate that the caller probably intended
    // to call instead.
    diag(Initializer->getSourceLocation(),
         "move constructor initializes %0 by calling a copy constructor")
        << (Initializer->isBaseInitializer() ? "base class" : "class member");
    diag(CopyCtor->getLocation(), "copy constructor being called",
         DiagnosticIDs::Note);
    diag(Candidate->getLocation(), "candidate move constructor here",
         DiagnosticIDs::Note);
  }
}

} // namespace tidy
} // namespace clang

