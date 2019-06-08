# Owaria

## Configuring build environment
#### Visual Studio 2017
- Install Visual Studio 2017
- Run vcvars32.bat or vsvars32.bat. It should be in C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\

#### Cmake
- Install [cmake](https://cmake.org/download/)(minimum version 3.7).

#### Vcpkg
- Clone [vcpkg](https://github.com/microsoft/vcpkg) to "C:/" directory. Install it by command ".\bootstrap-vcpkg.bat".
- Install required libraries box2d, sfml, tinyxml2 and tgui by using command "vcpkg.exe install <library_name>".
- Add a new environment Variable. Environment Variables -> New System Variable -> Name: "VCKG_PATH" Value: <your_vcpkg_directory>

#### Owaria
- Clone Owaria, browse into the directory, Right Click->Click Open In Visual Studio.

![alt text](https://i.ibb.co/HrvHGZP/owaria.png)
