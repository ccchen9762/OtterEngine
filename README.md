# OtterEngine

* This project using assimp + directxtk, make sure you install before compiling
    * a simple way to do
    ```
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ./vcpkg install assimp:x64-windows
    ./vcpkg install directxtk:x64-windows
    ```