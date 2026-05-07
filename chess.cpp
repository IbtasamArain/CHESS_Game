
//  chess.cpp — All Class Implementations
//  Includes: Piece, Pawn, Rook, Knight, Bishop, Queen, King,
//            Board, Game

#include "chess.h"   // our own header
#include <cctype>    // for tolower()
using namespace std;


//  Piece — Base Class Implementations

Piece::Piece(Color c, int r, int co)
    : color(c), row(r), col(co), hasMoved(false) {}

Color Piece::getColor()    const { return color; }
int   Piece::getRow()      const { return row; }
int   Piece::getCol()      const { return col; }
bool  Piece::getHasMoved() const { return hasMoved; }

void Piece::setPosition(int r, int c) { row = r; col = c; }
void Piece::setHasMoved(bool val)     { hasMoved = val; }

bool Piece::isWhite() const { return color == WHITE; }


//  Pawn — Implementation
//  Rules:
//    - 1 square forward (2 on first move)
//    - Captures 1 square diagonally forward
//    - White moves UP (row+1), Black moves DOWN (row-1)

Pawn::Pawn(Color c, int r, int co) : Piece(c, r, co) {}

char Pawn::getSymbol() const {
    return (color == WHITE) ? 'P' : 'p';
}

bool Pawn::isValidMove(int toRow, int toCol, Board& board) const {
    // direction: White goes up (+1), Black goes down (-1)
    int direction = (color == WHITE) ? 1 : -1;

    int dr = toRow - row;  // row difference
    int dc = toCol - col;  // column difference

    // CASE 1: Move 1 square straight forward (no capture)
    if (dc == 0 && dr == direction)
        return board.isEmpty(toRow, toCol);

    // CASE 2: Move 2 squares forward on very first move
    if (dc == 0 && dr == 2 * direction && !hasMoved)
        return board.isEmpty(row + direction, col) && board.isEmpty(toRow, toCol);

    // CASE 3: Diagonal capture (1 square diagonally forward)
    if (abs(dc) == 1 && dr == direction)
        return board.isEnemy(toRow, toCol, color);

    return false; // everything else is invalid
}


//  Rook — Implementation
//  Rules:
//    - Any number of squares horizontally or vertically
//    - Cannot jump over other pieces

Rook::Rook(Color c, int r, int co) : Piece(c, r, co) {}

char Rook::getSymbol() const {
    return (color == WHITE) ? 'R' : 'r';
}

bool Rook::isValidMove(int toRow, int toCol, Board& board) const {
    // Must stay in the same row OR same column
    if (toRow != row && toCol != col) return false;

    // Cannot land on friendly piece
    if (board.isFriendly(toRow, toCol, color)) return false;

    // Check path is clear
    if (toRow == row) {
        // Horizontal movement
        int step = (toCol > col) ? 1 : -1;
        for (int c = col + step; c != toCol; c += step)
            if (!board.isEmpty(row, c)) return false;
    } else {
        // Vertical movement
        int step = (toRow > row) ? 1 : -1;
        for (int r = row + step; r != toRow; r += step)
            if (!board.isEmpty(r, col)) return false;
    }

    return true;
}


//  Knight — Implementation
//  Rules:
//    - Moves in L-shape: 2+1 or 1+2 squares
//    - Only piece that CAN jump over others

Knight::Knight(Color c, int r, int co) : Piece(c, r, co) {}

char Knight::getSymbol() const {
    // 'N' because 'K' is reserved for King
    return (color == WHITE) ? 'N' : 'n';
}

bool Knight::isValidMove(int toRow, int toCol, Board& board) const {
    int dr = abs(toRow - row);
    int dc = abs(toCol - col);

    // Must be (2,1) or (1,2) — the L-shapes
    if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2)))
        return false;

    // Cannot capture own piece
    return !board.isFriendly(toRow, toCol, color);
}


//  Bishop — Implementation
//  Rules:
//    - Any number of squares diagonally
//    - Cannot jump over other pieces

Bishop::Bishop(Color c, int r, int co) : Piece(c, r, co) {}

char Bishop::getSymbol() const {
    return (color == WHITE) ? 'B' : 'b';
}

