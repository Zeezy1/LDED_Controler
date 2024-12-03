# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\DED_Controller_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DED_Controller_autogen.dir\\ParseCache.txt"
  "DED_Controller_autogen"
  )
endif()
