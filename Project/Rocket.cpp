#include "Rocket.h"

Rocket::Rocket(SDL_Renderer* renderer)
    : renderer(renderer), texture(nullptr), landed(false), hasLandedOrCrashed(false),
    timeElapsed(0.0f), fuel(2000), thrustBuild(0.0f)
{
    position = { 100, 100 };
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

    if (hasLandedOrCrashed) return;

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

void Rocket::render(TTF_Font* font) {
   int width = 6;  
    int height = 12;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    Vector2 top = { position.x, position.y - height / 2 };
    Vector2 bottomLeft = { position.x - width / 2, position.y + height / 2 };
    Vector2 bottomRight = { position.x + width / 2, position.y + height / 2 };
    Vector2 wingLeft = { position.x - width, position.y };
    Vector2 wingRight = { position.x + width, position.y };
    Vector2 thrusterLeft = { position.x - width / 4, position.y + height / 2 };
    Vector2 thrusterRight = { position.x + width / 4, position.y + height / 2 };
    Vector2 cockpitTop = { position.x - width / 4, position.y - height / 3 };
    Vector2 cockpitBottom = { position.x + width / 4, position.y - height / 4 };

    // Rotate points according to the rocket's angle
    float radian = angle * M_PI / 180.0f;
    auto rotatePoint = [&](Vector2 p) {
        float x = position.x + (p.x - position.x) * cos(radian) - (p.y - position.y) * sin(radian);
        float y = position.y + (p.x - position.x) * sin(radian) + (p.y - position.y) * cos(radian);
        return Vector2{ x, y };
    };

    top = rotatePoint(top);
    bottomLeft = rotatePoint(bottomLeft);
    bottomRight = rotatePoint(bottomRight);
    wingLeft = rotatePoint(wingLeft);
    wingRight = rotatePoint(wingRight);
    thrusterLeft = rotatePoint(thrusterLeft);
    thrusterRight = rotatePoint(thrusterRight);
    cockpitTop = rotatePoint(cockpitTop);
    cockpitBottom = rotatePoint(cockpitBottom);

    SDL_RenderDrawLine(renderer, top.x, top.y, bottomLeft.x, bottomLeft.y);
    SDL_RenderDrawLine(renderer, bottomLeft.x, bottomLeft.y, bottomRight.x, bottomRight.y);
    SDL_RenderDrawLine(renderer, bottomRight.x, bottomRight.y, top.x, top.y);

    SDL_RenderDrawLine(renderer, bottomLeft.x, bottomLeft.y, wingLeft.x, wingLeft.y);
    SDL_RenderDrawLine(renderer, bottomRight.x, bottomRight.y, wingRight.x, wingRight.y);

    SDL_SetRenderDrawColor(renderer, 0, 191, 255, 255); 
    SDL_RenderDrawLine(renderer, cockpitTop.x, cockpitTop.y, cockpitBottom.x, cockpitBottom.y);

    if (thrustBuild > 0) {
        int flameHeight = static_cast<int>(thrustBuild * 4);
        int maxFlameHeight = 10;
        flameHeight = std::min(flameHeight, maxFlameHeight);

        // Calculate the flame tip position
        Vector2 flameTip = { position.x, position.y + height / 2 + flameHeight };
        flameTip = rotatePoint(flameTip);  // Rotate the flame tip

        SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); // Orange flame
        SDL_RenderDrawLine(renderer, thrusterLeft.x, thrusterLeft.y, flameTip.x, flameTip.y);
        SDL_RenderDrawLine(renderer, thrusterRight.x, thrusterRight.y, flameTip.x, flameTip.y);
    }

    renderCrashMessages(font, renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Rocket::update(const std::vector<LandscapeLine>& lines, float deltaTime) {
    if (hasLandedOrCrashed) return;

    if (!landed) {
        timeElapsed += deltaTime;

        velocity.y += 5.5f * deltaTime;

        if (velocity.x > 0) {
            velocity.x = std::max(0.0f, velocity.x - 1.0f * deltaTime);
        }
        else if (velocity.x < 0) {
            velocity.x = std::min(0.0f, velocity.x + 1.0f * deltaTime);
        }

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
            const float ANGLE_TOLERANCE = 2.0f;
            const float SPEED_TOLERANCE = 2.0f;

            bool isAngleSafe = std::fabs(angle) <= (10.0f + ANGLE_TOLERANCE);
            bool isSpeedSafe = velocity.y < (25.0f + SPEED_TOLERANCE);

            crashReasons.clear(); // Clear previous crash reasons

            if (isLandingZone && isAngleSafe && isSpeedSafe) {
                land();
            }
            else {
                if (!isLandingZone) {
                    crashReasons.push_back("You landed on non-landable terrain!");
                }
                if (!isAngleSafe) {
                    crashReasons.push_back("Angle too steep! Angle: " + std::to_string(angle) + " (Limit: +-15)");
                }
                if (!isSpeedSafe) {
                    crashReasons.push_back("Speed too high! Speed: " + std::to_string(velocity.y) + " (Limit: < 25.0)");
                }

                crash();
            }
            return;
        }
    }
}

bool Rocket::lineIntersectsRocket(const LandscapeLine& line) {
    int rocketWidth = 6;
    int rocketHeight = 12;

    Vector2 bottomLeft = { position.x - rocketWidth / 2, position.y + rocketHeight / 2 };
    Vector2 bottomRight = { position.x + rocketWidth / 2, position.y + rocketHeight / 2 };

    return pointIsBelowLine(bottomLeft, line) || pointIsBelowLine(bottomRight, line);
}

bool Rocket::pointIsBelowLine(const Vector2& point, const LandscapeLine& line) {
    float t = (point.x - line.p1.x) / (line.p2.x - line.p1.x);
    if (t < 0 || t > 1) return false;

    float yOnLine = line.p1.y + t * (line.p2.y - line.p1.y);
    return point.y >= yOnLine;
}

void Rocket::renderCrashMessages(TTF_Font* font, SDL_Renderer* renderer) {
    if (!hasLandedOrCrashed || crashReasons.empty()) return;

    SDL_Color color = { 255, 0, 0, 255 }; // Red text for crash messages
    int yOffset = 20;

    for (const auto& reason : crashReasons) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, reason.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect dstRect = { 50, yOffset, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

        yOffset += 25; // Move to the next line

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

void Rocket::land() {
    if (landed) return;

    velocity = { 0, 0 };
    landed = true;
    hasLandedOrCrashed = true;
    std::cout << "Rocket Landed Safely" << std::endl;

    SDL_Delay(2000);
}

void Rocket::crash() {
    if (hasLandedOrCrashed) return;

    velocity = { 0, 0 };
    hasLandedOrCrashed = true;
    std::cout << "Rocket Crashed!" << std::endl;
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

bool Rocket::hasCrashed() const {
    return hasLandedOrCrashed && !landed;
}

bool Rocket::hasLanded() const {
    return landed;
}

void Rocket::setFuel(int amount) {
    fuel = amount;
}

void Rocket::reset() {
    position = { 100, 100 }; 
    velocity = { INITIAL_HORIZONTAL_SPEED, INITIAL_VERTICAL_SPEED };
    angle = 0.0f;
    landed = false;
    hasLandedOrCrashed = false;
    thrustBuild = 0.0f;
    timeElapsed = 0.0f;

    std::cout << "Rocket has been reset to its initial state." << std::endl;
}

void Rocket::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
