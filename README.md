# Flight Simulator

## Presentation of the project

This project is a flight simulator that aims to learn multiple techniques in OpenGL. The goal of the game is to collect as many rings possible.

## 🌟 Features

- Reflection and Refraction in the water
- Frustum Culling
- Particles for the firecamp
- Instancing of the trees
- Skybox for the night
- Lights
- Atmospheric Model
- Shadows

## 🖼️ Images

![alt text](images/Screenshot_20260521_184405.png)
![alt text](images/Screenshot_20260521_184535.png)


## ⬇️ Installation
### Prerequisites
To run this project, you will need
- [https://www.opengl.org/](openGL)


### Setup Steps

1. Clone the repository (with `--recursive` flag to also install the submodules)
```
git clone https://github.com/moiravs/info_h502_project --recursive
```

2. Install the dependencies

You will need the three following packages:
- [https://freetype.org/](freetype)
- [https://www.assimp.org/](assimp)
- [https://www.sfml-dev.org/](sfml)
- [https://ninja-build.org/](ninja)
- C++ build tools

They can be installed:

- Arch
```sudo pacman -S sfml freetype2 assimp```

- macOS 
```brew install sfml freetype assimp```

- Ubuntu
```sudo apt install libassimp-dev libfreetype-dev ninja-build libfreetype-dev libwayland-dev pkg-config wayland-protocols libxkbcommon-dev libegl1-mesa-dev libegl-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev```

- Windows
Ninja can be installed with the command `winget install Ninja-build.Ninja`

Sorry for what follows, we greatly recommend compiling on absolutely anything else than Windows.

All of the following steps should be done from the "x64 Native Tools Command Prompt" (which should come with VS)

In `info_h502_project`: 
`git clone https://github.com/microsoft/vcpkg`
`cd vcpkg`
`bootstrap-vcpkg.bat`
`vcpkg install assimp:x64-windows`
`vcpkg install freetype:x64-windows`

Copy the contents of `info_h502_project/vcpkg/installed/x64-windows/bin` to `info_h502_project/build`
Delete `info_h502_project/CMakeLists.txt` and rename `info_h502_project/CMakeListsWindows.txt` to `info_h502_project/CMakeLists.txt`

3. Compile
```cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=Release -S . -B build -G Ninja &&  cmake --build build```

### Usage

To launch the project, use the following command on Unix:
```./build/VRproject```

Or simply launch `./build/VRproject.exe` on Windows.

### Controls

`T` = Toggle freecam

Plane camera:

⬅️➡️⬆️⬇️ / mouse = Control the camera

`Z`,`Q`,`S`,`D` = Move the plane

`SCROLL` = Change the distance to the plane

`LSHIFT + SCROLL` = Zoom

Free camera: 

⬅️➡️⬆️⬇️ / mouse = Control the camera

`Z`,`Q`,`S`,`D` = Move the camera

`LSHIFT` = Go down

`SPACE` = Go up

`SCROLL` = Zoom

## Acknowledgments
Thanks to these amazing websites for the models and the textures:
- Plane: https://www.cgtrader.com/free-3d-models/military/military-vehicle/focke-wulf-fw-190-d-9-dora-3d-model-wwii-german-fighter
- Campfire: https://www.cgtrader.com/free-3d-models/exterior/exterior-public/campfire-a6dfe064-3e9e-497f-97e4-7b3195effa79
- AppleTree: https://www.cgtrader.com/free-3d-models/plant/leaf/apple-tree-d7daa804-0643-4496-bb53-06b73a6368ec
- Font: https://all-free-download.com/font/download/gabriele_ribbon_fg_6918761.html
- Skybox: https://svs.gsfc.nasa.gov/4851/
- Textures for the terrain: https://www.magnific.com/

Thank you also to this really cool website to learn OpenGL which has been really useful in this project: https://learnopengl.com/