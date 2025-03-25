# 🚀 Moon Lander Game

A 2D Moon Lander game built using **C++** and **SDL2**. The game challenges players to land a rocket safely on a moon Land while managing fuel and avoiding crashes.

## 🎮 Features
- Realistic gravity and thrust mechanics
- Smooth physics-based movement
- Fuel management system
- Landing and crashing detection
- Simple but effective visual effects

## 📝 Inspiration
This game was inspired by [Seb Lee-Delisle's Moon Lander](http://moonlander.seb.ly/). While the core idea remains similar, this project is an independent implementation built from scratch using C++ and SDL2, with additional mechanics and features.

## 🛠️ Installation & Setup
1. **Clone the Repository**
   ```sh
   git clone https://github.com/ibtisam-adil/MoonLander.git
   cd MoonLander
   ```
2. **Install Dependencies**
   Ensure you have SDL2 and SDL2_image installed:
   - **Windows**: Use vcpkg or install manually
   - **Linux (Ubuntu/Debian)**:
     ```sh
     sudo apt-get install libsdl2-dev libsdl2-image-dev
     ```
   - **Mac (Homebrew)**:
     ```sh
     brew install sdl2 sdl2_image
     ```
3. **Compile the Game**
   ```sh
   g++ -o MoonLander main.cpp Rocket.cpp LandscapeLine.cpp Vector2.cpp -lSDL2 -lSDL2_image
   ```
4. **Run the Game**
   ```sh
   ./MoonLander
   ```

## 🎮 Controls
| Key | Action |
|-----|--------|
| Left Arrow  | Rotate Left |
| Right Arrow | Rotate Right |
| Up Arrow    | Apply Thrust |

## 🛠️ Planned Features
- 🚀 Different difficulty modes
- 🌕 More varied terrains
- 🔥 Enhanced visual effects

## 🤝 Contributing
Feel free to submit issues and pull requests to improve the game!

## 📜 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
Made with ❤️ by **Ibtisam Adil**

