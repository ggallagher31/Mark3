project(ram_file)

set(LIB_SOURCES
    drvRAM.cpp
)

set(LIB_HEADERS
    public/drvRAM.h
)

mark3_add_library(ram_file ${LIB_SOURCES} ${LIB_HEADERS})

target_include_directories(ram_file
    PUBLIC
        public        
    )
    
target_link_libraries(ram_file
    mark3
)