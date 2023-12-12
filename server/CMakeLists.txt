cmake_minimum_required(VERSION 3.0.0)
project(server VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(server main.cpp models.cpp handler.cpp controllers.cpp router.cpp states.cpp)

find_package(PkgConfig)
pkg_check_modules(MySQL REQUIRED mysqlclient>=5.7)
target_include_directories(${PROJECT_NAME} PUBLIC ${MySQL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${MySQL_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
