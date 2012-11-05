//===--- Option.h - Abstract Driver Options ---------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_DRIVER_OPTION_H_
#define CLANG_DRIVER_OPTION_H_

#include "llvm/Support/Option.h"

namespace clang {
namespace driver {
namespace options {
  /// Flags specifically for clang options.
  enum ClangFlags {
    DriverOption     = (1 << 4),
    LinkerInput      = (1 << 5),
    NoArgumentUnused = (1 << 6),
    NoForward        = (1 << 7),
    Unsupported      = (1 << 8),
    CC1Option        = (1 << 9)
  };
}
} // end namespace driver
} // end namespace clang

#endif
