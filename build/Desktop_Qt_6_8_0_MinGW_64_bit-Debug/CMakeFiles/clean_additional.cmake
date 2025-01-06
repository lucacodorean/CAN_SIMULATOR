# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CAN_SIMULATOR_autogen"
  "CMakeFiles\\CAN_SIMULATOR_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CAN_SIMULATOR_autogen.dir\\ParseCache.txt"
  )
endif()
