#include "string_generator.h"
#include <algorithm>

void StringGenerator::initRandom() {
  std::random_device rd;
  rng = std::mt19937(rd());
  dist = std::uniform_int_distribution<std::mt19937::result_type>(
      0, symbols.size() - 1);
}

size_t StringGenerator::getRandomIndex() { return dist(rng); }

size_t StringGenerator::getRandomLength(const StringLenBounds &bounds) {
  std::uniform_int_distribution<size_t> len_dist(bounds.minLength,
                                                 bounds.maxLength);
  return len_dist(rng);
}

void StringGenerator::shuffleSymbolsALittleBit(std::string &str) {
  std::mt19937 shuffle_rng(std::random_device{}());
  std::uniform_int_distribution<size_t> index_dist(0, str.size() - 1);
  size_t shuffle_count = str.size() / 10;

  for (size_t i = 0; i <= shuffle_count; ++i) {
    size_t idx1 = index_dist(shuffle_rng);
    size_t idx2 = index_dist(shuffle_rng);
    std::swap(str[idx1], str[idx2]);
  }
}

StringGenerator::StringGenerator() { initRandom(); }

std::string
StringGenerator::GenerateRandomString(StringGenerator::StringLenBounds &bounds,
                                      StringGenerator::StringType type) {
  size_t length = getRandomLength(bounds);
  std::string randomString;
  for (size_t i = 0; i < length; ++i) {
    randomString += symbols[getRandomIndex()];
  }
  if (type == StringType::RANDOM) {
    return randomString;
  }
  if (type == StringType::REVERSED) {
    std::reverse(randomString.begin(), randomString.end());
  } else if (type == StringType::SORTED) {
    std::sort(randomString.begin(), randomString.end());
  }
  shuffleSymbolsALittleBit(randomString);
  return randomString;
}

std::vector<std::string> StringGenerator::GenerateVectorOfStrings(
    size_t count, StringGenerator::StringLenBounds &bounds,
    StringGenerator::StringType type) {
  std::vector<std::string> strings;
  strings.reserve(count);
  for (size_t i = 0; i < count; ++i) {
    strings.push_back(GenerateRandomString(bounds, type));
  }
  return strings;
}