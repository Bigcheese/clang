// RUN: rm -rf %t
// RUN: %clang_cc1 -fmodules -fmodule-cache-path %t -F %S/Inputs -I %S/Inputs -verify %s

#import "point.h"
@import Module;
#import "point.h"

