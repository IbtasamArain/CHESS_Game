#include <iostream>
#include "chess.h" // Apni chess header file include kar rahe hain

using namespace std;

int main() {
    // Game class ka ek object banayein taake game start ho sake
    Game chessGame;

    // Game loop ko start karein (yeh call chess.cpp wali Game::play() run karegi jahan color logic majood hai)
    chessGame.play();

    //for compile command
    //g++ main.cpp chess.cpp -o chessGame.exe
    //for run command
    //.\chessGame.exe

    return 0; 
}
