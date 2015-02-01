#ifndef CLANG_MODERNIZE_USE_STD_BEGIN_ACTIONS_H
#define CLANG_MODERNIZE_USE_STD_BEGIN_ACTIONS_H

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Refactoring.h"

class Transform;

class MemberBeginReplacer
    : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
  MemberBeginReplacer(unsigned &AcceptedChanges, Transform &Owner)
      : AcceptedChanges(AcceptedChanges), Owner(Owner) {}

  virtual void
  run(const clang::ast_matchers::MatchFinder::MatchResult &result) final;

private:
  unsigned &AcceptedChanges;
  Transform &Owner;
};

#endif // CLANG_MODERNIZE_USE_STD_BEGIN_ACTIONS_H
