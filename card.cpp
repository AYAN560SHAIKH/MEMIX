#include "Card.h"

Card::Card(const std::string& content, int pairID, bool isCode)
    : content(content), pairID(pairID), revealed(false), matched(false), isCodeCard(isCode) {}

std::string Card::getContent() const {
    return content;
}

int Card::getPairID() const {
    return pairID;
}

bool Card::isRevealed() const {
    return revealed;
}

bool Card::isMatched() const {
    return matched;
}

bool Card::isCode() const {
    return isCodeCard;
}

void Card::reveal() {
    revealed = true;
}

void Card::hide() {
    revealed = false;
}

void Card::setMatched(bool value) {
    matched = value;
}
