// RUN: %clang_cc1 -x objective-c++ -triple x86_64-apple-darwin11 -fsyntax-only -fobjc-fragile-abi -verify %s
// rdar://10731065

@interface MyView {}
@end

@implementation MyViewTemplate // expected-warning {{cannot find interface declaration for 'MyViewTemplate'}}
- (id) createRealObject {
  id realObj;
  *(MyView *) realObj = *(MyView *) self; // expected-error {{cannot assign to class object}}
}
@end

