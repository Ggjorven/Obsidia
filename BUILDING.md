## Installation

### Windows

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Obsidia.git
    cd Obsidia
    ```

2. Navigate to the scripts folder:
    ```sh
    cd Scripts/Windows
    ```

3. (Optional) If you haven't already installed the premake5 build system you can install it like this:
    ```sh
    ./install_premake5.bat
    ```

4. Choose what you want it build to:
    - Visual Studio 17 2022:
        ```sh
        ./generate_vs2022.bat
        ```
    - MinGW make files:
        ```sh
        ./generate_make_%compiler%.bat
        ```

### Linux

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Obsidia.git
    cd Obsidia
    ```

2. Navigate to the scripts folder:
    ```sh
    cd Scripts/Linux
    ```

3. (Optional) If you haven't already installed the premake5 build system you can install it like this:
    ```sh
    chmod +x install_premake5.sh
    ./install_premake5.sh
    ```

4. Generate make files:
    ```sh
    chmod +x generate_make_%compiler%.sh
    ./generate_make_%compiler%.sh
    ```

### MacOS

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Obsidia.git
    cd Obsidia
    ```

2. Navigate to the scripts folder:
    ```sh
    cd Scripts/MacOS
    ```

3. (Optional) If you haven't already installed the premake5 build system you can install it like this:
    ```sh
    chmod +x install_premake5.sh
    ./install_premake5.sh
    ```

4. Generate make files:
    ```sh
    chmod +x generate_xcode.sh
    ./generate_xcode.sh
    ```

## Building

### Windows
- Visual Studio 17 2022:
    1. Navigate to the root of the directory
    2. Open the Obsidia.sln file
    3. Start building in your desired configuration
    4. Build files can be in the Binaries/%Config%-windows/Sandbox/ folder.
    5. (Optional) Open a terminal and run the Sandbox project:
        ```sh
        ./Sandbox.exe 
        ```

- MinGW Make:
    1. Navigate to the root of the directory
    2. Open a terminal.
    3. Call make with desired configuration (Debug, Release or Distribution):
        ```sh
        make config=Release
        ```

    5. Build files can be in the Binaries/%Config%-linux/Sandbox/ folder.
    6. (Optional) Open a terminal and run the Sandbox project:
        ```sh
        ./Sandbox.exe 
        ```

### Linux

1. Navigate to the root of the directory
2. Open a terminal
3. Call make with desired configuration (Debug, Release or Distribution):

    ```sh
    make config=release
    ```

5. Build files can be in the Binaries/%Config%-linux/Sandbox/ folder.
6. (Optional) Open a terminal and run the Sandbox project:

    ```sh
    chmod +x Sandbox
    ./Sandbox
    ```

### MacOS
1. Navigate to the root of the directory
2. Open the Obsidia.xcworkspace file
3. Start building in your desired configuration
4. Build files can be in the Binaries/%Config%-macosx/Sandbox/ folder.
5. (Optional) Open a terminal and run the Sandbox project:

    ```sh
    ./Sandbox
    ```