cmake_minimum_required(VERSION 3.14)
project(MercuryProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

find_package(Eigen3)
if(NOT Eigen3_FOUND)
  message(STATUS "Eigen3 not found; using github version")
  FetchContent_Declare(
    Eigen3::Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
  )
  FetchContent_MakeAvailable(Eigen3::Eigen)
else()
  message(STATUS "Using system Eigen3")
endif()

find_package(OpenMP)

find_package(ROOT COMPONENTS Minuit2)
if(NOT TARGET ROOT::Minuit2)
  message(STATUS "Minuit2 not found; using github version")
  FetchContent_Declare(
    Minuit2
    GIT_REPOSITORY https://github.com/GooFit/Minuit2
  )
  FetchContent_MakeAvailable(Minuit2)
  target_compile_definitions(Minuit2Math PUBLIC MATHCORE_STANDALONE)
  set(Minuit_TARGET "Minuit2::Minuit2")

else()
  message(STATUS "Using system ROOT::Minuit2")
  set(Minuit_TARGET "ROOT::Minuit2")
endif()

file(GLOB_RECURSE MERCURY_CPP_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE MERCURY_CPP_EXT_SOURCES CONFIGURE_DEPENDS ext/*.cpp)
set(MERCURY_SOURCES ${MERCURY_CPP_SOURCES} ${MERCURY_CPP_EXT_SOURCES})

add_library(Mercury STATIC ${MERCURY_SOURCES})
target_include_directories(Mercury
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/mercury"
    "${CMAKE_CURRENT_SOURCE_DIR}/ext"
#    "${json11_SOURCE_DIR}"
    $<TARGET_PROPERTY:Eigen3::Eigen,INTERFACE_INCLUDE_DIRECTORIES>  # if eigen target available
    "${Minuit2_SOURCE_DIR}/inc"
    "${Minuit2_SOURCE_DIR}/../mathcore/inc"
    "${Minuit2_SOURCE_DIR}/../../core/foundation/inc"
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(Mercury PRIVATE OpenMP::OpenMP_CXX)
endif()
target_link_libraries(Mercury PUBLIC Eigen3::Eigen ${Minuit_TARGET})
set_target_properties(Mercury PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(testMercury tests/test.cpp)
target_link_libraries(testMercury PRIVATE Mercury)
target_include_directories(testMercury PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(testMercuryXY tests/testxy.cpp)
target_link_libraries(testMercuryXY PRIVATE Mercury)
target_include_directories(testMercuryXY PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
