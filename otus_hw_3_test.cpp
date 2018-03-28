// custom_map.h in Otus homework #3 project

#define BOOST_TEST_MODULE Otus_HW_3_TEST

#include <boost/test/unit_test.hpp>
#include "./headers/custom_allocator.h"
#include "./headers/custom_map.h"
#include <cstdlib>
#include <iostream>

uint factorial(uint n)
{
  uint result{1};
  for (uint i{2}; i <= n; i++)
    result *= i;
  return result;
}

BOOST_AUTO_TEST_SUITE(homework_3_test)

BOOST_AUTO_TEST_CASE(map_test_insert)
{
  custom_map<int, int> map3{};
  for (int idx{}; idx < 10; idx++)
    map3.insert(std::pair<const int, int>{idx, factorial(idx)});

  custom_map<int, int> testMap{};
  testMap.insert(0, 0);
  testMap.insert(1, 1);
  testMap.insert(2, 2);
  size_t presentNodesCount{};
  for(auto item:testMap)
  {
    presentNodesCount++;
  }
  BOOST_CHECK(presentNodesCount == testMap.count() &&
              presentNodesCount == 3);
}

BOOST_AUTO_TEST_CASE(map_test_sorting)
{
  custom_map<std::string, std::string>testMap{};
  testMap.insert(std::string{"asDFGH d"}, std::string{"asthdfgs564 64"});
  testMap.insert(std::string{"asFFGH d"}, std::string{"asthdfgss564 64"});
  testMap.insert(std::string{"bsDFGH d"}, std::string{"asdfgthgs564 64"});
  testMap.insert(std::string{"bsFFGH d"}, std::string{"assdfgthgs564 64"});
  testMap.insert(std::string{"bsjFGH d"}, std::string{"asssdfgthgs564 64"});

  size_t wrongPlacementsCount{};
  auto mapBegin{testMap.begin()};
  std::string previous{mapBegin->first};
  for (auto iter{std::next(testMap.begin())}; iter != testMap.end(); iter++)
  {
    if (iter->first < previous)
      wrongPlacementsCount++;
    previous = iter->first;
  }

  BOOST_CHECK(wrongPlacementsCount == 0);
}

BOOST_AUTO_TEST_CASE(allocator_test_1)
{
  custom_allocator<std::string> allocator{};
  std::string* pointer {allocator.allocate(1)};
  allocator.construct(pointer, "w45yghdf dfgHDFGh \t 46y4\0");
  std::string testString {"w45yghdf dfgHDFGh \t 46y4\0"};

  BOOST_CHECK(*pointer == testString);
  allocator.destroy(pointer);
  allocator.deallocate(pointer, 1);
}

BOOST_AUTO_TEST_SUITE_END()
