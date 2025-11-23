cmake_minimum_required(VERSION 3.10)

project(MarkovAnalyzer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(SOURCE_FILES
        main.c
        graph.c
        markov_check.c
        mermaid_gen.c
        tarjan.c
        hasse.c
        characteristic.c
        matrix.c
        period.c
)

add_executable(markov_analyzer ${SOURCE_FILES})

target_link_libraries(markov_analyzer m)