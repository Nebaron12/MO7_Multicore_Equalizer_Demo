# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "D:\\Fontys\\MO7\\SID_Multicore_Demo\\platform\\ps7_cortexa9_1\\standalone_ps7_cortexa9_1\\bsp\\include\\sleep.h"
  "D:\\Fontys\\MO7\\SID_Multicore_Demo\\platform\\ps7_cortexa9_1\\standalone_ps7_cortexa9_1\\bsp\\include\\xiltimer.h"
  "D:\\Fontys\\MO7\\SID_Multicore_Demo\\platform\\ps7_cortexa9_1\\standalone_ps7_cortexa9_1\\bsp\\include\\xtimer_config.h"
  "D:\\Fontys\\MO7\\SID_Multicore_Demo\\platform\\ps7_cortexa9_1\\standalone_ps7_cortexa9_1\\bsp\\lib\\libxiltimer.a"
  )
endif()
