# 🎮 Xonix Game (C++ & SFML)

This is a simple Xonix-style arcade game built using C++ and the SFML graphics library. The objective is to capture territory by enclosing empty areas while avoiding enemies.

## 🧠 Game Concept
You control a block that moves inside a rectangular field. When you leave the edge and move into the empty area, you leave a trail. If you return safely, the area is captured. If enemies hit your trail, you lose.

## 🛠️ Built With
- C++
- SFML (Simple and Fast Multimedia Library)
- CMake (for cross-platform builds)

## How to Build a Game on Linux:

Download the code, open terminal in the same folder, and execute the following commands:
· sudo apt update
· sudo apt install cmake
· sudo apt install build-essential
· sudo apt install libsfml-dev
· mkdir build
· cd build
· cmake ..
· make (use “sudo apt install make” if not already installed on your system)

Now open build folder and there is an executable file with name xonix. After First
compilation only make command need to compile code.
