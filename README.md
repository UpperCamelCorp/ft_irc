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

- C++11 or later
- POSIX compliant operating system (Linux, macOS)
- CMake 3.10 or later
- GCC or Clang compiler

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/irc-server.git
    cd irc-server
    ```

2. Build the project:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

3. Run the server:
    ```sh
    ./irc-server
    ```

## Usage

1. Start the server:
    ```sh
    ./irc-server
    ```

2. Connect to the server using an IRC client:
    - Server address: `localhost`
    - Port: `6667`

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or suggestions, please open an issue or contact the project maintainer at `your.email@example.com`.