# Root Cmake. Use to link the different libraries
#
# @version 1.0


#########################################################################################################
###                                       GENERAL CONFIG                                               ##
#########################################################################################################

message("%%% SYNTHESIS PROJECT : ORB TESTS")
message("\n%% GENERAL CONFIGURATION")

# GENERAL SETTINGS
project(OrbTest CXX)
cmake_minimum_required(VERSION 3.2)
set(VERSION 1.0.0)

set(APP_NAME OrbTest)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# OUTPUT DIR SETTINGS
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# COMPILER OPTIONS
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG   " -g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE " -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Warning : option are important for some lib


########################################################################################################
###                                       LIBRARY CONFIG                                              ##
########################################################################################################

message("\n%% EXTERNALS libs configuration")

message("- Checking : pkg")
find_package(PkgConfig REQUIRED)
if(PkgConfig_FOUND)
    message("-> succes " )
else()
    message("Fail : You must hae PkgConfig installed")
endif()

message("- Checking : OpenCV")
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

message("- Checking : Thread")
find_package(Threads)

######################################################################################################
###                                            LINKS                                                ##
######################################################################################################

link_libraries(
    ${CMAKE_THREAD_LIBS_INIT}
    dl
    config++
    panel
)

add_subdirectory(lib)
include_directories(lib)
include_directories(src)

######################################################################################################
###                                         APPLICATION                                             ##
######################################################################################################

message("\n%% APPLICATION GENERATION")

file(GLOB_RECURSE app_SRC 
    "lib/*.h"
    "src/*.h"
    "src/*.cpp"
)

# %% App files
add_executable(${APP_NAME} ${app_SRC})

target_link_libraries(${APP_NAME} PUBLIC
    ${OpenCV_LIBS}    
    loguru
)