bool Bishop::isValidMove(int toRow, int toCol, Board& board) const {
    int dr = abs(toRow - row);
    int dc = abs(toCol - col);

    // Must be diagonal: equal row and column distance
    if (dr != dc || dr == 0) return false;

    // Cannot land on friendly piece
    if (board.isFriendly(toRow, toCol, color)) return false;

    // Check all diagonal squares in between are empty
    int rowStep = (toRow > row) ? 1 : -1;
    int colStep = (toCol > col) ? 1 : -1;
    int r = row + rowStep, c = col + colStep;
    while (r != toRow) {
        if (!board.isEmpty(r, c)) return false;
        r += rowStep;
        c += colStep;
    }

    return true;
}


//  Queen — Implementation
//  Rules:
//    - Combination of Rook + Bishop
//    - Any number of squares: straight OR diagonal
//    - Cannot jump over other pieces

Queen::Queen(Color c, int r, int co) : Piece(c, r, co) {}

char Queen::getSymbol() const {
    return (color == WHITE) ? 'Q' : 'q';
}

bool Queen::isValidMove(int toRow, int toCol, Board& board) const {
    int dr = abs(toRow - row);
    int dc = abs(toCol - col);

    bool straightLine = (toRow == row || toCol == col); // like Rook
    bool diagonal     = (dr == dc && dr != 0);           // like Bishop

    if (!straightLine && !diagonal) return false;

    // Cannot land on friendly piece
    if (board.isFriendly(toRow, toCol, color)) return false;

    // Determine step direction (works for both straight and diagonal)
    int rowStep = 0, colStep = 0;
    if      (toRow > row) rowStep =  1;
    else if (toRow < row) rowStep = -1;
    if      (toCol > col) colStep =  1;
    else if (toCol < col) colStep = -1;

    // Walk from source to destination, check no piece in the way
    int r = row + rowStep, c = col + colStep;
    while (r != toRow || c != toCol) {
        if (!board.isEmpty(r, c)) return false;
        r += rowStep;
        c += colStep;
    }

    return true;
}


//  King — Implementation
//  Rules:
//    - Moves exactly 1 square in any direction
//    - Cannot move into check (enforced by Game class)

King::King(Color c, int r, int co) : Piece(c, r, co) {}

char King::getSymbol() const {
    return (color == WHITE) ? 'K' : 'k';
}

bool King::isValidMove(int toRow, int toCol, Board& board) const {
    int dr = abs(toRow - row);
    int dc = abs(toCol - col);

    // Must move exactly 1 square in any direction
    if (dr > 1 || dc > 1 || (dr == 0 && dc == 0)) return false;

    // Cannot capture own piece
    if (board.isFriendly(toRow, toCol, color)) return false;

    // NOTE: "cannot move into check" is validated in Game::play()
    // using simulate-move → isInCheck → undo approach

    return true;
}


//  Board — Implementations
// ============================================================

// Constructor: all squares start empty (nullptr)
Board::Board() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            grid[r][c] = nullptr;
}

// Destructor: free all dynamically allocated pieces
Board::~Board() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++) {
            delete grid[r][c];
            grid[r][c] = nullptr;
        }
}

void Board::setPiece(int row, int col, Piece* piece) {
    grid[row][col] = piece;
}

Piece* Board::getPiece(int row, int col) const {
    if (row < 0 || row > 7 || col < 0 || col > 7) return nullptr;
    return grid[row][col];
}

bool Board::isEmpty(int row, int col) const {
    return grid[row][col] == nullptr;
}

bool Board::isEnemy(int row, int col, Color myColor) const {
    if (isEmpty(row, col)) return false;
    return grid[row][col]->getColor() != myColor;
}

bool Board::isFriendly(int row, int col, Color myColor) const {
    if (isEmpty(row, col)) return false;
    return grid[row][col]->getColor() == myColor;
}

bool Board::inBounds(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// Display the board with column labels (a-h) and row numbers (1-8)
void Board::display() const {
    cout << "\n    a  b  c  d  e  f  g  h\n";
    cout << "   +---+---+---+---+---+---+---+---+\n";
    for (int r = 7; r >= 0; r--) {   // row 8 shown at top
        cout << " " << (r + 1) << " |";
        for (int c = 0; c < 8; c++) {
            if (grid[r][c] == nullptr)
                cout << " . |";  // empty square
            else
                cout << " " << grid[r][c]->getSymbol() << "|";
        }
        cout << " " << (r + 1) << "\n";
        cout << "   +---+---+---+---+---+---+---+---+\n";
    }
    cout << "    a  b  c  d  e  f  g  h\n\n";
}

// Find the King of a given color on the board
// Returns true and sets kingRow/kingCol if found
bool Board::findKing(Color color, int& kingRow, int& kingCol) const {
    char sym = (color == WHITE) ? 'K' : 'k';
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (grid[r][c] != nullptr && grid[r][c]->getSymbol() == sym) {
                kingRow = r;
                kingCol = c;
                return true;
            }
    return false; // King not found (captured = game over)
}

// Check if the King of the given color is currently under attack
bool Board::isInCheck(Color color) {
    int kingRow, kingCol;
    if (!findKing(color, kingRow, kingCol))
        return true; // King captured → treat as check

    // See if any enemy piece can move to the King's square
    Color enemy = (color == WHITE) ? BLACK : WHITE;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++) {
            Piece* p = grid[r][c];
            if (p != nullptr && p->getColor() == enemy)
                if (p->isValidMove(kingRow, kingCol, *this))
                    return true;
        }
    return false;
}

