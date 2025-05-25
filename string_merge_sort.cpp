#include <iostream>
#include <string>
#include <vector>

void makeInputFast();
template <typename T> T Enter();
template <typename T> std::vector<T> EnterVector(size_t size);
void printArray(const std::vector<std::string> &arr);

class StringMergeSorter {
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
      if (compareWithLCP(temp[i], temp[j]) <= 0) {
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
  std::vector<std::string> sort(std::vector<std::string> &arr) {
    if (arr.size() <= 1) {
      return arr;
    }

    std::vector<std::string> temp(arr.size());
    mergeSortRecursive(arr, temp, 0, arr.size() - 1);

    return arr;
  }
};

int main() {
  makeInputFast();
  int n = Enter<int>();
  auto strings = EnterVector<std::string>(n);

  StringMergeSorter sorter;
  std::vector<std::string> sortedStrings = sorter.sort(strings);

  printArray(sortedStrings);
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
