// Copyright 2021 <rinamuka4@gmail.com>
#ifndef INCLUDE_PREPARESUGGESTS_HPP_
#define INCLUDE_PREPARESUGGESTS_HPP_
#include <shared_mutex>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
struct sug {
  std::string id;
  std::string name;
  int cost;
};

struct sugUnit {
  std::string text;
  size_t position;
};

class preparerSug {
 public:
  explicit preparerSug(std::string filename) : filename_(filename){}

  void serveSuggestions();
  nlohmann::json getSuggestions(std::string input);

 private:
  nlohmann::json data;

  std::shared_mutex mutex;

  std::string filename_;
  std::vector<sug> suggestions;
};
#endif  // INCLUDE_PREPARESUGGESTS_HPP_
