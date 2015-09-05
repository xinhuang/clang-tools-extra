// RUN: grep -Ev "// *[A-Z-]+:" %s > %t.cpp
// RUN: clang-modernize -final-syntax-check -use-std-begin %t.cpp -- --std=c++11 -I %S
// RUN: FileCheck -input-file=%t.cpp %s

#include <iterator>

struct MyVector {
  int begin() { return 0; }
  int begin() const { return 0; }
  int end() { return 10; }
  int end() const { return 10; }
};

void func() {
  MyVector a;
  for (auto iter = a.begin(); iter != a.end(); ++iter) {}
  // CHECK: for (auto iter = std::begin(a); iter != std::end(a); ++iter) {}

  MyVector c;
  for (auto iter = c.begin(); iter != c.end(); ++iter) {}
  // CHECK: for (auto iter = std::begin(c); iter != std::end(c); ++iter) {}
}
