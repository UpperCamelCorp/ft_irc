# IRC Server Project

## Overview

This project aims to create an IRC (Internet Relay Chat) server using C++ and sockets. The server will handle multiple clients, allowing them to connect, communicate, and exchange messages in real-time.

## Features

- Multi-client support
- Real-time messaging
- User authentication
- Channel creation and management
- Private messaging

## Requirements

- C++98 or later
- POSIX compliant operating system (Linux, macOS)
- GCC or Clang compiler

## Installation

1. Clone the repository:
    ```sh
    git clone git@github.com:UpperCamelCorp/ft_irc.git irc
    cd irc
    ```

2. Build the project:
    ```sh
    make
    ```

3. Run the server:
    ```sh
    ./ircserv 6690 salutsalut
    ```

## Usage

1. Start the client :
   ```sh
     irssi ( for example )
   ```

3. Connect to the server using an IRC client:
   
    - Server address: `localhost`
    - Port: `6690`
    - Pass : `salutsalut` ( between 6 and 20 characters )

   ```sh
   /connect localhost 6690 salutsalut
   ```
## Contributing

Contributions are welcome! Please fork the repository and submit a pull request.
