# Calculator

A modern, fast, and responsive desktop calculator built with **C++** and **Qt6 QML**. 

This project demonstrates a clean Model-View architecture, separating the heavy mathematical logic (C++) from a smooth, declarative user interface (QML) using modern Qt6 module practices.

## Features

* **Advanced Layout Engine:** A dynamic history list that perfectly aligns mathematical operators in a center column, while keeping numbers right-aligned. Long numbers automatically become horizontally scrollable to prevent UI breaking.
* **Global Keyboard Support:** Fully mapped keyboard shortcuts. You can type numbers and operators directly, use `Enter`/`Return` for results, `Esc` to clear, `Backspace` to delete, and the `Up`/`Down` arrow keys to smoothly scroll through your calculation history.
* **Modern Qt6 Architecture:** Built using Qt6's updated QML Module system (`qt_add_qml_module`) and declarative type registration (`QML_ELEMENT`, `QML_ANONYMOUS`).
* **Efficient Memory Management:** Uses standard Qt Object Trees and `QAbstractListModel` for high-performance UI rendering without memory leaks.
* **Linux Ready:** Includes fully compliant FreeDesktop `.desktop` and `.appdata.xml` files for seamless integration into Linux application launchers and software centers.

## Project Structure

```text
├── CMakeLists.txt           # Modern Qt6 CMake configuration
├── assets/
│   ├── linux/               # Linux desktop and appdata files
│   └── backspace.svg        # UI Icons
├── qml/                     # QML Frontend
│   ├── Application.qml      # Main Application Window
│   ├── Display.qml          # Screen and History ListView
│   ├── NumPad.qml           # Grid layout for buttons
│   └── components/          # Reusable UI components (Buttons, etc.)
└── src/                     # C++ Backend
    ├── main.cpp             # Application entry point & property injection
    ├── CalculatorEngine.h   # Core math logic and Q_PROPERTY definitions
    ├── CalculatorEngine.cpp
    ├── Logger.h             # Debug logging utility
    └── Logger.cpp
