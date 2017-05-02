cmake_minimum_required (VERSION 2.8)
project (libctp)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

set(LIB_LIST)

# ctp .so
find_path(CTP_INCLUDE_DIRS NAMES HINTS "./ctp_future_api/include")
find_library(CTP_MDUSER_LIBRARIES NAMES thostmduserapi HINTS "./ctp_future_api/lib")
find_library(CTP_TRADER_LIBRARIES NAMES thosttraderapi HINTS "./ctp_future_api/lib")
if (CTP_INCLUDE_DIRS AND CTP_MDUSER_LIBRARIES AND CTP_TRADER_LIBRARIES)
    include_directories("${CTP_INCLUDE_DIRS}")
    list(APPEND LIB_LIST ${CTP_MDUSER_LIBRARIES})
    list(APPEND LIB_LIST ${CTP_TRADER_LIBRARIES})
endif()

# application
INCLUDE_DIRECTORIES(ctp_future_api/include)
file(GLOB_RECURSE DIR_SRCS "ctp_future_api/src/capi.cc")

# library
ADD_LIBRARY(ctp STATIC ${DIR_SRCS})
TARGET_LINK_LIBRARIES(ctp ${LIB_LIST})
