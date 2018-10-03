cmake_minimum_required(VERSION 3.12)
project(DesignPattern)

set(CMAKE_CXX_STANDARD 14)

add_executable(DesignPattern main.cpp SOLID/InterfaceSegregationPrinciple.cpp SOLID/LiskovSubstitutionPrinciple.cpp SOLID/OpenClosedPrinciple.cpp SOLID/SingleResponsibilityPrinciple.cpp SOLID/DependencyInversionPrinciple.cpp Builder/builder.cpp Builder/builderExercise.cpp FactoryMethod.cpp)


set(BOOST_ROOT "/usr/local/Cellar/boost/1.65.1")

find_package(Boost 1.65.1 COMPONENTS system filesystem REQUIRED)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_VERSION: ${Boost_VERSION}")