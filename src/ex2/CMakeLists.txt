file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp)

target_sources(ex2 PRIVATE ${SOURCE_FILES})
target_include_directories(ex2 PRIVATE ../include)
