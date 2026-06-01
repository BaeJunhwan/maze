# Maze Project

Eller's algorithm을 활용하여 랜덤 미로를 생성하고, 생성된 `.maz` 파일을 openFrameworks 환경에서 시각화하는 프로젝트입니다.
미로의 각 칸을 하나의 vertex로 보고, 벽이 없어 이동 가능한 통로를 edge로 해석하여 인접 리스트 형태로 저장하는 과정을 다루었습니다.

## Project Structure

```text
Maze/
├── generator/
│   ├── maze.c
│   ├── maze.h
│   └── Makefile
├── visualizer/
│   ├── main.cpp
│   ├── ofApp.cpp
│   ├── ofApp.h
│   ├── ofxWinMenu.cpp
│   └── ofxWinMenu.h
└──  maze.maz
```

## Features

* Eller's algorithm 기반 랜덤 미로 생성
* 오른쪽 벽과 아래쪽 벽 정보를 분리하여 미로 구조 관리
* `.maz` 파일 형식으로 미로 출력
* openFrameworks를 활용한 `.maz` 파일 시각화
* 미로의 각 칸을 vertex, 이동 가능한 통로를 edge로 해석
* 인접 리스트를 활용한 미로 그래프 구조 저장

## Development Environment

### Maze Generator

* Language: C
* Environment: Linux/Unix terminal
* Compiler: GCC
* Tool: PuTTY

### Maze Visualizer

* Language: C++
* Framework: openFrameworks
* IDE: Visual Studio
* OS: Windows


## Build & Run

### Maze Generator

```bash
cd generator
make
./a.out
```

프로그램 실행 후 미로의 가로 크기와 세로 크기를 숫자로 입력하면 `.maz` 형식의 미로 파일을 만들 수 있습니다.

### Maze Visualizer

본 시각화 프로그램은 openFrameworks 환경에서 실행됩니다.
`visualizer/` 폴더의 Visual Studio solution 파일을 열어 실행하면 `.maz` 파일을 불러와 미로를 화면에 출력할 수 있습니다.

## What I Learned

이 프로젝트를 통해 미로 생성 알고리즘이 실제 프로그램에서 어떻게 구현되는지 이해할 수 있었습니다.
특히 Eller's algorithm을 사용하면서 각 칸의 집합 정보를 관리하고, 벽을 제거하는 방식으로 미로를 구성하는 과정을 경험했습니다.

또한 생성된 `.maz` 파일을 openFrameworks에서 읽어 화면에 시각화하면서 파일 입출력과 시각화 과정이 어떻게 연결되는지 배웠습니다.
미로의 각 칸을 vertex, 이동 가능한 통로를 edge로 해석하여 인접 리스트로 저장하면서, 격자 형태의 문제를 그래프 구조로 모델링하는 방법을 익혔습니다.

## Future Improvements

* DFS 기반 미로 탐색 기능 추가
* BFS 기반 최단 경로 탐색 기능 추가
* 탐색 과정 시각화
* 시작점과 도착점 설정 기능 추가
