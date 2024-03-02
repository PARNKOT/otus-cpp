cmake_minimum_required(VERSION 3.10)

if(DEFINED ENV{GITHUB_RUN_NUMBER})
    project(01_hometask VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})
else()
    project(01_hometask VERSION 0.0.0)
endif()

