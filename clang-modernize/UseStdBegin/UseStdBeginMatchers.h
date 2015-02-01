#ifndef CLANG_MODERNIZE_USE_STD_BEGIN_MATCHERS_H
#define CLANG_MODERNIZE_USE_STD_BEGIN_MATCHERS_H

#include "clang/ASTMatchers/ASTMatchers.h"

extern const char *ClassID;
extern const char *CallExprID;

clang::ast_matchers::StatementMatcher makeMemberBeginMatcher();

#endif // CLANG_MODERNIZE_USE_STD_BEGIN_MATCHERS_H
