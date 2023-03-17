# Learn OpneGL

[Learn OpenGL](https://learnopengl.com/) 사이트에서 보고, 듣고, 느끼고, 배우는 것들을 기록


## Recent Result

[Model Loading ▶️ ASSIMP, Mesh, Model #2](https://github.com/habijung/learn-opengl/pull/2)


## Prerequisite

| Library | Version |
|:--------|--------:|
| GLAD    |     3.3 |
| GLFW    |   3.3.8 |
| GLM     | 0.9.9.8 |
| ASSIMP  |   5.2.5 | 


## Build

### JesBrains CLion

- Clone OpenGL project
```commandline
$ git clone git@github.com:habijung/learn-opengl.git
$ git submodule update --init --recursive --remote
```
- CLion에서 프로젝트 폴더 열기
- Build CMake
- Run Project

### Command Line

**For Windows 10 (or Later)**

- Install CMake for Windows
- Install MinGW 64-bit for Windows
- Install ninja.exe and add to **System Path**
- Clone OpenGL project
- Build CMake
```commandline
$ cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B build
$ cmake --build build --target Learn-OpenGL -j 12
```
- Run `build/Learn-OpenGL.exe`

---

**Updated:** 2023.03.17 15:52
