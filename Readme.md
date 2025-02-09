# Babel Project

## Overview

Babel is a cross-platform project that requires specific dependencies and configurations to build and run successfully. This README provides installation and execution instructions for Linux, macOS, and Windows.

## Prerequisites

- **Conan** (Package Manager for C++)
- **CMake** (Cross-platform build system)
- **GCC/MinGW** (C++ Compiler)
- **Python** (Required for Conan on Windows)

---

## Installation & Build Instructions

### Linux & macOS

1. **Setup Conan recipes:**

   ```bash
   mkdir conan-recipes
   cd conan-recipes
   git clone https://github.com/dimi309/portaudio-conan
   cd portaudio-conan
   conan export . --version=19.7.0
   cd ../..
   ```

2. **Install dependencies and build:**

   ```bash
   conan install . --output-folder=build --build=missing
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
   make
   ```

---

### Windows

#### Install MinGW (Windows)

1. Download the MinGW installer from [this link](https://nuwen.net/files/mingw/mingw-19.0.exe).
2. Extract it to `C:\`.
3. Open PowerShell and run the following command to add MinGW to your PATH:
   ```powershell
   [Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\MinGW\bin", [EnvironmentVariableTarget]::User)
   ```
4. Close and reopen PowerShell.
5. Verify the installation:
   ```bash
   g++ --version
   ```

#### Install Conan & Configure

6. Install Python and add it to the system PATH.
7. Install Conan:
   ```bash
   pip install conan
   ```
8. Modify Conan's default profile:
   - Open a PowerShell terminal.
   - Navigate to Conan profiles directory:
     ```powershell
     cd .conan2/profiles
     ```
   - Modify the default profile (`default`) and replace it with:
     ```
     [settings]
     os=Windows
     arch=x86_64
     compiler=gcc
     compiler.version=10
     compiler.libcxx=libstdc++11
     build_type=Release
     ```

#### Build the Project on Windows

9. Clone the required repository:
   ```bash
   git clone https://github.com/dimi309/portaudio-conan
   cd portaudio-conan
   conan export . --version=19.7.0
   cd ../
   ```
10. Install dependencies and build:
    ```bash
    conan install . --output-folder=build --build=missing
    cd build
    cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" ..
    make
    ```

---

## Running the Application

### Start the Babel Server

```bash
./babel_server port
```

### Start the Babel Client

```bash
./babel_client ip port
```

Replace `port` with the desired port number and `ip` with the target server IP address.

