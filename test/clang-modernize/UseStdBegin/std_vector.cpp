// RUN: grep -Ev "// *[A-Z-]+:" %s > %t.cpp
// RUN: clang-modernize -final-syntax-check -use-std-begin %t.cpp -- --std=c++11 -I %S
// RUN: FileCheck -input-file=%t.cpp %s

#include <vector>
#include <iterator>

void func() {
  std::vector<int> a;
  for (auto iter = a.begin(); iter != a.end(); ++iter) {}
  // CHECK: for (auto iter = std::begin(a); iter != std::end(a); ++iter) {}

  for (auto iter = const_cast<const std::vector<int>&>(a).begin(); iter != const_cast<const std::vector<int>&>(a).end(); ++iter) {}
  // CHECK: for (auto iter = std::begin(const_cast<const std::vector<int>&>(a)); iter != std::end(const_cast<const std::vector<int>&>(a)); ++iter) {}
}
