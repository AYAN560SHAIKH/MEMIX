#include "Game.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <thread>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

Game::Game() : rows(0), cols(0), score(0), resigned(false), timerSeconds(40) {}

void Game::setupGame(const std::string& difficultyLevel) {
    resigned = false;
    difficulty = difficultyLevel;
    cards.clear();

    if (difficulty == "1") { // Easy
        rows = 2;
        cols = 4;
    } else if (difficulty == "2") { // Medium
        rows = 3;
        cols = 4;
    } else if (difficulty == "3") { // Hard
        rows = 4;
        cols = 5;
    } else {
        std::cout << "Invalid difficulty. Defaulting to Easy.\n";
        rows = 2;
        cols = 4;
    }

    loadCodeOutputPairs();
    generateBoard();
    shuffleCards();
}

void Game::loadCodeOutputPairs() {
    std::vector<std::pair<std::string, std::string>> codePairs = {
        {"int x = 5; std::cout << x;", "5"},
        {"std::cout << 2 + 3;", "5"},
        {"std::cout << 4 * 2;", "8"},
        {"std::cout << 10 / 2;", "5"},
        {"int a = 3; a += 2; std::cout << a;", "5"},
        {"std::cout << (true ? 1 : 0);", "1"},
        {"std::cout << (3 > 2);", "1"},
        {"std::cout << (4 == 5);", "0"},
        {"std::cout << (6 % 4);", "2"},
        {"std::cout << 'A';", "A"},
        {"std::cout << (7 - 2);", "5"},
        {"std::cout << 3 * 3;", "9"},
        {"std::cout << (8 + 1);", "9"},
        {"std::cout << (10 > 3 ? 2 : 4);", "2"},
        {"std::cout << (5 >= 5);", "1"}
    };

    int pairCount = (rows * cols) / 2;
    std::random_shuffle(codePairs.begin(), codePairs.end());

    for (int i = 0; i < pairCount; ++i) {
        cards.emplace_back(codePairs[i].first, i, true);
        cards.emplace_back(codePairs[i].second, i, false);
    }
}

void Game::generateBoard() {
    std::random_shuffle(cards.begin(), cards.end());
}

void Game::shuffleCards() {
    std::random_shuffle(cards.begin(), cards.end());
}

void Game::displayBoard(bool revealAll) {
    system("cls");
    std::cout << "Score: " << score << "\n";
    std::cout << "Type 'R' at any time to resign.\n\n";

    for (int i = 0; i < rows * cols; ++i) {
        if (i % cols == 0) std::cout << "\n";

        if (cards[i].isMatched() || cards[i].isRevealed() || revealAll) {
            std::cout << "[" << cards[i].getContent() << "] ";
        } else {
            std::cout << "[" << i << "] ";
        }
    }
    std::cout << "\n";
}

void Game::flashTimeout() {
    for (int i = 0; i < 2; ++i) {
        displayBoard(true);
        Beep(750, 300);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        displayBoard(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void Game::playTurn() {
    displayBoard();

    auto start = std::chrono::steady_clock::now();
    int first, second;
    std::string input;

    std::cout << "\nEnter two card indices separated by space (or 'R' to resign): ";
    std::getline(std::cin, input);

    if (input == "R" || input == "r") {
        resigned = true;
        return;
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    try {
        size_t space = input.find(' ');
        first = std::stoi(input.substr(0, space));
        second = std::stoi(input.substr(space + 1));
    } catch (...) {
        std::cout << "Invalid input.\n";
        Sleep(1000);
        return;
    }

    if (first < 0 || second < 0 || first >= cards.size() || second >= cards.size() || first == second) {
        std::cout << "Invalid indices.\n";
        Sleep(1000);
        return;
    }

    if (elapsed > timerSeconds) {
        std::cout << "\n⏰ Time's up!\n";
        PlaySound("timeout.wav", NULL, SND_FILENAME | SND_ASYNC);
        flashTimeout();
        return;
    }

    cards[first].reveal();
    cards[second].reveal();
    displayBoard();

    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();

    if (cards[first].getPairID() == cards[second].getPairID() &&
        cards[first].isCode() != cards[second].isCode()) {
        std::cout << "\n✅ Match Found!\n";
        PlaySound("match.wav", NULL, SND_FILENAME | SND_ASYNC);
        cards[first].setMatched(true);
        cards[second].setMatched(true);
        score += 10;
    } else {
        std::cout << "\n❌ Not a Match.\n";
        PlaySound("nomatch.wav", NULL, SND_FILENAME | SND_ASYNC);
        cards[first].hide();
        cards[second].hide();
    }

    Sleep(1000);
}

bool Game::isGameWon() const {
    for (const auto& card : cards) {
        if (!card.isMatched()) return false;
    }
    return true;
}

bool Game::hasResigned() const {
    return resigned;
}

void Game::saveProgress() const {
    std::ofstream file("save.dat", std::ios::binary);
    int total = cards.size();
    file.write((char*)&total, sizeof(int));
    for (const auto& card : cards) {
        size_t len = card.getContent().size();
        file.write((char*)&len, sizeof(size_t));
        file.write(card.getContent().c_str(), len);
        int id = card.getPairID();
        file.write((char*)&id, sizeof(int));
        bool isCode = card.isCode();
        file.write((char*)&isCode, sizeof(bool));
        bool revealed = card.isRevealed();
        file.write((char*)&revealed, sizeof(bool));
        bool matched = card.isMatched();
        file.write((char*)&matched, sizeof(bool));
    }
    file.write((char*)&rows, sizeof(int));
    file.write((char*)&cols, sizeof(int));
    file.write((char*)&score, sizeof(int));
    file.close();
}

bool Game::loadProgress() {
    std::ifstream file("save.dat", std::ios::binary);
    if (!file) return false;

    cards.clear();
    int total;
    file.read((char*)&total, sizeof(int));
    for (int i = 0; i < total; ++i) {
        size_t len;
        file.read((char*)&len, sizeof(size_t));
        std::string content(len, ' ');
        file.read(&content[0], len);
        int id;
        file.read((char*)&id, sizeof(int));
        bool isCode;
        file.read((char*)&isCode, sizeof(bool));
        bool revealed, matched;
        file.read((char*)&revealed, sizeof(bool));
        file.read((char*)&matched, sizeof(bool));

        Card card(content, id, isCode);
        if (revealed) card.reveal();
        if (matched) card.setMatched(true);
        cards.push_back(card);
    }
    file.read((char*)&rows, sizeof(int));
    file.read((char*)&cols, sizeof(int));
    file.read((char*)&score, sizeof(int));
    file.close();
    return true;
}
