#ifndef CHESS_H
#define CHESS_H


//  chess.h — All Class Declarations
//  Contains: Color enum, Piece (abstract), all 6 piece classes,
//            Board class, Game class

#include <iostream>
#include <string>
using namespace std;


//  ENUM: Color
enum Color { WHITE, BLACK, NONE };


//  FORWARD DECLARATION
//  Board is used inside Piece::isValidMove(), so we declare it
//  here before the Piece class definition
class Board;


//  ABSTRACT BASE CLASS: Piece
//  - Encapsulation  : private/protected data members
//  - Abstraction    : pure virtual functions
//  - All pieces inherit from this
class Piece {
protected:
    Color color;    // WHITE or BLACK
    int   row, col; // position on the 8x8 board (0-indexed)
    bool  hasMoved; // used by Pawn (2-square first move)

public:
    // Constructor
    Piece(Color c, int r, int co);

    // Virtual destructor — needed for safe deletion via base pointer
    virtual ~Piece() {}

    // --- PURE VIRTUAL FUNCTIONS (makes Piece abstract) ---
    virtual bool isValidMove(int toRow, int toCol, Board& board) const = 0;
    virtual char getSymbol()  const = 0;

    // --- GETTERS ---
    Color getColor()    const;
    int   getRow()      const;
    int   getCol()      const;
    bool  getHasMoved() const;

    // --- SETTERS ---
    void setPosition(int r, int c);
    void setHasMoved(bool val);

    // Helper
    bool isWhite() const;
};


//  DERIVED CLASS: Pawn
//  Inherits from Piece
class Pawn : public Piece {
public:
    Pawn(Color c, int r, int co);
    char getSymbol()  const override;
    bool isValidMove(int toRow, int toCol, Board& board) const override;
};


//  DERIVED CLASS: Rook
class Rook : public Piece {
public:
    Rook(Color c, int r, int co);
    char getSymbol()  const override;
    bool isValidMove(int toRow, int toCol, Board& board) const override;
};


//  DERIVED CLASS: Knight
class Knight : public Piece {
public:
    Knight(Color c, int r, int co);
    char getSymbol()  const override;
    bool isValidMove(int toRow, int toCol, Board& board) const override;
};


//  DERIVED CLASS: Bishop
class Bishop : public Piece {
public:
    Bishop(Color c, int r, int co);
    char getSymbol()  const override;
    bool isValidMove(int toRow, int toCol, Board& board) const override;
};


//  DERIVED CLASS: Queen
class Queen : public Piece {
public:
    Queen(Color c, int r, int co);
    char getSymbol()  const override;
    bool isValidMove(int toRow, int toCol, Board& board) const override;
};


//  DERIVED CLASS: King
class King : public Piece {
public:
    King(Color c, int r, int co);
    char getSymbol()  const override;
    bool isValidMove(int toRow, int toCol, Board& board) const override;
};


//  CLASS: Board
//  - Composition: contains Piece* objects in a 2D grid
//  - Manages the 8x8 board state
class Board {
private:
    Piece* grid[8][8]; // 8x8 array of Piece pointers; nullptr = empty

public:
    Board();
    ~Board();

    // Place / Get a piece
    void   setPiece(int row, int col, Piece* piece);
    Piece* getPiece(int row, int col) const;

    // Square state helpers
    bool isEmpty(int row, int col)                const;
    bool isEnemy(int row, int col, Color myColor) const;
    bool isFriendly(int row, int col, Color myColor) const;
    bool inBounds(int row, int col)               const;

    // Display board in console
    void display() const;

    // Find King position for a color
    bool findKing(Color color, int& kingRow, int& kingCol) const;

    // Check detection
    bool isInCheck(Color color);

    // Execute / Undo a move (undo used for check-testing)
    Piece* executeMove(int fromRow, int fromCol, int toRow, int toCol);
    void   undoMove(int fromRow, int fromCol, int toRow, int toCol, Piece* captured);
};


//  CLASS: Game
//  - Manages overall game flow: setup, turns, input, win/draw
//  - Composition: Game HAS-A Board
class Game {
private:
    Board board;       // Composition: Game HAS-A Board
    Color currentTurn; // whose turn is it?
    bool  gameOver;    // has the game ended?

    // Private helper methods
    void setupBoard();
    bool parseInput(const string& input,
                    int& fromRow, int& fromCol,
                    int& toRow,   int& toCol);
    bool hasAnyLegalMove(Color color);
    void handlePawnPromotion(int row, int col);

public:
    Game();
    void play(); // main game loop
};


#endif // CHESS_H
