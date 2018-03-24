// allocator.cpp in Otus homework #3 project
#include <iostream>
#include "./headers/custom_allocator.h"
#include "./headers/custom_map.h"

using namespace std;

int main()
{
  custom_map<int, std::string> myMap{};
  for (auto iter : myMap)
    std::cout << iter.first;
  return 0;
}
