set(COURSE_NAME Williams_Concurrency_in_action)

set (SOURCES_LIST
    main.cpp
	2_4.cpp
    3_3_1.cpp
)

add_executable(${COURSE_NAME} ${SOURCES_LIST})

target_include_directories(${COURSE_NAME} PRIVATE ${CONAN_INCLUDE_DIRS})
target_link_libraries(${COURSE_NAME} PRIVATE ${CONAN_LIBS})
 