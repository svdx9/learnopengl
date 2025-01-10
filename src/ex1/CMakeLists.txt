file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp)

target_sources(ex1 PRIVATE ${SOURCE_FILES})
target_include_directories(ex1 PRIVATE ../include)
