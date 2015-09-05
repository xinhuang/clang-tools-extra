#include "UseStdBeginActions.h"
#include "UseStdBeginMatchers.h"
#include "Core/Transform.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/Refactoring.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

#include <string>

namespace {
#if 0
std::string getExprText(const SourceManager &SM, const Expr &E) {
  const auto &LocStart = E.getLocStart();
  const auto &LocEnd = E.getLocEnd();
  const char *CharStart = SM.getCharacterData(LocStart);
  const char *CharEnd = SM.getCharacterData(LocEnd);
  if (CharStart == CharEnd)
    return {};
  return std::string(CharStart, CharEnd + 1);
}

const Expr *ignoreConstCasts(const Expr *ME) {
  const Expr *E = ME;
  while (true) {
    if (const auto *P = dyn_cast<const CXXConstCastExpr>(E)) {
      E = P->getSubExpr();
      continue;
    }
    if (const auto *P = dyn_cast<const ParenExpr>(E)) {
      if (P == ignoreConstCasts(P->getSubExpr())) {
        return E;
      }
      E = P;
    }
    return E;
  }
}
#endif // 0

std::string getExprText(const SourceManager &SM, const MemberExpr &E) {
  const auto &LocStart = E.getLocStart();
  const auto &LocEnd = E.getLocEnd();
  const char *CharStart = SM.getCharacterData(LocStart);
  const char *CharEnd = SM.getCharacterData(LocEnd);
  errs() << "\n-------->" << std::string(CharStart, 10) << "<--------\n";
  errs() << "\n-------->" << std::string(CharEnd, 10) << "<--------\n";
  if (CharStart == CharEnd)
    return {};
  std::string text(CharStart, CharEnd);
  errs() << "\n-------->" << text << "<--------\n";
  if (text.length() > 2 && text.substr(text.length() - 2) == "->") {
    errs() << "\ntrimmed for ->: " << text << "\n";
    return text.substr(0, text.length() - 2);
  } else if (text.length() > 1 && text.back() == '.') {
    errs() << "\ntrimmed for .\n";
    return text.substr(0, text.length() - 1);
  }
  return text;
}
}

void MemberBeginReplacer::run(const MatchFinder::MatchResult &Result) {
  const auto *MemberCall =
      Result.Nodes.getNodeAs<CXXMemberCallExpr>(CallExprID);

  const auto &SM = *Result.SourceManager;
  const auto *Member = dyn_cast<MemberExpr>(MemberCall->getCallee());
  if (Member == nullptr) {
    return;
  }
  auto RefName = getExprText(SM, *Member);
  errs() << "\n" << (RefName.empty() ? std::string("Empty Name") : RefName)
         << "\n";
  if (RefName.empty() ||
      RefName == "this") // It's calling this->begin() or begin()
    return;

  auto MethodName = MemberCall->getMethodDecl()->getNameAsString();
  // bool IsConst = Member->getBase()->getType().isConstQualified();
  // if (IsConst) {
  //   MethodName = "c" + MethodName;
  //   const auto *NonConstExpr = ignoreConstCasts(Member->getBase());
  //   RefName = getExprText(SM, *NonConstExpr);
  // }
  const auto StartLoc = SM.getSpellingLoc(MemberCall->getLocStart());
  const auto EndLoc = SM.getSpellingLoc(MemberCall->getLocEnd());

  // FIXME: add #include <iterator>
  std::string Replacement("std::" + MethodName + "(" + RefName);
  auto Length = SM.getFileOffset(EndLoc) - SM.getFileOffset(StartLoc);
  Owner.addReplacementForCurrentTU(
      tooling::Replacement(SM, StartLoc, Length, Replacement));
  ++AcceptedChanges;
}
