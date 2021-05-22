


if not exist "build" mkdir build

"C:/Program Files/CMake/bin/cmake.exe" -S . -B build -G "MinGW Makefiles" -DCMAKE_C_COMPILER="arm-none-eabi-gcc" -DCMAKE_CXX_COMPILER="arm-none-eabi-g++" -DCMAKE_AR="arm-none-eabi-ar" -DCMAKE_LINKER="arm-none-eabi-ld" -DCMAKE_SYSTEM_NAME=Generic -DCMAKE_SYSTEM_PROCESSOR=arm -DCMAKE_C_FLAGS="--specs=nosys.specs"