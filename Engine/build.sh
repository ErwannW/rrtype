mkdir build-release

cd build-release
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j 32
cp mge ../mge
cd ..
