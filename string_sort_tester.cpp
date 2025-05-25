#include "string_sort_tester.h"

size_t StringSortTester::GetComparisonCount() const { return comparison_count; }

void StringSortTester::ResetComparisonCount() { comparison_count = 0; }

void StringSortTester::StartTimer() {
  time_point = std::chrono::high_resolution_clock::now();
}

uint64_t StringSortTester::GetTime() {
  auto end_time = std::chrono::high_resolution_clock::now();
  uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          end_time - time_point)
                          .count();
  time_point = end_time;
  return duration;
}

StringSortTester::SorterResult
StringSortTester::Sort(std::vector<std::string> &to_sort) {
  ResetComparisonCount();
  StartTimer();
  auto sorted = sorter->Sort(to_sort);
  auto time = GetTime();
  auto comparisons = sorter->GetComparisonCount();
  return SorterResult{sorted, time, comparisons};
}
