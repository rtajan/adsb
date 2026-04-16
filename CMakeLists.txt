cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0054 NEW)

project(_cpp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Link with the "Threads library (required to link with AFF3CT after)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(PYBIND11_NEWPYTHON ON)

find_package(pybind11 CONFIG REQUIRED )
message(STATUS "Found pybind11: ${pybind11_VERSION}")

find_package(Threads REQUIRED)
find_package(cpptrace REQUIRED)
find_package(streampu REQUIRED)
message(STATUS "Found streampu: ${streampu_VERSION}")

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/_cpp/*.cpp")

pybind11_add_module(_cpp MODULE ${SOURCES})

set_target_properties(_cpp PROPERTIES
                      OUTPUT_NAME _cpp
                      POSITION_INDEPENDENT_CODE ON) # set -fpic

target_link_libraries     (_cpp PRIVATE spu::spu-static-lib)
target_include_directories(_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/_cpp/include)

option(LINK_UHD "Link with the UHD library (used in the radio USRP)" ON)
if(LINK_UHD)
    #message(STATUS "UHD: on")
    #target_compile_definitions(_cpp PUBLIC
    #                           $<BUILD_INTERFACE:LINK_UHD>
    #                           $<INSTALL_INTERFACE:LINK_UHD>)

    # Link UHD
    find_package (UHD REQUIRED)
    if    (UHD_FOUND)
        message(STATUS "UHD: found")
        include_directories(${UHD_INCLUDE_DIRS})
        target_link_libraries(_cpp PRIVATE ${UHD_LIBRARIES})
    endif (UHD_FOUND)

    # Link Boost
    find_package(Boost QUIET COMPONENTS filesystem thread REQUIRED)
    if (Boost_FOUND)
        message(STATUS "Boost: found")
        target_link_libraries(_cpp PRIVATE ${Boost_FILESYSTEM_LIBRARY}
                                               ${Boost_SYSTEM_LIBRARY}
                                               ${Boost_THREAD_LIBRARY})
    endif (Boost_FOUND)
else()
    message(STATUS "UHD: off")
endif(LINK_UHD)

# MIPP
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/include/mipp.h")
    message(STATUS "Sub-project found: MIPP")
    set(MIPP_TESTS_EXE OFF)
    set(MIPP_EXAMPLES_EXE OFF)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/MIPP/)
    target_link_libraries(_cpp PRIVATE MIPP::mipp)
else()
    message(FATAL_ERROR "MIPP can't be found, try to init the submodule with the following cmd:\n"
                        "$ git submodule update --init --recursive")
endif()

find_package(CURL REQUIRED)
if(CURL_FOUND)
    message(STATUS "CURL: found")
    target_include_directories(_cpp PRIVATE ${CURL_INCLUDE_DIRS})
    target_link_libraries(_cpp PRIVATE ${CURL_LIBRARIES})
endif(CURL_FOUND)

find_package(nlohmann_json REQUIRED)
if(nlohmann_json_FOUND)
    message(STATUS "nlohmann_json: found")
    target_link_libraries(_cpp PRIVATE nlohmann_json::nlohmann_json)
endif(nlohmann_json_FOUND)

