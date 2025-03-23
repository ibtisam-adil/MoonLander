#include "Rocket.h"

Rocket::Rocket(SDL_Renderer* renderer)
    : renderer(renderer), texture(nullptr), landed(false), hasLandedOrCrashed(false),
    fuel(1000), thrustBuild(0.0f)
{
    position = { SCREEN_WIDTH / 4.0f, 100 };
    velocity = { INITIAL_HORIZONTAL_SPEED, INITIAL_VERTICAL_SPEED };
    angle = 0.0f;
}

Rocket::~Rocket() {
    cleanup();
}

bool Rocket::loadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cout << "Failed to load rocket image: " << IMG_GetError() << std::endl;
        return false;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture != nullptr;
}

void Rocket::handleInput(const Uint8* keys, float deltaTime) {
    if (keys[SDL_SCANCODE_LEFT]) angle = std::max(angle - ROTATION_SPEED, -90.0f);
    if (keys[SDL_SCANCODE_RIGHT]) angle = std::min(angle + ROTATION_SPEED, 90.0f);

    if (keys[SDL_SCANCODE_UP] && fuel > 0) {
        float radian = angle * M_PI / 180.0f;
        thrustBuild += (THRUST_POWER - thrustBuild) * 0.2f;

        velocity.x += std::sin(radian) * thrustBuild * deltaTime;
        velocity.y -= 2.5f * std::cos(radian) * thrustBuild * deltaTime;

        fuel -= 1; 

    }
    else {
        thrustBuild *= 0.9f;
    }
}

void Rocket::update(const std::vector<LandscapeLine>& lines, float deltaTime) {
    if (hasLandedOrCrashed) return;

    if (!landed) {
        timeElapsed += deltaTime;

        // Always apply gravity: Vertical speed increases by +1 per second
        velocity.y += 5.5f * deltaTime;

        // Reduce horizontal speed naturally towards 0
        if (velocity.x > 0) {
            velocity.x = std::max(0.0f, velocity.x - 1.0f * deltaTime);
        }
        else if (velocity.x < 0) {
            velocity.x = std::min(0.0f, velocity.x + 1.0f * deltaTime);
        }

        // Apply movement
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;


        checkCollision(lines);
    }
}


void Rocket::checkCollision(const std::vector<LandscapeLine>& lines) {
    if (hasLandedOrCrashed) return;

    for (const auto& line : lines) {
        if (lineIntersectsRocket(line)) {
            bool isLandingZone = line.landable;
            bool isAngleSafe = std::fabs(angle) <= 35.0f;
            bool isSpeedSafe = velocity.y < 35.0f;

            if (isLandingZone && isAngleSafe && isSpeedSafe) {
                land();
                std::cout << " Landed successfully on a safe zone!" << std::endl;
            }
            else {
                std::cout << " Crash detected!" << std::endl;

                if (!isLandingZone) {
                    std::cout << "You landed on non-landable terrain!" << std::endl;
                }
                if (!isAngleSafe) {
                    std::cout << "Angle too steep! Your angle: " << angle << " (Limit: +- 35)" << std::endl;
                }
                if (!isSpeedSafe) {
                    std::cout << "Speed too high! Your vertical speed: " << velocity.y << " (Limit: < 15.0)" << std::endl;
                }

                crash();
            }
            return;
        }
    }
}

bool Rocket::lineIntersectsRocket(const LandscapeLine& line) {
    Vector2 bottomLeft = { position.x - 10, position.y + 20 };
    Vector2 bottomRight = { position.x + 10, position.y + 20 };

    return pointIsBelowLine(bottomLeft, line) || pointIsBelowLine(bottomRight, line);
}

bool Rocket::pointIsBelowLine(const Vector2& point, const LandscapeLine& line) {
    float t = (point.x - line.p1.x) / (line.p2.x - line.p1.x);
    if (t < 0 || t > 1) return false;

    float yOnLine = line.p1.y + t * (line.p2.y - line.p1.y);
    return point.y >= yOnLine;
}

void Rocket::land() {
    if (landed) return;

    velocity = { 0, 0 };
    landed = true;
    std::cout << "Rocket Landed Safely" << std::endl;
}

void Rocket::crash() {
    if (hasLandedOrCrashed) return;

    velocity = { 0, 0 };
    hasLandedOrCrashed = true;
    std::cout << "Rocket Crashed!" << std::endl;
}

void Rocket::render() {
    if (!texture) return;

    SDL_Rect destRect = { static_cast<int>(position.x) - 10, static_cast<int>(position.y) - 20, 20, 40 };
    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, angle, nullptr, SDL_FLIP_NONE);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect highlightRect = { static_cast<int>(position.x) - 10, static_cast<int>(position.y) - 20, 20, 40 };
    SDL_RenderDrawRect(renderer, &highlightRect);
}

float Rocket::getAltitude(const std::vector<LandscapeLine>& lines) {
    float closestGroundY = SCREEN_HEIGHT; 
    float rocketBottomY = position.y + 20;

    for (const auto& line : lines) {
        if (position.x >= line.p1.x && position.x <= line.p2.x) {
            float t = (position.x - line.p1.x) / (line.p2.x - line.p1.x);
            float groundY = line.p1.y + t * (line.p2.y - line.p1.y);

            if (groundY < closestGroundY) {
                closestGroundY = groundY;
            }
        }
    }

    return closestGroundY - rocketBottomY;
}

Vector2 Rocket::getVelocity() { return velocity; }
int Rocket::getFuel() { return fuel; }
float Rocket::getTimeElapsed() { return timeElapsed; }


void Rocket::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
