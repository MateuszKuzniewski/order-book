# order-book
C++ Order Book implementation

Currently under development

## Dependencies

* Ninja
* CMake 4.2.3
* Clang

#### Arch Linux
```shell
sudo pacman -S cmake clang ninja
```
#### Debian / Ubuntu
```shell
sudo apt install cmake ninja-build clang
```

#### Windows
Install ninja with Winget
```shell
winget install Ninja-build.Ninja
```



## How to build
## - Linux

### Clone the project
```shell
git clone https://github.com/MateuszKuzniewski/order-book.git
```

### Build with CMake
```shell
cmake -S . -B build -G Ninja
cmake --build build
```

### Run 
```shell
./build/my_app
```

## - Windows
### Clone the project
```shell
git clone https://github.com/MateuszKuzniewski/order-book.git
```

### Build with CMake
```shell
cmake -B build -G Ninja
cmake --build build
```
### Run
```shell
build\my_app.exe
```

