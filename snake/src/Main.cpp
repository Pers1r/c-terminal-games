#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <random>

// --- OS Specific Includes for Non-Blocking Input ---

#include "InputHadler.h"
#include "Snake.h"

// --- Main Game Logic ---

// Constants
const int WIDTH = 40;
const int HEIGHT = 15;
const int TARGET_FPS = 5;

// Game State
Snake snake;
std::pair direction = {0, 0};
bool running = true;

void ClearScreen() {
    // ANSI escape code to clear screen and move cursor to (0,0)
    // Works on Linux, Mac, and Windows 10+
    std::cout << "\033[2J\033[H";
}

void HandleInput(InputHandler &input) {
    if (input.kbhit()) {
        char key = input.getch();
        switch (key) {
            case 'w':
        		if (direction.second == -1 || direction.second == 1) {
        			break;
        		}
        		direction = {0, -1};
        		break;
            case 's':
        		if (direction.second == 1 || direction.second == -1) {
        			break;
        		}
        		direction = {0, 1};
        		break;
            case 'a':
        		if (direction.first == -1 || direction.first == 1) {
        			break;
        		}
        		direction = {-1, 0};
        		break;
            case 'd':
        		if (direction.first == -1 || direction.first == 1) {
        			break;
        		}
        		direction = {1, 0};
        		break;
            case 'q': running = false; break; // Quit
        }
    }
}

void Update() {
    // Boundary checks
	snake.head.last_x = snake.head.pos.first;
	snake.head.last_y = snake.head.pos.second;
	snake.head.pos.first += direction.first;
	snake.head.pos.second += direction.second;
	for (auto & el : snake.body) {
		el.last_x = el.x;
		el.last_y = el.y;
		if (el.prev == nullptr) {
			el.x = snake.head.last_x;
			el.y = snake.head.last_y;
		} else {
			el.x = el.prev->last_x;
			el.y = el.prev->last_y;
		}
	}

    if (snake.head.pos.first < 1 || snake.head.pos.first >= WIDTH - 1 || snake.head.pos.second < 1 || snake.head.pos.second >= HEIGHT - 1) {
	    running = false;
    }
}

void Render(double fps) {
    // Construct the frame in a string buffer (prevents flickering)
    std::string buffer = "";

    // Move cursor home (ANSI)
    buffer += "\033[H";

    // Draw Top Border
    for (int i = 0; i < WIDTH; i++) buffer += "#";
    buffer += "\n";

    // Draw Game Area
    for (int y = 1; y < HEIGHT - 1; y++) {
        buffer += "#"; // Left Border
        for (int x = 1; x < WIDTH - 1; x++) {
            if (x == snake.head.pos.first && y == snake.head.pos.second) {
                buffer += "O"; // The Player
            } else {
            	bool tmp = false;
            	for (auto & el : snake.body) {
            		if (el.x == x && el.y == y) {
            			buffer += "o";
            			tmp = true;
            			break;
            		}
            	}
            	if (!tmp) {
            		buffer += " ";
            	}
            }
        }
        buffer += "#\n"; // Right Border
    }

    // Draw Bottom Border
    for (int i = 0; i < WIDTH; i++) buffer += "#";
    buffer += "\n";

    // Draw UI
    buffer += "FPS: " + std::to_string((int)fps) + " | Pos: " + std::to_string(snake.head.pos.first) + "," + std::to_string(snake.head.pos.second) + "\n";
    buffer += "Controls: WASD to move, Q to quit";

    // Print everything at once
    std::cout << buffer << std::flush;
}

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 100);

    InputHandler input;

    // FPS Management Variables
    auto lastTime = std::chrono::high_resolution_clock::now();
    double frameDuration = 1000.0 / TARGET_FPS; // ms per frame

    ClearScreen();

    while (running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = currentTime - lastTime;

        // Only update if enough time has passed (Frame Limiter)
        if (elapsed.count() >= frameDuration) {
            lastTime = currentTime;
            double currentFPS = 1000.0 / elapsed.count();

            HandleInput(input);
            Update();
            Render(currentFPS);
        } else {
            // Sleep briefly to reduce CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    ClearScreen();
    std::cout << "Game Over!" << std::endl;
    return 0;
}