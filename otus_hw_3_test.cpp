// custom_map.h in Otus homework #3 project

#define BOOST_TEST_MODULE Otus_HW_3_TEST

#include <boost/test/unit_test.hpp>
#include "./headers/custom_allocator.h"
#include "./headers/custom_map.h"
#include <cstdlib>
#include <iostream>
#include <map>

uint factorial(uint n)
{
  uint result{1};
  for (uint i{2}; i <= n; i++)
    result *= i;
  return result;
}

template <typename Key, typename Value>
using CustomAllocatedMap =
std::map<
          Key, Value, std::less<Key>,
          CustomAllocator<std::pair<const Key, Value>, 20>
        >;

template <typename Key, typename Value>
using CustomAllocatedCustomMap =
CustomMap<
          Key, Value, std::less<Key>,
          CustomAllocator<Node<std::pair<const Key, Value>>, 20>
         >;

BOOST_AUTO_TEST_SUITE(IMPLEMENTATION_TEST)

BOOST_AUTO_TEST_CASE(map_test_insert)
{
  CustomMap<int, int> map3{};
  for (int idx{}; idx < 10; idx++)
    map3.insert(std::pair<const int, int>{idx, factorial(idx)});

  CustomMap<int, int> testMap{};
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
  CustomMap<std::string, std::string>testMap{};
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
  CustomAllocator<std::string> allocator{};
  std::string* pointer {allocator.allocate(1)};
  allocator.construct(pointer, "w45yghdf dfgHDFGh \t 46y4\0");
  std::string testString {"w45yghdf dfgHDFGh \t 46y4\0"};

  BOOST_CHECK(*pointer == testString);
  allocator.destroy(pointer);
  allocator.deallocate(pointer, 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(FRAGMENTATION_TEST)

BOOST_AUTO_TEST_CASE(fragmentation_test)
{
  std::vector<std::shared_ptr<int>> dummyVector1{};
  std::map<int, int> map1{};
  for (int idx{}; idx < 20; idx++)
  {
    map1.insert(std::pair<const int, int>{idx, idx});
    for (int i{}; i < idx; i++)
      dummyVector1.push_back(std::make_shared<int>(idx));
  }

  size_t fragmentationsCount1{} ;
  auto previous1 {map1.begin()};
  auto current1 {std::next(previous1)};
  int distance{std::addressof(*current1) - std::addressof(*previous1)};
  for (; current1 != map1.end(); current1++)
  {
    if ( (std::addressof(*current1) - std::addressof(*previous1)) != distance )
        fragmentationsCount1++;
    distance = std::addressof(*current1) - std::addressof(*previous1);
    previous1 = current1;
  }
  std::cout << "map1 fragmentations: " << fragmentationsCount1 << std::endl;

  CustomAllocatedMap<int, int> map2{};
  std::vector<std::shared_ptr<int>> dummyVector2{};
  for (uint16_t idx{}; idx < 20; idx++)
  {
    for (int i{}; i < idx; i++)
      dummyVector2.push_back(std::make_shared<int>(idx));
    map2.insert(std::pair<const int, int>{idx, idx});
  }

  size_t fragmentationsCount2{} ;
  auto previous2 {map2.begin()};
  auto current2 {std::next(previous2)};
  distance = std::addressof(*current2) - std::addressof(*previous2);
  for (; current2 != map2.end(); current2++)
  {
    if ( (std::addressof(*current2) - std::addressof(*previous2)) != distance )
        fragmentationsCount2++;
    distance = std::addressof(*current2) - std::addressof(*previous2);
    previous2 = current2;
  }
  std::cout << "map2 fragmentations: " << fragmentationsCount2 << std::endl;

  CustomMap<int, int> map3{};
  std::vector<std::shared_ptr<int>> dummyVector3{};
  for (uint16_t idx{}; idx < 20; idx++)
  {
    for (int i{}; i < idx; i++)
      dummyVector3.push_back(std::make_shared<int>(idx));
    map3.insert(std::pair<const int, int>{idx, idx});
  }

  size_t fragmentationsCount3{} ;
  auto previous3 {map3.begin()};
  auto current3 {std::next(previous3)};
  distance = std::addressof(*current3) - std::addressof(*previous3);
  for (; current3 != map3.end(); current3++)
  {
    if ( (std::addressof(*current3) - std::addressof(*previous3)) != distance )
        fragmentationsCount3++;
    distance = std::addressof(*current3) - std::addressof(*previous3);
    previous3 = current3;
  }
  std::cout << "map3 fragmentations: " << fragmentationsCount3 << std::endl;

  CustomAllocatedCustomMap<int, int> map4{};
  std::vector<std::shared_ptr<int>> dummyVector4{};
  for (uint16_t idx{}; idx < 20; idx++)
  {
    for (int i{}; i < idx; i++)
      dummyVector4.push_back(std::make_shared<int>(idx));
    map4.insert(std::pair<const int, int>{idx, idx});
  }

  size_t fragmentationsCount4{} ;
  auto previous4 {map4.begin()};
  auto current4 {std::next(previous4)};
  distance = std::addressof(*current4) - std::addressof(*previous4);
  for (; current4 != map4.end(); current4++)
  {
    if ( (std::addressof(*current4) - std::addressof(*previous4)) != distance )
        fragmentationsCount4++;
    distance = std::addressof(*current4) - std::addressof(*previous4);
    previous4 = current4;
  }
  std::cout << "map4 fragmentations: " << fragmentationsCount4 << std::endl;

  BOOST_CHECK(fragmentationsCount1 >  0 &&
              fragmentationsCount2 == 0 &&
              fragmentationsCount3 >  0 &&
              fragmentationsCount4 == 0    );
}

BOOST_AUTO_TEST_SUITE_END()
