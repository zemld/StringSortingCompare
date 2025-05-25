#pragma once
#include "lcp_comparator.h"
#include "sorter.h"
#include <memory>
#include <string>
#include <vector>

class StringMergeSorter : public Sorter {
private:
  void mergeSortRecursive(std::vector<std::string> &arr,
                          std::vector<std::string> &temp, int left, int right) {
    if (left < right) {
      int middle = left + (right - left) / 2;

      mergeSortRecursive(arr, temp, left, middle);
      mergeSortRecursive(arr, temp, middle + 1, right);

      merge(arr, temp, left, middle, right);
    }
  }

  void merge(std::vector<std::string> &arr, std::vector<std::string> &temp,
             int left, int middle, int right) {
    for (int i = left; i <= right; i++) {
      temp[i] = arr[i];
    }

    int i = left;
    int j = middle + 1;
    int k = left;

    while (i <= middle && j <= right) {
      if (comparator->CompareWithLCP(temp[i], temp[j]) <= 0) {
        arr[k] = temp[i];
        i++;
      } else {
        arr[k] = temp[j];
        j++;
      }
      k++;
    }

    while (i <= middle) {
      arr[k] = temp[i];
      i++;
      k++;
    }
  }

public:
  std::unique_ptr<LCPComparator> comparator;
  StringMergeSorter()
      : comparator(std::unique_ptr<LCPComparator>(new LCPComparator())) {}

  size_t GetComparisonCount() const override {
    return comparator->GetComparisonCount();
  }

  std::vector<std::string> Sort(std::vector<std::string> &arr) override {
    if (arr.size() <= 1) {
      return arr;
    }

    std::vector<std::string> temp(arr.size());
    mergeSortRecursive(arr, temp, 0, arr.size() - 1);

    return arr;
  }
};