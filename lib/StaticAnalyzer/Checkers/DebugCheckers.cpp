//==- DebugCheckers.cpp - Debugging Checkers ---------------------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines a checkers that display debugging information.
//
//===----------------------------------------------------------------------===//

#include "ClangSACheckers.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"
#include "clang/Analysis/Analyses/LiveVariables.h"
#include "clang/Analysis/Analyses/Dominators.h"
#include "llvm/Support/Process.h"

using namespace clang;
using namespace ento;

//===----------------------------------------------------------------------===//
// DominatorsTreeDumper
//===----------------------------------------------------------------------===//

namespace {
class DominatorsTreeDumper : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager& mgr,
                        BugReporter &BR) const {
    if (AnalysisDeclContext *AC = mgr.getAnalysisDeclContext(D)) {
      DominatorTree dom;
      dom.buildDominatorTree(*AC);
      dom.dump();
    }
  }
};
}

void ento::registerDominatorsTreeDumper(CheckerManager &mgr) {
  mgr.registerChecker<DominatorsTreeDumper>();
}

//===----------------------------------------------------------------------===//
// LiveVariablesDumper
//===----------------------------------------------------------------------===//

namespace {
class LiveVariablesDumper : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager& mgr,
                        BugReporter &BR) const {
    if (LiveVariables* L = mgr.getAnalysis<LiveVariables>(D)) {
      L->dumpBlockLiveness(mgr.getSourceManager());
    }
  }
};
}

void ento::registerLiveVariablesDumper(CheckerManager &mgr) {
  mgr.registerChecker<LiveVariablesDumper>();
}

//===----------------------------------------------------------------------===//
// CFGViewer
//===----------------------------------------------------------------------===//

namespace {
class CFGViewer : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager& mgr,
                        BugReporter &BR) const {
    if (CFG *cfg = mgr.getCFG(D)) {
      cfg->viewCFG(mgr.getLangOptions());
    }
  }
};
}

void ento::registerCFGViewer(CheckerManager &mgr) {
  mgr.registerChecker<CFGViewer>();
}

//===----------------------------------------------------------------------===//
// CFGDumper
//===----------------------------------------------------------------------===//

namespace {
class CFGDumper : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager& mgr,
                        BugReporter &BR) const {
    if (CFG *cfg = mgr.getCFG(D)) {
      cfg->dump(mgr.getLangOptions(),
                llvm::sys::Process::StandardErrHasColors());
    }
  }
};
}

void ento::registerCFGDumper(CheckerManager &mgr) {
  mgr.registerChecker<CFGDumper>();
}
