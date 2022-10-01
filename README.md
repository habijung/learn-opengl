# Learn OpneGL

[Learn OpenGL](https://learnopengl.com/) 사이트에서 보고, 듣고, 느끼고, 배우는 것들을 기록


# Prerequisite

| Library | Version |
|:--------|--------:|
| GLAD    |     3.3 |
| GLFW    |   3.3.8 |
| GLM     | 0.9.9.8 |


# Build

JesBrains의 CLion에서 빌드

- Clone OpenGL project
```commandline
$ git clone git@github.com:habijung/learn-opengl.git
$ git submodule update --init --recursive
```

- Update Libraries Tag
```commandline
# GLFW
$ cd external/glfw
$ git checkout tags/3.3.8

# GLM
$ cd external/glm
$ git checkout tags/0.9.9.8

# ASSIMP
# 추가 예정
```

- CLion에서 프로젝트 폴더 열기
- Run Project
