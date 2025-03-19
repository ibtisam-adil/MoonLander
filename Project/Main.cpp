#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Vector2 {
    float x, y;
};

struct LandscapeLine {
    Vector2 p1, p2;
    bool landable;
    int multiplier;

    LandscapeLine(Vector2 a, Vector2 b) : p1(a), p2(b), landable(a.y == b.y), multiplier(1) {}
};

struct Star {
    float x, y;
};

class Landscape {
public:
    std::vector<Vector2> points;
    std::vector<LandscapeLine> lines;
    std::vector<Star> stars;

    float scale;
    float rightEdge;

    Landscape(int screenWidth) {
        scale = screenWidth / 600.0f;  // Adjust scaling to match the screen width
        setupData();
        adjustPoints();
        generateLines();
        generateStars();
    }

    void render(SDL_Renderer* renderer, int viewX) {
        int offset = 0;

        // Ensure landscape covers the entire screen width
        while (viewX - offset > rightEdge) offset += rightEdge;
        while (viewX - offset < 0) offset -= rightEdge;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Draw landscape lines
        for (size_t i = 0; i < lines.size(); i++) {
            Vector2 p1 = { lines[i].p1.x + offset, lines[i].p1.y };
            Vector2 p2 = { lines[i].p2.x + offset, lines[i].p2.y };

            if (p1.x >= 0 && p1.x <= SCREEN_WIDTH) {
                SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
            }

            if (lines[i].landable) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for landing zones
                SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
        }

        // Draw stars
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        for (const auto& star : stars) {
            float starX = star.x + offset;
            float starY = star.y;
            if (starX >= 0 && starX <= SCREEN_WIDTH) {
                SDL_RenderDrawPoint(renderer, starX, starY);
            }
        }
    }

private:
    void setupData() {
        points = {
            {0.5, 355.55}, {5.45, 355.55}, {6.45, 359.4}, {11.15, 359.4}, {12.1, 363.65},
            {14.6, 363.65}, {15.95, 375.75}, {19.25, 388}, {19.25, 391.9}, {21.65, 400},
            {28.85, 404.25}, {30.7, 412.4}, {33.05, 416.7}, {37.9, 420.5}, {42.7, 420.5},
            {47.4, 416.65}, {51.75, 409.5}, {56.55, 404.25}, {61.3, 400}, {63.65, 396.15},
            {68, 391.9}, {70.3, 388}, {75.1, 386.1}, {79.85, 379.95}, {84.7, 378.95},
            {89.05, 375.65}, {93.75, 375.65}, {98.5, 376.55}, {103.2, 379.95}, {104.3, 383.8},
            {107.55, 388}, {108.95, 391.9}, {112.4, 396.15}, {113.3, 400}, {117.1, 404.25},
            {121.95, 404.25}, {125.3, 396.3}, {128.6, 394.2}, {132.45, 396.15}, {135.75, 399.9},
            {138.15, 408.15}, {144.7, 412.4}, {146.3, 424.8}, {149.55, 436.65}, {149.55, 441.05},
            {154.35, 444.85}, {163.45, 444.85}, {168.15, 441.05}, {172.95, 436.75}, {175.45, 432.9},
            {179.7, 428.6}, {181.95, 424.8}, {186.7, 422.5}, {189.15, 412.4}, {191.55, 404.35},
            {196.35, 402.4}, {200.7, 398.1}, {205.45, 391.9}, {210.15, 383.8}, {212.55, 375.75}
        };
    }

    void adjustPoints() {
        float minX = points[0].x, maxX = points[0].x;

        for (auto& p : points) {
            p.x *= scale;
            p.y = SCREEN_HEIGHT - (p.y * scale);
            if (p.x < minX) minX = p.x;
            if (p.x > maxX) maxX = p.x;
        }

        // Shift all points so the leftmost one is at x = 0
        for (auto& p : points) {
            p.x -= minX;
        }

        rightEdge = maxX - minX;
    }

    void generateLines() {
        for (size_t i = 1; i < points.size(); i++) {
            lines.emplace_back(points[i - 1], points[i]);
        }
    }

    void generateStars() {
        for (const auto& line : lines) {
            if (rand() % 10 < 1) { // 10% chance to generate a star
                float starX = line.p1.x;
                float starY = (rand() % SCREEN_HEIGHT);
                if (starY < line.p1.y && starY < line.p2.y) {
                    stars.push_back({ starX, starY });
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("SDL2 Moon Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Landscape landscape(SCREEN_WIDTH);

    bool running = true;
    SDL_Event event;
    int viewX = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        landscape.render(renderer, viewX);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Simulate 60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
