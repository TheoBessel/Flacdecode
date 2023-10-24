cmake_minimum_required(VERSION 3.1...3.27)

set(project_name "FlacDecode")

set(CMAKE_CXX_STANDARD 17)
project(${project_name})

add_executable(
    ${project_name}
    decodeflac.cpp
    BitInput/bitinput.cpp
    Bloc/bloc.cpp
    Bloc/Metadata/metadata.cpp
    Bloc/Vorbis/vorbis.cpp
    Bloc/Image/image.cpp
    Bloc/Padding/padding.cpp
    Frame/frame.cpp
)

target_compile_features(${project_name} PUBLIC cxx_std_17)
set_target_properties(${project_name} PROPERTIES CXX_EXTENSIONS OFF)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")