// Execute a move: returns the captured piece (or nullptr)
Piece* Board::executeMove(int fromRow, int fromCol, int toRow, int toCol) {
    Piece* captured     = grid[toRow][toCol];   // save what's there (may be nullptr)
    grid[toRow][toCol]   = grid[fromRow][fromCol]; // move piece to destination
    grid[fromRow][fromCol] = nullptr;              // vacate source square
    grid[toRow][toCol]->setPosition(toRow, toCol); // update piece's internal coords
    grid[toRow][toCol]->setHasMoved(true);         // mark as moved
    return captured;
}

// Undo a move (used after simulate-then-check)
void Board::undoMove(int fromRow, int fromCol, int toRow, int toCol, Piece* captured) {
    grid[fromRow][fromCol] = grid[toRow][toCol]; // put piece back
    grid[toRow][toCol]     = captured;           // restore captured piece
    grid[fromRow][fromCol]->setPosition(fromRow, fromCol); // restore coords
}


//  Game — Implementations

Game::Game() : currentTurn(WHITE), gameOver(false) {
    setupBoard();
}

// Place all 32 pieces in their starting positions
void Game::setupBoard() {
    // --- WHITE pieces (rows 0 and 1) ---
    board.setPiece(0, 0, new Rook(WHITE,   0, 0));
    board.setPiece(0, 1, new Knight(WHITE, 0, 1));
    board.setPiece(0, 2, new Bishop(WHITE, 0, 2));
    board.setPiece(0, 3, new Queen(WHITE,  0, 3));
    board.setPiece(0, 4, new King(WHITE,   0, 4));
    board.setPiece(0, 5, new Bishop(WHITE, 0, 5));
    board.setPiece(0, 6, new Knight(WHITE, 0, 6));
    board.setPiece(0, 7, new Rook(WHITE,   0, 7));
    for (int c = 0; c < 8; c++)
        board.setPiece(1, c, new Pawn(WHITE, 1, c));

    // --- BLACK pieces (rows 7 and 6) ---
    board.setPiece(7, 0, new Rook(BLACK,   7, 0));
    board.setPiece(7, 1, new Knight(BLACK, 7, 1));
    board.setPiece(7, 2, new Bishop(BLACK, 7, 2));
    board.setPiece(7, 3, new Queen(BLACK,  7, 3));
    board.setPiece(7, 4, new King(BLACK,   7, 4));
    board.setPiece(7, 5, new Bishop(BLACK, 7, 5));
    board.setPiece(7, 6, new Knight(BLACK, 7, 6));
    board.setPiece(7, 7, new Rook(BLACK,   7, 7));
    for (int c = 0; c < 8; c++)
        board.setPiece(6, c, new Pawn(BLACK, 6, c));
}

// Parse "e2 e4" style input into row/col integers
// Returns false if format is invalid
bool Game::parseInput(const string& input,int& fromRow, int& fromCol,int& toRow,   int& toCol) {
    if (input.size() < 5) return false;

    char fc = tolower(input[0]); // from-col: 'a'-'h'
    char fr = input[1];           // from-row: '1'-'8'
    char tc = tolower(input[3]); // to-col:   'a'-'h'
    char tr = input[4];           // to-row:   '1'-'8'

    if (fc < 'a' || fc > 'h') return false;
    if (tc < 'a' || tc > 'h') return false;
    if (fr < '1' || fr > '8') return false;
    if (tr < '1' || tr > '8') return false;

    fromCol = fc - 'a'; // 'a'→0, 'b'→1 ... 'h'→7
    fromRow = fr - '1'; // '1'→0, '2'→1 ... '8'→7
    toCol   = tc - 'a';
    toRow   = tr - '1';

    return true;
}

