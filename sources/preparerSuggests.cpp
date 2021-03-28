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
  mutex.unlock();
  sleep(5);
  //
}

void to_json(json& j, const sugUnit& s) {
  j = json{{"text", s.text}, {"position", s.position}};
}

bool my_cmp(const sug& a, const sug& b) {
  // smallest comes first
  return a.cost > b.cost;
}

json preparerSug::getSuggestions(std::string input) {
  std::vector<json> goodSugs;
  std::sort(suggestions.begin(), suggestions.end(), my_cmp);

  for (size_t i = 0; i < suggestions.size(); ++i) {
    if (suggestions[i].id == input) {
      json gsug;
      sugUnit su = sugUnit{suggestions[i].name, i};
      to_json(gsug, su);
      goodSugs.push_back(gsug);
    }
  }


  json j_goodSugs(goodSugs);
  return j_goodSugs;
}