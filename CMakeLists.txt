cmake_minimum_required(VERSION 3.24)
project(2526_TI301_PJT C)

set(CMAKE_C_STANDARD 11)

add_executable(TI_301_PJT
        main.c
        graph.h
        markov_check.h
        mermaid_gen.h
        graph.c
        markov_check.c
        mermaid_gen.c)

