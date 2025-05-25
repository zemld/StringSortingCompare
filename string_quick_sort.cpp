#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void makeInputFast();
template <typename T> T Enter();
template <typename T> std::vector<T> EnterVector(size_t size);
void printArray(const std::vector<std::string> &arr);

class StringQuickSorter {
private:
  int calculateLCP(const std::string &str1, const std::string &str2) {
    int minLength = std::min(str1.length(), str2.length());
    int i = 0;

    while (i < minLength && str1[i] == str2[i]) {
      i++;
    }

    return i;
  }

  int compareWithLCP(const std::string &str1, const std::string &str2) {
    int lcp = calculateLCP(str1, str2);

    if (lcp == str1.length() && lcp == str2.length()) {
      return 0;
    } else if (lcp == str1.length()) {
      return -1;
    } else if (lcp == str2.length()) {
      return 1;
    }

    return (str1[lcp] < str2[lcp]) ? -1 : 1;
  }

  int makePartition(std::vector<std::string> &arr, int low, int high) {
    std::string pivot = arr[high];

    int i = low - 1;

    for (int j = low; j < high; j++) {
      if (compareWithLCP(arr[j], pivot) <= 0) {
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

public:
  void sort(std::vector<std::string> &arr) {
    if (arr.size() <= 1)
      return;
    quickSortWithLCP(arr, 0, arr.size() - 1);
  }
};

int main() {
  makeInputFast();
  int n = Enter<int>();
  auto strings = EnterVector<std::string>(n);

  StringQuickSorter sorter;
  sorter.sort(strings);

  printArray(strings);

  return 0;
}

void makeInputFast() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
}

template <typename T> T Enter() {
  T value;
  std::cin >> value;
  return value;
}

template <typename T> std::vector<T> EnterVector(size_t size) {
  std::vector<T> vector(size);
  for (size_t i = 0; i != size; ++i) {
    std::cin >> vector[i];
  }
  return vector;
}

void printArray(const std::vector<std::string> &arr) {
  for (const auto &str : arr) {
    std::cout << str << std::endl;
  }
}
