set(COURSE_NAME Asynchrony_Back_to_the_future)

file(GLOB SOURCES_LIST
     "*.h"
     "*.cpp"
)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.78.0) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(${COURSE_NAME} ${SOURCES_LIST})
    target_link_libraries(${COURSE_NAME} ${Boost_LIBRARIES})
endif()

target_include_directories(${COURSE_NAME} PRIVATE ${CONAN_INCLUDE_DIRS})
target_link_libraries(${COURSE_NAME} PRIVATE ${CONAN_LIBS})
 