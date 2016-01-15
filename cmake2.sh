rm -r build
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER="clang++" -G Ninja ..
