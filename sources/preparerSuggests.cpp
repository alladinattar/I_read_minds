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
  while (true) {
    mutex.lock();
    std::ifstream file(filename_);
    suggestions.clear();
    if (filename_.empty())
      throw std::invalid_argument("the path isn't available");

    if (!file) {
      throw std::out_of_range{"unable to open json: " + filename_};
    }
    data.clear();
    file >> data;
    file.close();
    for (auto const& item : data.at("suggestions")) {
      auto sugObj = item.get<sug>();
      suggestions.push_back(sugObj);
    }
    mutex.unlock();

    sleep(40);
  }
}

void to_json(json& j, const sugUnit& s) {
  j = json{{"text", s.text}, {"position", s.position}};
}

bool my_cmp(const sug& a, const sug& b) {
  // smallest comes first
  return a.cost > b.cost;
}

json preparerSug::getSuggestions(std::string input) {
  auto j = json::parse(input);
  std::vector<json> goodSugs;
  mutex.lock();
  std::sort(suggestions.begin(), suggestions.end(), my_cmp);
  for (size_t i = 0; i < suggestions.size(); ++i) {
    if (suggestions[i].id == j["input"].get<std::string>()) {
      json gsug;
      sugUnit su = sugUnit{suggestions[i].name, i};
      to_json(gsug, su);
      goodSugs.push_back(gsug);
    }
  }
  mutex.unlock();
  json ja ;
  ja["suggestions"] = goodSugs;
  //json j_goodSugs(goodSugs);

  return ja;
}