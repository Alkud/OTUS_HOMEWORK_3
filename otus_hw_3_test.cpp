// custom_map.h in Otus homework #3 project

#define BOOST_TEST_MODULE Otus_HW_3_TEST

#include <boost/test/unit_test.hpp>
#include "./headers/custom_allocator.h"
#include "./headers/custom_map.h"
#include <cstdlib>
#include <iostream>


BOOST_AUTO_TEST_SUITE(homework_3_test)

BOOST_AUTO_TEST_CASE(map_test_insert)
{
  custom_map<int, int> testMap{};
  testMap.insert(std::make_pair<int, int>(2, 2));
  testMap.insert(std::make_pair<int, int>(1, 1));
  testMap.insert(std::make_pair<int, int>(3, 3));
  size_t presentNodesCount{};
  for(auto item:testMap)
    presentNodesCount++;
  BOOST_CHECK(presentNodesCount == testMap.count() &&
              presentNodesCount == 3);
}

BOOST_AUTO_TEST_CASE(map_test_sorting)
{
  custom_map<std::string, std::string> testMap{};
  testMap.insert(std::string{"asDFGH d"}, std::string{"asthdfgs564 64"});
  testMap.insert(std::string{"atDFGH d"}, std::string{"asthdfgss564 64"});
  testMap.insert(std::string{"bsDFGH d"}, std::string{"asdfgthgs564 64"});
  testMap.insert(std::string{"bsCFGH d"}, std::string{"assdfgsdhgs564 64"});

  size_t wrongPlacementsCount{};
  std::string previous{(testMap.begin())->first};
  for (auto iter{++testMap.begin()}; iter != testMap.end(); iter++)
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
