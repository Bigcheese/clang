// RUN: %clang_cc1 -fsyntax-only -verify -Wformat-nonliteral -pedantic %s

#include <stdarg.h>

extern "C" {
extern int scanf(const char *restrict, ...);
extern int printf(const char *restrict, ...);
extern int vprintf(const char *restrict, va_list);
}

void f(char **sp, float *fp) {
  // TODO: Warn that the 'a' length modifier is an extension.
  scanf("%as", sp);

  // TODO: Warn that the 'a' conversion specifier is a C++11 feature.
  printf("%a", 1.0);
  scanf("%afoobar", fp);
}

void g() {
  printf("%ls", "foo"); // expected-warning{{format specifies type 'wchar_t *' but the argument has type 'const char *'}}
}

// Test that we properly handle format_idx on C++ members.
class Foo {
public:
  const char *gettext(const char *fmt) __attribute__((format_arg(2)));

  int scanf(const char *, ...) __attribute__((format(scanf, 2, 3)));
  int printf(const char *, ...) __attribute__((format(printf, 2, 3)));
  int printf2(const char *, ...);

  static const char *gettext_static(const char *fmt) __attribute__((format_arg(1)));
  static int printf_static(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
};

void h(int *i) {
  Foo foo;
  foo.scanf("%d"); // expected-warning{{more '%' conversions than data arguments}}
  foo.printf("%d", i); // expected-warning{{format specifies type 'int' but the argument has type 'int *'}}
  Foo::printf_static("%d", i); // expected-warning{{format specifies type 'int' but the argument has type 'int *'}}

  printf(foo.gettext("%d"), i); // expected-warning{{format specifies type 'int' but the argument has type 'int *'}}
  printf(Foo::gettext_static("%d"), i); // expected-warning{{format specifies type 'int' but the argument has type 'int *'}}
}

// Test handling __null for format string literal checking.
extern "C" {
  int test_null_format(const char *format, ...) __attribute__((__format__ (__printf__, 1, 2)));
}

void rdar8269537(const char *f)
{
  test_null_format(false); // expected-warning {{null from a constant boolean}}
  test_null_format(0); // no-warning
  test_null_format(__null); // no-warning
  test_null_format(f); // expected-warning {{not a string literal}}
}

int Foo::printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  const char * const format = fmt;
  vprintf(format, ap); // no-warning

  const char *format2 = fmt;
  vprintf(format2, ap); // expected-warning{{format string is not a string literal}}

  return 0;
}

int Foo::printf2(const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  vprintf(fmt, ap); // expected-warning{{format string is not a string literal}}

  return 0;
}
