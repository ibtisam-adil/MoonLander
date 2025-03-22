#include "Rocket.h"

Rocket::Rocket(SDL_Renderer* renderer)
    : renderer(renderer), texture(nullptr), landed(false), hasLandedOrCrashed(false)
{
    position = { SCREEN_WIDTH / 2.0f, 100 };
    velocity = { 0, 0 };
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

void Rocket::handleInput(const Uint8* keys) {
    if (keys[SDL_SCANCODE_LEFT]) angle = std::max(angle - ROTATION_SPEED, -90.0f);
    if (keys[SDL_SCANCODE_RIGHT]) angle = std::min(angle + ROTATION_SPEED, 90.0f);
    if (keys[SDL_SCANCODE_UP]) {
        float radian = angle * M_PI / 180.0f;
        velocity.x += std::sin(radian) * THRUST_POWER;
        velocity.y -= std::cos(radian) * THRUST_POWER;
    }
}

void Rocket::checkCollision(const std::vector<LandscapeLine>& lines) {
    if (hasLandedOrCrashed) return;

    for (const auto& line : lines) {
        if (lineIntersectsRocket(line)) {
            bool isLandingZone = line.landable;
            bool isAngleSafe = std::fabs(angle) <= 15.0f;
            bool isSpeedSafe = velocity.y < 5.0f;

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
                    std::cout << "Angle too steep! Your angle: " << angle << " (Limit: +- 15)" << std::endl;
                }
                if (!isSpeedSafe) {
                    std::cout << "Speed too high! Your vertical speed: " << velocity.y << " (Limit: < 5.0)" << std::endl;
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
    std::cout << "Plane Landed Safely" << std::endl;
}

void Rocket::crash() {
    if (hasLandedOrCrashed) return;

    velocity = { 0, 0 };
    hasLandedOrCrashed = true;
    std::cout << "Plane Crashed" << std::endl;
}

void Rocket::update(const std::vector<LandscapeLine>& lines) {
    if (hasLandedOrCrashed) return;
    if (!landed) {
        velocity.y += GRAVITY;
        position.x += velocity.x;
        position.y += velocity.y;

        checkCollision(lines);
    }
}

void Rocket::render() {
    if (!texture) return;

    SDL_Rect destRect = { static_cast<int>(position.x) - 10, static_cast<int>(position.y) - 20, 20, 40 };
    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, angle, nullptr, SDL_FLIP_NONE);
}

void Rocket::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
