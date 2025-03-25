# 🚀 Moon Lander Game

A 2D Moon Lander game built using **C++** and **SDL2**. The game challenges players to land a rocket safely on a procedurally generated terrain while managing fuel and avoiding crashes.

## 🎮 Features
- Realistic gravity and thrust mechanics
- Procedural terrain inspired by CoinMarketCap line charts
- Smooth physics-based movement
- Fuel management system
- Landing and crashing detection
- Simple but effective visual effects

## 🛠️ Installation & Setup
1. **Clone the Repository**
   ```sh
   git clone https://github.com/yourusername/moon-lander-game.git
   cd moon-lander-game
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
   g++ -o moon_lander main.cpp Rocket.cpp LandscapeLine.cpp Vector2.cpp -lSDL2 -lSDL2_image
   ```
4. **Run the Game**
   ```sh
   ./moon_lander
   ```

## 🎮 Controls
| Key | Action |
|-----|--------|
| Left Arrow  | Rotate Left |
| Right Arrow | Rotate Right |
| Up Arrow    | Apply Thrust |

## 📷 Screenshots
*(Include screenshots of gameplay here)*

## 🛠️ Planned Features
- 🚀 Different difficulty modes
- 🌕 More varied terrains
- 🔥 Enhanced visual effects
- 🎵 Sound effects & music

## 🤝 Contributing
Feel free to submit issues and pull requests to improve the game!

## 📜 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
Made with ❤️ by **Your Name**
