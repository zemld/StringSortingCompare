#pragma once
#include "comparator.h"
#include <string>

class OrdinalComparator : public Comparator {
public:
  bool Compare(const std::string &a, const std::string &b) {
    size_t min_length = std::min(a.length(), b.length());
    for (size_t i = 0; i < min_length; ++i) {
      if (a[i] < b[i]) {
        ++comparison_count;
        return true;
      }
      if (a[i] > b[i]) {
        ++comparison_count;
        return false;
      }
    }
    return a.length() < b.length();
  }
};