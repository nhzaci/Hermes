# Trading System Project

# Architecture

## Components

### Gateway

- Communicates to external network of exchanges
- Subscribes to exchange feeds
- Handles outflow of orders onto exchanges

#### Network Protocols

##### FIX (Financial Information eXchange)

- Set of key-value pairs which numbers indicating certain tags

Example:

`8=FIX.4.42|9=76|35=A|34=1|49=TRADER1|52=20220117-12:11:44.224|56=VENUE1|98=0|108=30|141=Y|10=134`

##### FAST (FIX Adapted for STreaming)

- High-speed version of FIX protocol, operates on top of UDP
- Makes use of compression techniques

##### ITCH/OUCH protocol

- OUCH operates over TCP
- ITCH operates over multicast or TCP

##### CME Market Data Protocol

- CME (Chicago Merchantile Exchange) developed its own proprietary binary protocols famous for low-latency venues

### Orderbook

- Presents internal representation of orders from feed

### Strategy

- Contains signal and execution components

### Order Manager

- Decides if orders are sent / not sent based on risk parameters

# References

- Developing HFT Systems by Sebastien Donadio, Sourav Ghosh, Romain Rossier