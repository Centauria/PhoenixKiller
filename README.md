# PhoenixKiller

Find out hidden mining virus on the computer.

## Requirements

```
cmake >= 3.20
gcc, g++ >= 9
libboost-dev
```

## Installation

Before you install, you should check your version of cmake and gcc.
Otherwise, you should install it by yourself.

If your ```cmake --version``` is lower than 3.20, then you can install it by typing in this command.

```shell
sudo snap install cmake --classic
```

If you don't have ```snap``` installed, you may have to build ```cmake``` from the source.

```shell
wget https://github.com/Kitware/CMake/releases/download/v3.20.2/cmake-3.20.2.tar.gz
tar -xvf cmake-3.20.2.tar.gz
cd cmake-3.20.2
./bootstrap --parallel=8
make -j8
sudo make install
```

Also, if you don't have gcc/g++ version 9 or higher, you can install the newest one.

```shell
sudo apt install software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt install gcc-11 g++-11 libboost-dev
```

Then you can get the repository and compile it.

```shell
git clone https://github.com/Centauria/PhoenixKiller.git
cd PhoenixKiller
git submodule update --init --recursive
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$(which gcc-11) -DCMAKE_CXX_COMPILER=$(which g++-11) ..
make -j8
sudo make install
```

There are 2 executives: ```PhoenixKiller``` and ```gpupid```.

When running ```sudo make install```, a systemd service and a timer will be installed on the machine and will start
automatically.

## Usage

### PhoenixKiller

```
Optional arguments:
-h --help        	shows help message and exits
-v --version     	prints version information and exits
--with-md5-check 	Use md5 check when searching.
More accuracy, but slower [default: false]
--cleanup        	Cleanup mining programs automatically. [default: false]
```

### gpupid

Run directly. Shows current processes using GPUs.

## Q&A

1. Could NOT find NVML (missing: NVML_INCLUDE_DIRS)

```shell
CMake Error at /usr/share/cmake-3.10/Modules/FindPackageHandleStandardArgs.cmake:137 (message):
  Could NOT find NVML (missing: NVML_INCLUDE_DIRS)
```

It can be solved by adding this option on cmake:

```shell
cmake -DNVML_RETRIEVE_HEADER_ONLINE=true ..
```

2. fatal error: charconv: No such file or directory

```shell
[ 44%] Building CXX object src/CMakeFiles/PhoenixKiller.dir/main.cpp.o
In file included from /home/centauria/Centauria1/tools/PhoenixKiller/src/main.cpp:1:0:
/home/centauria/Centauria1/tools/PhoenixKiller/3rd-party/argparse/include/argparse/argparse.hpp:35:10: fatal error: charconv: No such file or directory
 #include <charconv>
          ^~~~~~~~~~
compilation terminated.
```

It's because your gcc/g++ version is lower than 8.1.
Update to gcc/g++ 9/10/11.

3. undefined reference to filesystem::

```shell
CMakeFiles/PhoenixKiller.dir/main.cpp.o: In function `main':
main.cpp:(.text+0xbf1): undefined reference to `std::filesystem::__cxx11::directory_iterator::operator*() const'
main.cpp:(.text+0xc7b): undefined reference to `std::filesystem::__cxx11::directory_iterator::operator++()'
...
```

It's also because your gcc/g++ version is too low.