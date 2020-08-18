cmake_minimum_required(VERSION 3.8)

project(mycraft)


#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)



set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "H:/programs/QT/5.14.2/mingw73_32/lib/cmake")


#Attach QT5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
include_directories(mycraft ${Qt5Widgets_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_definitions(${Qt5Widgets_DEFINITIONS})






add_executable(mycraft
    main.cpp)



target_link_libraries(mycraft ${Qt5Widgets_LIBRARIES})
