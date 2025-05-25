#pragma once
#include <random>
#include <string>
#include <vector>

class StringGenerator {
public:
  struct StringLenBounds {
    size_t minLength;
    size_t maxLength;
  };

private:
  const std::string symbols =
      "0123456789!@#%:;^&*()-"
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::mt19937 rng;
  std::uniform_int_distribution<std::mt19937::result_type> dist;

  void initRandom();
  size_t getRandomIndex();
  size_t getRandomLength(const StringLenBounds &bounds);

  void shuffleSymbolsALittleBit(std::string &str);

public:
  StringGenerator();

  enum class StringType {
    RANDOM = 1,
    REVERSED = 2,
    SORTED = 3,
  };

  std::string GenerateRandomString(StringLenBounds &,
                                   StringType = StringType::RANDOM);
  std::vector<std::string> GenerateVectorOfStrings(size_t, StringLenBounds &,
                                                   StringType);
};