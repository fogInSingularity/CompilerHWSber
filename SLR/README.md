# SLR(1) parser

SLR(1) parser for basic arithmetic grammar. Parser uses precalculated GOTO and ACTION tables.

## Prerequisites

- CMake ≥ 3.21
- A C++20 compiler

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Usage

```bash
./build/slr.x <text_path>
```