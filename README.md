# samp-keybinder

## Overview
This project is an AutoHotkey keybinder for GTA San Andreas Multiplayer (SA:MP) that features a user authentication GUI, a SocketIO connection, and a modern dark-themed main GUI with various functionalities.

## Project Structure
```
samp-keybinder
├── src
│   ├── main.ahk          ; Entry point for the application
│   ├── GUI
│   │   ├── auth.ahk     ; User authentication GUI
│   │   ├── main_gui.ahk  ; Main GUI layout with tab menu
│   │   └── custom_buttons.ahk ; Custom buttons for window control
│   ├── libs
│   │   ├── JSON.ahk     ; JSON parsing and generation functions
│   │   └── SocketIO.ahk ; SocketIO connection management
│   └── utils
│       └── theme.ahk    ; Functions for applying a dark theme
├── README.md             ; Project documentation
└── LICENSE               ; Licensing information
```

## Features
- User authentication with username and password.
- SocketIO connection to a local server at `127.0.0.1:4325`.
- Modern dark-themed GUI with a tab menu for easy navigation.
- Custom buttons for minimizing and closing the application window.

## Setup Instructions
1. Ensure you have AutoHotkey installed on your system.
2. Clone or download the repository.
3. Navigate to the `src` directory.
4. Run `main.ahk` to start the application.

## Usage Guidelines
- Enter your username and password in the authentication GUI to connect to the server.
- Use the main GUI to access various functionalities such as keybinds and settings.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request for any enhancements or bug fixes.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.