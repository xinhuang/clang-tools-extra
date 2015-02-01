#include "UseStdBeginMatchers.h"

using namespace clang::ast_matchers;

const char *ClassID = "class";
const char *CallExprID = "callExpr";

clang::ast_matchers::StatementMatcher makeMemberBeginMatcher() {
  return memberCallExpr(
             hasDeclaration(methodDecl(ofClass(
                 recordDecl(allOf(hasMethod(hasName("begin")),
                                  hasMethod(hasName("end")))).bind(ClassID)))),
             callee(methodDecl(anyOf(hasName("begin"), hasName("end")))))
      .bind(CallExprID);
}
