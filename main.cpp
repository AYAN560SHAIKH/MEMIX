#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

void playBackgroundMusic(const std::string& filename) {
    PlaySound(filename.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void stopBackgroundMusic() {
    PlaySound(NULL, 0, 0);
}

int main() {
    Game game;
    bool running = true;

    while (running) {
        system("cls");
        std::cout << "=== MEMIX MATCHING GAME ===\n";
        std::cout << "1. Start New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Exit\n";
        std::cout << "Select option: ";

        std::string menuChoice;
        std::getline(std::cin, menuChoice);

        if (menuChoice == "1") {
            system("cls");
            std::cout << "Select difficulty:\n";
            std::cout << "1. Easy\n";
            std::cout << "2. Medium\n";
            std::cout << "3. Hard\n";
            std::cout << "4. Exit\n";
            std::cout << "Select difficulty: ";

            std::string difficultyChoice;
            std::getline(std::cin, difficultyChoice);

            if (difficultyChoice == "4") continue;

            game.setupGame(difficultyChoice);
            while (!game.isGameWon()) {
                game.playTurn();
                if (game.hasResigned()) break;
            }

            if (game.hasResigned()) {
                std::cout << "\nYou resigned. Returning to menu.\n";
                Sleep(1500);
                continue;
            }

            std::cout << "\n🎉 Congratulations! You completed the game! 🎉\n";
            std::cout << "Returning to menu...\n";
            Sleep(2000);

        } else if (menuChoice == "2") {
            if (game.loadProgress()) {
                while (!game.isGameWon()) {
                    game.playTurn();
                    if (game.hasResigned()) break;
                }

                if (game.hasResigned()) {
                    std::cout << "\nYou resigned. Returning to menu.\n";
                    Sleep(1500);
                    continue;
                }

                std::cout << "\n🎉 Congratulations! You completed the game! 🎉\n";
                std::cout << "Returning to menu...\n";
                Sleep(2000);
            } else {
                std::cout << "❌ Failed to load saved progress!\n";
                Sleep(1500);
            }
        } else if (menuChoice == "3") {
            running = false;
        } else {
            std::cout << "Invalid input. Try again.\n";
            Sleep(1000);
        }
    }

    return 0;
}
