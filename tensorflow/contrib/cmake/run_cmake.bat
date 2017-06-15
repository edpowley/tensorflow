rmdir /s build
mkdir build
cd build
cmake .. -A x64 -DCMAKE_BUILD_TYPE=Release -Dtensorflow_BUILD_PYTHON_BINDINGS=OFF -Dtensorflow_ENABLE_GRPC_SUPPORT=OFF
cd ..
