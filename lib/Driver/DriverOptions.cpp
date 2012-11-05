//===--- DriverOptions.cpp - Driver Options Table -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "clang/Driver/Options.h"
#include "clang/Driver/Option.h"

#include "llvm/Support/OptTable.h"

using namespace clang::driver;
using namespace clang::driver::options;

using namespace llvm::options;

#define PREFIX(NAME, VALUE) const char *const NAME[] = VALUE;
#include "clang/Driver/Options.inc"
#undef PREFIX

static const llvm::OptTable::Info InfoTable[] = {
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, FLAGS, PARAM, \
               HELPTEXT, METAVAR)   \
  { PREFIX, NAME, HELPTEXT, METAVAR, OPT_##ID, Option::KIND##Class, PARAM, \
    FLAGS, OPT_##GROUP, OPT_##ALIAS },
#include "clang/Driver/Options.inc"
};

namespace {

class DriverOptTable : public llvm::OptTable {
public:
  DriverOptTable()
    : llvm::OptTable(InfoTable, sizeof(InfoTable) / sizeof(InfoTable[0])) {}
};

}

llvm::OptTable *clang::driver::createDriverOptTable() {
  return new DriverOptTable();
}
