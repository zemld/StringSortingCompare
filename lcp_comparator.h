#pragma once
#include "comparator.h"

class LCPComparator : public Comparator {
public:
  int СalculateLCP(const std::string &str1, const std::string &str2) {
    int minLength = std::min(str1.length(), str2.length());
    int i = 0;

    while (i < minLength && str1[i] == str2[i]) {
      ++comparison_count;
      ++i;
    }

    return i;
  }

  int CompareWithLCP(const std::string &str1, const std::string &str2) {
    int lcp = СalculateLCP(str1, str2);

    if (lcp == str1.length() && lcp == str2.length()) {
      return 0;
    } else if (lcp == str1.length()) {
      return -1;
    } else if (lcp == str2.length()) {
      return 1;
    }
    ++comparison_count;
    return (str1[lcp] < str2[lcp]) ? -1 : 1;
  }
};