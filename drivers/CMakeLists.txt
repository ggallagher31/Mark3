add_subdirectory(common)
if("${mark3_arch}" STREQUAL "avr")
    add_subdirectory(cpu/${mark3_arch}/${mark3_variant}/${mark3_toolchain})
endif("${mark3_arch}" STREQUAL "avr")    

