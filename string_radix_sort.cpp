#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

void makeInputFast();
template <typename T> T Enter();
template <typename T> std::vector<T> EnterVector(size_t size);
void printArray(const std::vector<std::string> &arr);

class StringMSDRadixSorter {
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

public:
  void sort(std::vector<std::string> &strings) {
    if (strings.size() <= 1) {
      return;
    }

    aux.resize(strings.size());
    msdRadixSort(strings, 0, strings.size() - 1, 0);
  }
};

int main() {
  makeInputFast();
  int n = Enter<int>();
  auto strings = EnterVector<std::string>(n);

  StringMSDRadixSorter sorter;
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
