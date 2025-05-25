#pragma once
#include "comparator.h"
#include "sorter.h"
#include <chrono>
#include <memory>

class StringSortTester {
private:
  size_t comparison_count;
  std::chrono::steady_clock::time_point time_point;
  size_t GetComparisonCount() const;
  void ResetComparisonCount();
  void StartTimer();
  uint64_t GetTime();

public:
  struct SorterResult {
  public:
    std::vector<std::string> sorted_data;
    uint64_t time_taken;
    size_t comparisons;
  };
  std::unique_ptr<Sorter> sorter;
  StringSortTester(Sorter *sorter) : comparison_count(0), sorter(sorter) {}
  SorterResult Sort(std::vector<std::string> &to_sort);
};