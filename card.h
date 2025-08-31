#pragma once
#include <string>

class Card {
private:
    std::string content;
    int pairID;
    bool revealed;
    bool matched;
    bool isCodeCard;

public:
    Card(const std::string& content = "", int pairID = -1, bool isCode = true);

    std::string getContent() const;
    int getPairID() const;
    bool isRevealed() const;
    bool isMatched() const;
    bool isCode() const;

    void reveal();
    void hide();
    void setMatched(bool value);
};
