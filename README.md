# Hermes

Hermes is my project to create a complete end-to-end high level representation of a trading system.

The project is named Hermes as Hermes is the fastest greek god, which is in line with the aim of the project which is to create a fast, performant and extensible trading system comprising of multiple components.

Hermes makes use of C++20 concepts for compile-time polymorphism.

# Building the project

- Give execute permissions for `./build.sh`

```bash
# allow execute permissions
chmod u+x build.sh

# build for debug by default
./build.sh

# build for release
./build.sh -r
```

## Requirements

- C++20
- Clang++ 13.0
- CMake

## Running binaries

### Running all tests

```bash
# run all tests
./build/test/TestMain # all tests are registered inside
```

### Running benchmarks

```bash
# run orderbook benchmark
./build/bench/orderbook/OrderbookBenchmark
```

# Developer Documentation

- Found in the `docs/` directory, documents design decisions, requirements, design patterns etc
