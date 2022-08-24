# authentication process
1. client sends `IDENTIFY <nickname>`
2. server accepts connection and puts the client into the default welcome room

# TODOs
- encryption
- replace `sf::SocketSelector` with some homebrewed solution which is able to
  asynchronously send and receive
