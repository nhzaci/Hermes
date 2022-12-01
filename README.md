# Hermes

Hermes is my project to create a complete end-to-end high level representation of a trading system.

The project is named Hermes as Hermes is the fastest greek god, which is in line with the aim of the project which is to create a fast, performant and extensible trading system comprising of multiple components.

Hermes makes use of C++20 concepts for compile-time polymorphism.

# Building the project

## Requirements

- C++20
- Clang++ 13.0
- CMake

## Building for debug

```bash
# allow execute permissions
chmod u+x debugBuild.sh
# run build
./debugBuild.sh
```

## Building for release

```bash
# allow execute permissions
chmod u+x releaseBuild.sh
# run build
./releaseBuild.sh
```

## Running binaries

All the executables added will be present in `~/build/bin/`

### Running all tests

```bash
# run all tests
./build/bin/TestMain # all tests are registered inside
```

### Running benchmarks
```bash
# run orderbook benchmark
./build/bin/OrderbookBenchmark
```

# Developer Documentation

- Found in the `docs/` directory, documents design decisions, requirements, design patterns etc
