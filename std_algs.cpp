#include "ordinal_comparator.h"
#include "sorter.h"
#include "string_generator.h"
#include "string_merge_sort.h"
#include "string_quick_sort.h"
#include "string_radix_and_quick_sort.h"
#include "string_radix_sort.h"
#include "string_sort_tester.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct TestResults {
  std::vector<uint64_t> working_time;
  std::vector<size_t> comparisons;

  void Clear() {
    working_time.clear();
    comparisons.clear();
  }
};

class QuickSorter : public Sorter {
private:
  void quickSort(std::vector<std::string> &arr, int low, int high) {
    if (low < high) {
      std::string pivot = arr[high];

      int i = low - 1;

      for (int j = low; j <= high - 1; j++) {
        if (comparator->Compare(arr[j], pivot)) {
          i++;
          std::swap(arr[i], arr[j]);
        }
      }
      std::swap(arr[i + 1], arr[high]);

      int pivot_index = i + 1;

      quickSort(arr, low, pivot_index - 1);
      quickSort(arr, pivot_index + 1, high);
    }
  }

public:
  std::unique_ptr<OrdinalComparator> comparator;

  QuickSorter() : comparator(std::make_unique<OrdinalComparator>()) {}

  size_t GetComparisonCount() const override {
    return comparator->GetComparisonCount();
  }

  std::vector<std::string> Sort(std::vector<std::string> &to_sort) override {
    if (to_sort.size() <= 1) {
      return to_sort;
    }
    quickSort(to_sort, 0, to_sort.size() - 1);
    return to_sort;
  }
};

class MergeSorter : public Sorter {
private:
  void merge(std::vector<std::string> &arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<std::string> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
      L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
      R[j] = arr[m + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2) {
      if (comparator->Compare(L[i], R[j])) {
        arr[k] = L[i];
        i++;
      } else {
        arr[k] = R[j];
        j++;
      }
      k++;
    }

    while (i < n1) {
      arr[k] = L[i];
      i++;
      k++;
    }

    while (j < n2) {
      arr[k] = R[j];
      j++;
      k++;
    }
  }

  void mergeSort(std::vector<std::string> &arr, int l, int r) {
    if (l < r) {
      int m = l + (r - l) / 2;

      mergeSort(arr, l, m);
      mergeSort(arr, m + 1, r);

      merge(arr, l, m, r);
    }
  }

public:
  std::unique_ptr<OrdinalComparator> comparator;

  MergeSorter() : comparator(std::make_unique<OrdinalComparator>()) {}

  size_t GetComparisonCount() const override {
    return comparator->GetComparisonCount();
  }

  std::vector<std::string> Sort(std::vector<std::string> &to_sort) override {
    if (to_sort.size() <= 1) {
      return to_sort;
    }
    mergeSort(to_sort, 0, to_sort.size() - 1);
    return to_sort;
  }
};

void writeHeaderToCSV(const std::string &algorithm_name,
                      const std::string &test_type) {
  std::string filename = algorithm_name + "_" + test_type + ".csv";
  bool file_exists = false;
  std::ifstream check_file(filename);
  if (check_file.good()) {
    file_exists = true;
  }
  check_file.close();

  std::ofstream file(filename, std::ios::app);

  if (file.is_open()) {
    if (!file_exists) {
      file << "string_len;working_time;symbols_comparision\n";
    }
    file.close();
  }
}

void writeResultsToCSV(const std::string &algorithm_name,
                       const std::string &test_type,
                       std::vector<double> &string_lengths,
                       std::vector<uint64_t> &working_time,
                       std::vector<size_t> &comparisons) {
  std::string filename = algorithm_name + "_" + test_type + ".csv";
  std::ofstream file(filename, std::ios::app);

  if (file.is_open()) {
    size_t records = string_lengths.size();
    for (size_t i = 0; i < records; ++i) {
      file << string_lengths[i] << ";" << working_time[i] << ";"
           << comparisons[i] << std::endl;
    }
    file.close();
  } else {
    std::cerr << "Error: Could not open file " << filename << std::endl;
  }
}

