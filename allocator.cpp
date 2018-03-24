// allocator.cpp in Otus homework #3 project
#include <iostream>
#include "./headers/custom_allocator.h"
#include "./headers/custom_map.h"

using namespace std;

int main()
{
  custom_map<int, std::string> myMap{};
  for (size_t idx{}; idx < 100; idx++)
    myMap.insert(std::make_pair<int, std::string>(idx, std::to_string(idx)));
  for (auto iter : myMap)
    std::cout << iter.first;
  custom_map<int, std::string>::iterator iter{myMap.begin()};
  iter->first;
  return 0;
}
