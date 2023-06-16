project(GaussDigitization)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

cmake_policy(SET CMP0057 NEW)

add_definitions(-g)

option(WITH_DGTAL "Use DGTAL to output images" OFF)
if (WITH_DGTAL)
    MESSAGE(STATUS "WITH_DGTAL")
    add_definitions(-DWITH_DGTAL)
    #Required in DGtal
    FIND_PACKAGE(DGtal REQUIRED)
    INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
    LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})
    TARGET_LINK_LIBRARIES(GaussDigitization ${DGTAL_LIBRARIES})
endif (WITH_DGTAL)




