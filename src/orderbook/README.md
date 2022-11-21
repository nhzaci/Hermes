# Orderbook

The orderbook directory contains several components:
- OrderType
  - Currently has 3 types, Limit, Market and FOK, more to be added
- Order
  - Contains interface for a single order, has several attributes
- Container
  - Contains orders inside the container, can be ordered according to a Compare, which can be std::less or std::greater according to price_t of Order
- Orderbook
  - Contains 2 containers, performs CRUD and crossing of orders on Container
  
TODO:
- [X] Support bids and asks
- [X] Insert, remove, modify orders
- [X] Crossing for limit orders
- [] Refactor order crossing algo into separate strategy
- [] Support Market orders
- [] Support FOK orders
- [] Support more order types

## Order

An order should have several things:
- price
- quantity
- id
- exchange id (or client id)
- order type
  - Limit
  - FOK
  - Market
  
## Container

A container should be able to:
- Sort orders within the container according to a specified Compare function
- Insert orders
- Modify orders
- Delete orders
- Perform crossing of the spread with an order of the opposite side
- Get current best price inside the container

## Orderbook

An orderbook should be able to:
- Contain bid and ask containers
- Insert, modify, delete orders
- Perform crossing of spread
- Get spread between the two containers