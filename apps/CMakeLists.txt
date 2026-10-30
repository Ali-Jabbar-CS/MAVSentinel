add_executable(mavsentinel
    main.cpp
)

target_link_libraries(mavsentinel
    PRIVATE mavsentinel_core
)

target_compile_options(mavsentinel
    PRIVATE -Wall -Wextra -Wpedantic -Werror
)
