#ifndef INCLUDE_PREPARESUGGESTS_HPP_
#define INCLUDE_PREPARESUGGESTS_HPP_
#include <shared_mutex>
#include "nlohmann/json.hpp"
struct sug{
  std::string id;
  std::string name;
  int cost;
};

struct sugUnit{//with position
  std::string text;
  size_t position;
};

class preparerSug{
 public:
  preparerSug(std::string filename):filename_(filename){};

  void serveSuggestions();//every 15 min openfile read close and update collection  //simple lock infinitive loop with sleep
  nlohmann::json getSuggestions(std::string input);//using lockshared

 private:
  nlohmann::json data;

  std::shared_mutex mutex;

  std::string filename_;
  std::vector<sug> suggestions;
};
#endif