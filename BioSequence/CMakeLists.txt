cmake_minimum_required (VERSION 2.6)
project (VAT)

find_package(Boost
  1.53.0 REQUIRED
  COMPONENTS program_options timer iostreams thread
  system timer chrono )


set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-uninitialized -Wno-deprecated-declarations")

add_subdirectory("src/blastalgo/blast/core")
include_directories(
"${CMAKE_SOURCE_DIR}/src"
"{Boost_INCLUDE_DIR} "
)

add_executable(VAT src/VATMain.cpp
 src/paras/ConstParameter.cpp
 src/paras/Seed.cpp
 src/paras/Options.cpp
 src/tool/TinyThread.cpp
 src/tool/System.cpp
 src/tool/BinaryFile.cpp
 src/tool/Utils.cpp
 src/tool/BinaryFile.cpp
 )

target_link_libraries(VAT blast_core ${Boost_LIBRARIES} boost_system -lpthread boost_thread boost_program_options boost_timer boost_iostreams)

install(TARGETS VAT DESTINATION bin)
