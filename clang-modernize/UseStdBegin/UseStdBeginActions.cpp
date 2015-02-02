#include "UseStdBeginActions.h"
#include "UseStdBeginMatchers.h"
#include "Core/Transform.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/Refactoring.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

#include <string>

std::string getStringInRange(const SourceManager &SM, const Stmt &ST) {
  const auto &LocStart = ST.getLocStart();
  const auto &LocEnd = ST.getLocEnd();
  const char *CharStart = SM.getCharacterData(LocStart);
  const char *CharEnd = SM.getCharacterData(LocEnd);
  return std::string(CharStart, CharEnd + 1);
}

void MemberBeginReplacer::run(const MatchFinder::MatchResult &Result) {
  const auto *MemberCall =
      Result.Nodes.getNodeAs<CXXMemberCallExpr>(CallExprID);

  const auto &SM = *Result.SourceManager;
  const auto *Member = dyn_cast<MemberExpr>(MemberCall->getCallee());
  if (Member == nullptr) {
    return;
  }
  const auto &Instance = *Member->getBase();
  const auto &RefName = getStringInRange(SM, Instance);
  bool IsConst = Instance.getType().isConstQualified();

  auto MethodName = MemberCall->getMethodDecl()->getNameAsString();
  MethodName = IsConst ? "c" + MethodName : MethodName;
  const auto StartLoc = SM.getSpellingLoc(MemberCall->getLocStart());
  const auto EndLoc = SM.getSpellingLoc(MemberCall->getLocEnd());

  // FIXME: add #include <iterator>
  std::string Replacement("std::" + MethodName + "(" + RefName);
  auto Length = SM.getFileOffset(EndLoc) - SM.getFileOffset(StartLoc);
  Owner.addReplacementForCurrentTU(
      tooling::Replacement(SM, StartLoc, Length, Replacement));
  ++AcceptedChanges;
}
