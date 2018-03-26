#CmakeList.txt for OTUS Homework #2

#Check CMake version
cmake_minimum_required(VERSION 3.2)

#Set project name, set PROJECT_VERSION_MINOR value equal to TRAVIS_BUILD NUMBER
project(allocator VERSION 0.0.1)

set (BOOST_MIN_VERSION "1.54.0")

#Find Boost UTF component, get BOOST_LIBRARIES and BOOST_INCLUDE_DIR values
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

#Set library target
add_library(custom_allocator INTERFACE)
target_sources(custom_allocator INTERFACE ${PROJECT_SOURCE_DIR}/headers/custom_allocator.h)
target_include_directories(custom_allocator INTERFACE ${PROJECT_SOURCE_DIR}/headers)
add_library(custom_map INTERFACE)
target_sources(custom_map INTERFACE ${PROJECT_SOURCE_DIR}/headers/custom_map.h)
target_include_directories(custom_map INTERFACE ${PROJECT_SOURCE_DIR}/headers)
add_library(memory_manager INTERFACE)
target_sources(memory_manager INTERFACE ${PROJECT_SOURCE_DIR}/headers/memory_manager.h)
target_include_directories(memory_manager INTERFACE ${PROJECT_SOURCE_DIR}/headers)


#Set executable target
set(HEADERS ${PROJECT_SOURCE_DIR}/headers/custom_allocator.h ${PROJECT_SOURCE_DIR}/headers/custom_map.h ${PROJECT_SOURCE_DIR}/headers/memory_manager.h)
add_executable(allocator allocator.cpp ${HEADERS})

#Set test target_link_libraries
add_executable(otus_hw_3_test otus_hw_3_test.cpp)

#Enable tests
enable_testing()
add_test(
NAME test_hw_3
COMMAND otus_hw_3_test)

#Set properties and compile options for executable target
set_target_properties(allocator
  PROPERTIES CXX_STANDARD 14
             CXX_STANDARD_REQUIRED ON
             COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

#Set properties for test target
set_target_properties(otus_hw_3_test PROPERTIES
  COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
  INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

#Link
target_link_libraries(custom_allocator INTERFACE memory_manager)
target_link_libraries(allocator custom_allocator custom_map)
target_link_libraries(otus_hw_3_test
  ${Boost_LIBRARIES}
  custom_allocator
)

install(TARGETS allocator RUNTIME DESTINATION bin)

message("${PROJECT_NAME} build ${PROJECT_VERSION}")

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT alexandr_kudinov@mail.ru)

include(CPack)
