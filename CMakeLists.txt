cmake_minimum_required (VERSION 3.2.0)

project (twitter-sentiment-analysis)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

# Compiler options
set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall -Wextra -Wpedantic -Wno-unused-parameter")

#------------------------------------------------------------
# Boost
#

option (Boost_USE_STATIC_LIBS "Use static libraries for boost" ON)

set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)

# use cmake flags instead.
#find_package (Boost REQUIRED COMPONENTS
#    system
#    thread    
#    )

include_directories (SYSTEM ${Boost_INCLUDE_DIRS})
link_libraries (${Boost_LIBRARIES})

#------------------------------------------------------------
# oauth 
#
find_library(OAUTH_LIBRARY 
    NAMES liboauth.so
    HINTS /usr/lib
)

include_directories(${OAUTH_INCLUDE_DIR})
link_libraries(${OAUTH_LIBRARY})

#------------------------------------------------------------
# curl
#
find_library(CURL_LIBRARY
    NAMES curl libcurl.so
    HINTS /usr/lib
)

include_directories(${CURL_INCLUDE_DIR})
link_libraries(${CURL_LIBRARY})


#------------------------------------------------------------
# rt Realtime Extensions library
#
find_library(RT_LIBRARY
    NAMES librt.so
    HINTS /usr/lib
)

#------------------------------------------------------------
# include other dir
#
#include_directories(include)
include_directories(thirdparty)
#------------------------------------------------------------
# executable
#

set(HEADER_FILES 
    include/ExceptionHandler.hpp
    include/Oauth.hpp
    include/ConnectionHandler.hpp
    include/TwitterHandler.hpp
    include/AnalysisHandler.hpp
    include/utility.hpp
)

add_executable (main  
    ${OAUTH_INCLUDE_DIR}         # this is optional
    ${CURL_INCLUDE_DIR}
    ${HEADER_FILES}
    main.cpp
)

target_link_libraries (main
    ${OAUTH_LIBRARY}   
    ${CURL_LIBRARY}
    ${RT_LIBRARY} 
    Threads::Threads
)
