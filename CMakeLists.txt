cmake_minimum_required(VERSION 3.10)
project(otus5 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_library(matrix INTERFACE)
target_sources(matrix INTERFACE ${CMAKE_SOURCE_DIR}/matrix.h)

add_executable(otus5 main.cpp)
target_link_libraries(otus5 matrix)

add_executable(test_matrix test_matrix)
set_target_properties(test_matrix PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)
target_link_libraries(test_matrix matrix ${Boost_LIBRARIES})

install(TARGETS otus5 RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEB_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${otus5_VERSION})
set(CPACK_PACKAGE_CONTACT buevichd@yandex-team.ru)
include(CPack)

