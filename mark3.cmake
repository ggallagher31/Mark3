set(mark3_arch      "avr"             CACHE STRING "Target Archictecture (i.e. avr, cm0, cm3, cm4f, msp430)")
set(mark3_variant   "atmega328p"      CACHE STRING "Target Archictecture (i.e. avr, cm0, cm3, cm4f, msp430)")
set(mark3_toolchain "gcc"             CACHE STRING "Target Toolchain name (i.e. gcc)")
set(mark3_root_dir  "$ENV{ROOT_DIR}"  CACHE STRING "Target Toolchain name (i.e. gcc)")

message(STATUS "-----------------------------------------------")
message(STATUS "==[MARK3 CMake Build System]===================")
message(STATUS "-----------------------------------------------")
message(STATUS " CPU Architecture  = ${mark3_arch}")
message(STATUS " CPU Variant       = ${mark3_variant}")
message(STATUS " Toolchain         = ${mark3_toolchain}")
message(STATUS "-----------------------------------------------")

include("${mark3_root_dir}/build/arch/${mark3_arch}/${mark3_variant}/${mark3_toolchain}/platform.cmake")

get_property(mark3_cc GLOBAL PROPERTY global_cc)
get_property(mark3_cxx GLOBAL PROPERTY global_cxx)
get_property(mark3_objcopy GLOBAL PROPERTY global_objcopy)
get_property(mark3_objdump GLOBAL PROPERTY global_objdump)
get_property(mark3_size GLOBAL PROPERTY global_size)
get_property(mark3_cc_flags GLOBAL PROPERTY global_cc_flags)
get_property(mark3_cxx_flags GLOBAL PROPERTY global_cxx_flags)
get_property(mark3_ln_flags GLOBAL PROPERTY global_ln_flags)
get_property(mark3_ln_dbg_flags GLOBAL PROPERTY global_ln_dbg_flags)
get_property(mark3_objcopy_flags GLOBAL PROPERTY global_objcopy_flags)
get_property(mark3_objcopy_dbg_flags GLOBAL PROPERTY global_objcopy_dbg_flags)
get_property(mark3_base_libs GLOBAL PROPERTY global_base_libs)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ${mark3_arch})
set(CMAKE_C_COMPILER ${mark3_cc})
set(CMAKE_CXX_COMPILER ${mark3_cxx})
set(CMAKE_CC_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

