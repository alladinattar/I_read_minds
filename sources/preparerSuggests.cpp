#include <fstream>
#include <iostream>
#include <thread>

#include "nlohmann/json.hpp"
#include "prepareSuggests.hpp"
#include "unistd.h"
using nlohmann::json;

auto get_id(const json& j) -> std::string { return j.get<std::string>(); }
auto get_name(const json& j) -> std::string { return j.get<std::string>(); }
auto get_cost(const json& j) -> int { return j.get<int>(); }

void from_json(const json& j, sug& s) {
  s.id = get_id(j.at("id"));
  s.name = get_name(j.at("name"));
  s.cost = get_cost(j.at("cost"));
}

void preparerSug::serveSuggestions() {
  mutex.lock();
  suggestions.clear();
  if (filename_.empty())
    throw std::invalid_argument("the path isn't available");
  std::ifstream file(filename_);
  if (!file) {
    throw std::out_of_range{"unable to open json: " + filename_};
  }
  data.clear();
  file >> data;

  for (auto const& item : data.at("suggestions")) {
    auto sugObj = item.get<sug>();
    suggestions.push_back(sugObj);
  }
  std::cout<<suggestions[0].name<<std::endl;
  mutex.unlock();
  sleep(5);
  //
}


std::vector<sugUnit> preparerSug::getSuggestions(std::string input) {
  
}