# info_h502_project

## Presentation of the project

This project is a flight simulator that aims to learn multiple techniques in OpenGL.

## 🌟 Features

- Reflection and Refraction in the water
- Frustum Culling
- Particles for the firecamp
- Instancing of the trees
- Skybox for the night
- Lights
- Atmospheric Model

## 🖼️ Images

![alt text](images/Screenshot_20260521_184405.png)
![alt text](images/Screenshot_20260521_184535.png)


## ⬇️ Installation
### Prerequisites
To run this project, you will need
- [https://www.assimp.org/](assimp)
- [https://www.sfml-dev.org/](sfml)
- [https://www.opengl.org/](openGL)

### Installation
`rm -rf build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE=Debug -S . -B build -G Ninja &&  cmake --build build

### Usage
`./build/VRproject