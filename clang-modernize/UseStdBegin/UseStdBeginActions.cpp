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

void MemberBeginReplacer::run(const MatchFinder::MatchResult &result) {
  const auto *memberCall =
      result.Nodes.getNodeAs<CXXMemberCallExpr>(CallExprID);

  const auto &sourceMgr = *result.SourceManager;
  std::string refName;
  if (const auto *memberExpr = dyn_cast<MemberExpr>(memberCall->getCallee())) {
    refName = getStringInRange(sourceMgr, *memberExpr->getBase());
  }

  const auto &methodName = memberCall->getMethodDecl()->getNameAsString();
  const auto startLoc = sourceMgr.getSpellingLoc(memberCall->getLocStart());
  const auto endLoc = sourceMgr.getSpellingLoc(memberCall->getLocEnd());

  // if (printLocations) {
  //   outs() << sourceMgr.getFilename(startLoc) << ":"
  //          << sourceMgr.getSpellingLineNumber(startLoc) << ":"
  //          << sourceMgr.getSpellingColumnNumber(startLoc) << "\n";
  // }

  // FIXME: add #include <iterator>
  std::string replacememnt("std::" + methodName + "(" + refName);
  auto length =
      sourceMgr.getFileOffset(endLoc) - sourceMgr.getFileOffset(startLoc);
  Owner.addReplacementForCurrentTU(
      tooling::Replacement(sourceMgr, startLoc, length, replacememnt));
  ++AcceptedChanges;
}