void writeCSV(const std::string &algorithm_name, const std::string &test_type,
              std::vector<double> &string_lengths,
              std::vector<uint64_t> &working_time,
              std::vector<size_t> &comparisons) {
  writeHeaderToCSV(algorithm_name, test_type);
  writeResultsToCSV(algorithm_name, test_type, string_lengths, working_time,
                    comparisons);
}

double avg_string_length(const std::vector<std::string> &data, size_t begin,
                         size_t end) {
  double total_length = 0;
  for (size_t i = begin; i <= end && i < data.size(); ++i) {
    total_length += data[i].length();
  }
  return total_length / (end - begin + 1);
}

void MakeTest(StringSortTester &tester, std::vector<std::string> &data,
              TestResults &results) {
  auto result = tester.Sort(data);
  results.working_time.push_back(result.time_taken);
  results.comparisons.push_back(result.comparisons);
}

int main() {
  StringSortTester std_quick_sort_tester(new QuickSorter());
  StringSortTester std_merge_sort_tester(new MergeSorter());
  StringSortTester string_quick_sort_tester(new StringQuickSorter());
  StringSortTester string_merge_sort_tester(new StringMergeSorter());
  StringSortTester string_radix_and_quick_sort_tester(
      new StringRadixAndQuickSorter());
  StringSortTester string_radix_sort_tester(new StringMSDRadixSorter());
  std::vector<double> string_lengths;
  TestResults std_quick_sort_results;
  TestResults std_merge_sort_results;
  TestResults string_quick_sort_results;
  TestResults string_merge_sort_results;
  TestResults string_radix_and_quick_sort_results;
  TestResults string_radix_sort_results;
  for (int type = 1; type <= 3; ++type) {
    std::string test_type;
    if (type == 1) {
      test_type += "random";
    } else if (type == 2) {
      test_type += "reversed";
    } else {
      test_type += "sorted";
    }
    for (size_t i = 100; i <= 3000; i += 100) {
      StringGenerator string_generator;
      StringGenerator::StringLenBounds bounds{10, 200};
      std::vector<std::string> data = string_generator.GenerateVectorOfStrings(
          i, bounds, static_cast<StringGenerator::StringType>(type));
      MakeTest(std_quick_sort_tester, data, std_quick_sort_results);
      MakeTest(std_merge_sort_tester, data, std_merge_sort_results);
      MakeTest(string_quick_sort_tester, data, string_quick_sort_results);
      MakeTest(string_merge_sort_tester, data, string_merge_sort_results);
      MakeTest(string_radix_and_quick_sort_tester, data,
               string_radix_and_quick_sort_results);
      MakeTest(string_radix_sort_tester, data, string_radix_sort_results);
      string_lengths.push_back(avg_string_length(data, 0, i - 1));
    }

    writeCSV("std_quick_sort", test_type, string_lengths,
             std_quick_sort_results.working_time,
             std_quick_sort_results.comparisons);
    writeCSV("std_merge_sort", test_type, string_lengths,
             std_merge_sort_results.working_time,
             std_merge_sort_results.comparisons);
    writeCSV("string_quick_sort", test_type, string_lengths,
             string_quick_sort_results.working_time,
             string_quick_sort_results.comparisons);
    writeCSV("string_merge_sort", test_type, string_lengths,
             string_merge_sort_results.working_time,
             string_merge_sort_results.comparisons);
    writeCSV("string_radix_and_quick_sort", test_type, string_lengths,
             string_radix_and_quick_sort_results.working_time,
             string_radix_and_quick_sort_results.comparisons);
    writeCSV("string_radix_sort", test_type, string_lengths,
             string_radix_sort_results.working_time,
             string_radix_sort_results.comparisons);
    std_quick_sort_results.Clear();
    std_merge_sort_results.Clear();
    string_quick_sort_results.Clear();
    string_merge_sort_results.Clear();
    string_radix_and_quick_sort_results.Clear();
    string_radix_sort_results.Clear();
    string_lengths.clear();
  }
  return 0;
}
