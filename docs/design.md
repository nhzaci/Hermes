# Design Documentation

- Seeks to document design decisions used in this project.

# Design Patterns

## C++20 Concepts (TODO)

## Curiously Recurring Template Pattern (CRTP)

- As with any low latency project, making everything evaluated at compile time is very important. As such, templates in C++ provide a very powerful tool in allowing functions to be evaluated at compile time.
- In most other languages, abstract classes with derived classes would be the go-to, however, the runtime overhead of needing to store type information (RTTI) as well as having an indirection to evaluate which derived class it's calling takes up precious overhead.

- Here's an example, [reference](https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/):

```cpp
template <typename T>
class Base
{
  void someMethod() {
    T& derived = static_cast<T&>(*this);
    // do stuff with derived
    // e.g. derived.someMethod();
  }
  
private:
  Base() {};
  friend T;
};

class Derived : public Base<Derived>
{
  // ...
};
```

## PubSub (Publisher Subscriber)

- Making use of publisher subscriber pattern to communicate between services, delivering push-based updates to downstream components

## Strategy

- Define a family of algorithms, putting each of them into a separate class and make their objects interchangeable
- Used in Gateway where they could have different Protocols for different gateways

# High Level Design

## End-to-end Flow

Gateway (Ingress) -> Orderbook -> Strategy -> OrderManager -> Routing -> Gateway (Egress)

## Gateway

### Requirements

- Allow switching of underlying protocols as protocols get updated over time
- Allows sending of orders through gateway

## Order Book

### Operations

- Insertion
  - Adds a new order into the book
  - Time: O(1) or O(log n)
- Modification
  - Use order ID to seek order in the book
  - Should have similar time complexity to insertion
- Cancelation
  - Allows an order to be withdrawn from the book

### Requirements

- Constant look-up, fast quantity update
- Iteration in order of prices
- Retrieving best bid and ask in constant time
