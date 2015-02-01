//===-- UseStdBegin.h ------- member-wise begin/end replacement -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief This file provides the declaration of the UseStdBegin class.
///
//===----------------------------------------------------------------------===//

#ifndef CLANG_MODERNIZE_USE_STD_BEGIN_H
#define CLANG_MODERNIZE_USE_STD_BEGIN_H

#include "Core/Transform.h"
#include "llvm/Support/Compiler.h"

class UseStdBeginTransform : public Transform {
public:
  UseStdBeginTransform(const TransformOptions &Options)
      : Transform("UseStdBegin", Options) {}

  /// \see Transform::run().
  virtual int apply(const clang::tooling::CompilationDatabase &Database,
                    const std::vector<std::string> &SourcePaths) override;
};

#endif // CLANG_MODERNIZE_USE_STD_BEGIN_H
