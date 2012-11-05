//===--- CC1AsOptions.cpp - Clang Assembler Options Table -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "clang/Driver/CC1AsOptions.h"
#include "clang/Driver/Option.h"

#include "llvm/Support/OptTable.h"

using namespace clang;
using namespace clang::driver;
using namespace clang::driver::options;
using namespace clang::driver::cc1asoptions;

using namespace llvm::options;

#define PREFIX(NAME, VALUE) const char *const NAME[] = VALUE;
#include "clang/Driver/CC1AsOptions.inc"
#undef PREFIX

static const llvm::OptTable::Info CC1AsInfoTable[] = {
#define OPTION(PREFIX, NAME, ID, KIND, GROUP, ALIAS, FLAGS, PARAM, \
               HELPTEXT, METAVAR)   \
  { PREFIX, NAME, HELPTEXT, METAVAR, OPT_##ID, Option::KIND##Class, PARAM, \
    FLAGS, OPT_##GROUP, OPT_##ALIAS },
#include "clang/Driver/CC1AsOptions.inc"
#undef OPTION
};

namespace {

class CC1AsOptTable : public llvm::OptTable {
public:
  CC1AsOptTable()
    : llvm::OptTable(CC1AsInfoTable,
               sizeof(CC1AsInfoTable) / sizeof(CC1AsInfoTable[0])) {}
};

}

llvm::OptTable *clang::driver::createCC1AsOptTable() {
  return new CC1AsOptTable();
}
