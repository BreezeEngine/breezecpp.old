rm -r build
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER="ccache" -D CMAKE_CXX_COMPILER_ARG1="clang++" -G Ninja ..
