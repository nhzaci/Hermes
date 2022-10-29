# Hermes

Hermes is my project to create a complete end-to-end high level representation of a trading system.

The project is named Hermes as Hermes is the fastest greek god, which is in line with the aim of the project which is to create a fast, performant and extensible trading system comprising of multiple components.

# Requirements

- C++20
- CMake

# Documentation

- Found in the `docs/` directory, documents design decisions, requirements, design patterns etc

# Components

## Gateway

- Communicates to external network of exchanges

## Orderbook

- Presents internal representation of orders from feed

## Strategy

- Contains signal and execution components

## Order Manager

- Decides if orders are sent / not sent based on risk parameters

# References

- Developing HFT Systems by Sebastien Donadio, Sourav Ghosh, Romain Rossier