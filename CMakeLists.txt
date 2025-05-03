cmake_minimum_required(VERSION 3.20.0)
project(EvSer VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB APP_SRC "bin/*.cpp")
file(GLOB PROJ_SRC "src/EventService/*.cpp" "src/core/*.cpp" "src/log/*.cpp")
file(GLOB TEST_SRC "test/*.cpp")
 
include(CTest)
# Fetch GoogleTest 1.16
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)
# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()
include_directories(${GTEST_INCLUDE_DIRS})
  
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

#----------------------------------------------------------------
# Application
#----------------------------------------------------------------

add_executable(EvSer ./bin/EvServiceApp.cpp ${PROJ_SRC})
target_include_directories(EvSer PRIVATE src)
target_link_libraries(EvSer)


#----------------------------------------------------------------
# Unit Tests
#----------------------------------------------------------------


add_executable( TestEventService ${TEST_SRC} ${PROJ_SRC})
target_include_directories(TestEventService PRIVATE src ${GTEST_INCLUDE_DIRS})
target_link_libraries(
    TestEventService
    PRIVATE
    ${GTEST_LDFLAGS}
    gtest
    gmock
)
 
include(GoogleTest)
gtest_discover_tests(TestEventService)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
