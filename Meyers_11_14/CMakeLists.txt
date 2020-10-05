set(COURSE_NAME meyers_11_14)

set (SOURCES_LIST
    main.cpp
#    5_5_pass_by_value.cpp
#    5_5_tag_dispatch.cpp
#    5_5_constanting_templates.cpp
)

add_executable(${COURSE_NAME} ${SOURCES_LIST})

target_include_directories(${COURSE_NAME} PRIVATE ${CONAN_INCLUDE_DIRS})
target_link_libraries(${COURSE_NAME} PRIVATE ${CONAN_LIBS})
 