// Try every possible move for 'color'; return true if at least one legal move exists
bool Game::hasAnyLegalMove(Color color) {
    for (int fr = 0; fr < 8; fr++) {
        for (int fc = 0; fc < 8; fc++) {
            Piece* p = board.getPiece(fr, fc);
            if (p == nullptr || p->getColor() != color) continue;

            for (int tr = 0; tr < 8; tr++) {
                for (int tc = 0; tc < 8; tc++) {
                    if (!p->isValidMove(tr, tc, board)) continue;

                    // Simulate the move
                    Piece* captured = board.executeMove(fr, fc, tr, tc);
                    bool inCheck    = board.isInCheck(color);
                    board.undoMove(fr, fc, tr, tc, captured);

                    if (!inCheck) return true; // found at least one legal move
                }
            }
        }
    }
    return false;
}

// If a Pawn reached the last rank, auto-promote it to Queen
void Game::handlePawnPromotion(int row, int col) {
    Piece* p = board.getPiece(row, col);
    if (p == nullptr) return;

    char sym = p->getSymbol();
    if ((sym == 'P' && row == 7) || (sym == 'p' && row == 0)) {
        Color c = p->getColor();
        delete p; // free the pawn
        board.setPiece(row, col, new Queen(c, row, col)); // promote to Queen
        cout << "  *** Pawn promoted to Queen! ***\n";
    }
}

// Main game loop
void Game::play() {
    cout << "\n==============================\n";
    cout << "   CHESS GAME - OOP in C++   \n";
    cout << "==============================\n";
    cout << "Symbols : White = UPPERCASE  |  Black = lowercase\n";
    cout << "Pieces  : K=King  Q=Queen  R=Rook  B=Bishop  N=Knight  P=Pawn\n";
    cout << "Input   : Enter moves like 'e2 e4'  |  Type 'quit' to exit\n\n";

    while (!gameOver) {
        board.display();

        string colorName = (currentTurn == WHITE) ? "WHITE" : "BLACK";

        // --- Check / Checkmate / Stalemate detection ---
        if (board.isInCheck(currentTurn)) {
            if (!hasAnyLegalMove(currentTurn)) {
                cout << "  *** CHECKMATE! " << colorName << " has no legal moves! ***\n";
                string winner = (currentTurn == WHITE) ? "BLACK" : "WHITE";
                cout << "  *** " << winner << " WINS! Congratulations! ***\n\n";
                gameOver = true;
                break;
            }
            cout << "  *** CHECK! " << colorName << "'s King is under attack! ***\n";
        } else {
            if (!hasAnyLegalMove(currentTurn)) {
                cout << "  *** STALEMATE! " << colorName << " has no legal moves. It's a DRAW! ***\n";
                gameOver = true;
                break;
            }
        }

        // --- Get input ---
        cout << colorName << "'s turn. Enter move (e.g. e2 e4): ";
        string input;
        getline(cin, input);

        if (input == "quit" || input == "exit") {
            cout << "  Game ended by player.\n";
            break;
        }

        // --- Parse ---
        int fromRow, fromCol, toRow, toCol;
        if (!parseInput(input, fromRow, fromCol, toRow, toCol)) {
            cout << "  Invalid format! Use: 'e2 e4'  (column a-h, row 1-8)\n";
            continue;
        }

        // --- Source square must have a piece ---
        Piece* movingPiece = board.getPiece(fromRow, fromCol);
        if (movingPiece == nullptr) {
            cout << "  No piece at that square. Try again.\n";
            continue;
        }

        // --- Must be current player's piece ---
        if (movingPiece->getColor() != currentTurn) {
            cout << "  That is not your piece! Try again.\n";
            continue;
        }

        // --- Piece movement rule check ---
        if (!movingPiece->isValidMove(toRow, toCol, board)) {
            cout << "  Invalid move for this piece. Try again.\n";
            continue;
        }

        // --- Simulate move; reject if it leaves own King in check ---
        Piece* captured = board.executeMove(fromRow, fromCol, toRow, toCol);
        if (board.isInCheck(currentTurn)) {
            board.undoMove(fromRow, fromCol, toRow, toCol, captured);
            cout << "  That move leaves your King in check! Try again.\n";
            continue;
        }

        // --- Move accepted ---
        if (captured != nullptr) {
            cout << "  Captured: " << captured->getSymbol() << "\n";
            delete captured; // free memory
        }

        // --- Pawn promotion check ---
        handlePawnPromotion(toRow, toCol);

        // --- Switch turn ---
        currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
    }

    cout << "\nThanks for playing\n";
}
