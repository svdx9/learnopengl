file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp ../include/*.cpp)

target_sources(ex5 PRIVATE ${SOURCE_FILES})
target_include_directories(ex5 PRIVATE ../include)
