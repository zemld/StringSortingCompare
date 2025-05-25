#pragma once
#include <string>

class Comparator {
protected:
  size_t comparison_count;

public:
  Comparator() : comparison_count(0) {}

  bool Compare(const std::string &a, const std::string &b);

  size_t GetComparisonCount() const { return comparison_count; }

  void ResetComparisonCount() { comparison_count = 0; }
};