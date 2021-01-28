project(GaussDigitization)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

cmake_policy(SET CMP0057 NEW)

#Required in DGtal
FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS})

add_definitions(-g)

TARGET_LINK_LIBRARIES(GaussDigitization ${DGTAL_LIBRARIES})
