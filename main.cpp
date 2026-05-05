#include <iostream>
#include "chess.h" // Apni chess header file include kar rahe hain

using namespace std;

int main() {
    // Game class ka ek object banayein taake game start ho sake
    Game chessGame;

    // Game loop ko start karein (yeh call chess.cpp wali Game::play() run karegi jahan color logic majood hai)
    chessGame.play();

    return 0; 
}