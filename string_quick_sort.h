#pragma once
#include "lcp_comparator.h"
#include "sorter.h"
#include <string>
#include <vector>

class StringQuickSorter : public Sorter {
private:
  int makePartition(std::vector<std::string> &arr, int low, int high) {
    std::string pivot = arr[high];

    int i = low - 1;

    for (int j = low; j < high; j++) {
      if (comparator->CompareWithLCP(arr[j], pivot) <= 0) {
        i++;
        std::swap(arr[i], arr[j]);
      }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
  }

  void quickSortWithLCP(std::vector<std::string> &arr, int low, int high) {
    if (low < high) {
      int pivotIndex = makePartition(arr, low, high);

      quickSortWithLCP(arr, low, pivotIndex - 1);
      quickSortWithLCP(arr, pivotIndex + 1, high);
    }
  }

  void sort(std::vector<std::string> &arr) {
    if (arr.size() <= 1)
      return;
    quickSortWithLCP(arr, 0, arr.size() - 1);
  }

public:
  std::unique_ptr<LCPComparator> comparator;
  StringQuickSorter()
      : comparator(std::unique_ptr<LCPComparator>(new LCPComparator())) {}

  size_t GetComparisonCount() const override {
    return comparator->GetComparisonCount();
  }

  std::vector<std::string> Sort(std::vector<std::string> &to_sort) override {
    sort(to_sort);
    return to_sort;
  }
};