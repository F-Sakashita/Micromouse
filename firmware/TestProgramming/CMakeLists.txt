cmake_minimum_required(VERSION 3.0.0)
project(TestProgramming VERSION 0.1.0)

include(CTest)
enable_testing()

#Include Directories Setting
set(INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/../Source/Utility
)

message(
    "Projec Source Dir : ${PROJECT_SOURCE_DIR}"
)

#Add include files
include_directories("${INCLUDE_DIR}")

set(
    UTILITY_DIR
    ${PROJECT_SOURCE_DIR}/../Source/Utility
)

# ビルド対象としてサブディレクトリを追加
add_subdirectory(
    ${UTILITY_DIR} build
)

add_executable(${PROJECT_NAME} 
    main.cpp
)

# add_subdirectoryにより生成された静的ライブラリをリンクさせる
# ライブラリ名はサブディレクトリのCMakeListsに記載
target_link_libraries(
    ${PROJECT_NAME} 
    utility_lib
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
