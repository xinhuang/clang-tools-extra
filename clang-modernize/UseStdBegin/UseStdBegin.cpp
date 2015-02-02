#include "UseStdBegin.h"
#include "UseStdBeginActions.h"
#include "UseStdBeginMatchers.h"

using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

int UseStdBeginTransform::apply(const CompilationDatabase &Database,
                                const std::vector<std::string> &SourcePaths) {
  unsigned AcceptedChanges = 0;
  MatchFinder Finder;
  MemberBeginReplacer Replacer(AcceptedChanges, /*Owner =*/*this);

  Finder.addMatcher(makeMemberBeginMatcher(), &Replacer);

  ClangTool Tool(Database, SourcePaths);
  if (Tool.run(createActionFactory(Finder).get())) {
    llvm::errs() << "Error encountered during translation.\n";
    return 1;
  }

  setAcceptedChanges(AcceptedChanges);

  return 0;
}

struct UseStdBeginFactory : TransformFactory {
  UseStdBeginFactory() {
    Since.Clang = Version(3, 0);
    Since.Gcc = Version(4, 6);
    Since.Icc = Version(13);
    Since.Msvc = Version(11);
  }

  Transform *createTransform(const TransformOptions &Opts) override {
    return new UseStdBeginTransform(Opts);
  }
};

// Register the factory using this statically initialized variable.
static TransformFactoryRegistry::Add<UseStdBeginFactory>
X("use-std-begin", "Replace memberwise begin/end by std::begin/std::end or std::cbegin/std::cend"
                      " (EXPERIMENTAL)");

// This anchor is used to force the linker to link in the generated object file
// and thus register the factory.
volatile int UseStdBeginTransformAnchorSource = 0;
