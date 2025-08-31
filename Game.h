#pragma once
#include "Card.h"
#include <vector>
#include <string>

class Game {
private:
    std::vector<Card> cards;
    int rows;
    int cols;
    int score;
    bool resigned;
    int timerSeconds;
    std::string difficulty;

    void loadCodeOutputPairs();
    void generateBoard();
    void shuffleCards();
    void displayBoard(bool revealAll = false);
    void flashTimeout();

public:
    Game();
    void setupGame(const std::string& difficultyLevel);
    void playTurn();
    bool isGameWon() const;
    bool hasResigned() const;

    void saveProgress() const;
    bool loadProgress();
};
