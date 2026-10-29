add_executable(mavsentinel /mavsentinel/apps);
target_link_libraries(mavsentinel
                        PUBLIC mavsentinel)
target_compile_options(mavsentinel
     PRIVATE
)
