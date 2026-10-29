add_library(mavsentinel_core
    STATIC
        version.cpp
)

target_include_directories(mavsentinel_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(mavsentinel_core
    PUBLIC
        cxx_std_23
)
target_compile_options(mavsentinel_core
     PRIVATE
        -Wall -Wextra -Wpedantic -Werror
)
