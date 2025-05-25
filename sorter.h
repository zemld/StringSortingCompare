#pragma once
#include "comparator.h"
#include <memory>
#include <string>
#include <vector>

class Sorter {
public:
  virtual std::vector<std::string> Sort(std::vector<std::string> &to_sort) = 0;
  virtual size_t GetComparisonCount() const = 0;
};
