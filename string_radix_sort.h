#pragma once
#include "ordinal_comparator.h"
#include "sorter.h"
#include <string>
#include <vector>

class StringMSDRadixSorter : public Sorter {
private:
  const int ALPHABET_SIZE = 256;
  std::vector<std::string> aux;

  void msdRadixSort(std::vector<std::string> &arr, int lo, int hi, int d) {
    if (hi <= lo) {
      return;
    }

    std::vector<int> count(ALPHABET_SIZE, 0);

    for (int i = lo; i <= hi; i++) {
      int c = (d < arr[i].length()) ? (unsigned char)arr[i][d] : 0;
      count[c]++;
    }

    for (int r = 1; r < ALPHABET_SIZE; r++) {
      count[r] += count[r - 1];
    }

    for (int i = hi; i >= lo; i--) {
      int c = (d < arr[i].length()) ? (unsigned char)arr[i][d] : 0;
      aux[lo + --count[c]] = arr[i];
    }

    for (int i = lo; i <= hi; i++) {
      arr[i] = aux[i];
    }

    for (int r = 0; r < ALPHABET_SIZE; r++) {
      int start = lo + (r > 0 ? count[r - 1] : 0);
      int end = lo + count[r] - 1;

      if (start <= end) {
        if (r > 0) {
          msdRadixSort(arr, start, end, d + 1);
        }
      }
    }
  }

  void sort(std::vector<std::string> &strings) {
    if (strings.size() <= 1) {
      return;
    }

    aux.resize(strings.size());
    msdRadixSort(strings, 0, strings.size() - 1, 0);
  }

public:
  std::unique_ptr<OrdinalComparator> comparator;
  StringMSDRadixSorter()
      : comparator(
            std::unique_ptr<OrdinalComparator>(new OrdinalComparator())) {}

  size_t GetComparisonCount() const override {
    return comparator->GetComparisonCount();
  }
  std::vector<std::string> Sort(std::vector<std::string> &to_sort) override {
    sort(to_sort);
    return to_sort;
  }
};