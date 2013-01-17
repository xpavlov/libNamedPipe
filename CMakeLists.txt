
cmake_minimum_required(VERSION 2.8)
Project(libNamedPipe)
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost REQUIRED COMPONENTS thread )
IF(Boost_FOUND)
 INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
 LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)
SET(TBB_INCLUDE_DIRS $ENV{TBB_INCLUDE_DIRS})
SET(TBB_LIBRARY_DIRS $ENV{TBB_LIBRARY_DIRS})
if(NOT TBB_INCLUDE_DIRS)
        MESSAGE (FATAL_ERROR "TBB_INCLUDE_DIR not set project will not be built.")
else(NOT TBB_INCLUDE_DIRS)
include_directories(${TBB_INCLUDE_DIRS})
ENDIF(NOT TBB_INCLUDE_DIRS)

if(NOT TBB_LIBRARY_DIRS)
        MESSAGE (FATAL_ERROR  "TBB_LIBRARY_DIRS not set project will not be built.")
else(NOT TBB_LIBRARY_DIRS)
link_directories(${TBB_LIBRARY_DIRS})
ENDIF(NOT TBB_LIBRARY_DIRS)

file(GLOB NamedPipe_src PosixNamedPipe.cpp WinNamedPipe.cpp NamedPipeServer.cpp )
file(GLOB NamedPipe_h PosixNamedPipe.h INamedPipe.h NamedPipe.h NamedPipeServer.h WinNamedPipe.h)

add_library(NamedPipe STATIC ${NamedPipe_src} ${NamedPipe_h})
SET_TARGET_PROPERTIES(NamedPipe PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (NamedPipe tbb ${Boost_THREAD_LIBRARY} )

file(GLOB NamedPipeTestClient_src ./examples/NamedPipeTestClient.cpp )
file(GLOB NamedPipeTestClient_h PosixNamedPipe.h INamedPipe.h NamedPipe.h)

add_executable(NamedPipeTestClient ${NamedPipeTestClient_src} ${NamedPipeTestClient_h})
SET_TARGET_PROPERTIES(NamedPipeTestClient PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (NamedPipeTestClient tbb NamedPipe  ${Boost_THREAD_LIBRARY} )

file(GLOB NamedPipeTestServer_src ./examples/NamedPipeTestServer.cpp ./examples/SimpleServer.cpp )
file(GLOB NamedPipeTestServer_h ./examples/SimpleServer.h)

add_executable(NamedPipeTestServer ${NamedPipeTestServer_src} ${NamedPipeTestServer_h})
SET_TARGET_PROPERTIES(NamedPipeTestServer PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (NamedPipeTestServer tbb NamedPipe  ${Boost_THREAD_LIBRARY} )




