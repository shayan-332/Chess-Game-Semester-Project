#ifndef CHESS_H
#define CHESS_H

#include<iostream>
#include<string>
#include<cctype>
#include<windows.h>
#define SIZE 8

using namespace std;

// Forward declaration
class board;

// Utility functions
void gotoxy(int x, int y);
void setupboard(board& b);
void showboard(board& b);

// Position class
class position
{
public:
    int row;
    int col;
    position();
    position(int r, int c);
    bool isvalid();
    bool issame(position p);
};

// Chess move class
class chessmove
{
public:
    position from;
    position to;
    chessmove();
    chessmove(position f, position t);
};

// Base piece class
class piece
{
public:
    char color;
    char type;
    bool hasmoved;
    chessmove moves[28];
    int movecount;

    piece(char c, char t);
    virtual void getpossiblemoves(position pos, board& b) = 0;
    virtual ~piece() {}
};

// Board class
class board
{
public:
    piece* grid[SIZE][SIZE];
    board();
    ~board();
    piece* getpiece(int r, int c);
    piece* getpiece(position p);
};

// Derived piece classes
class knight : public piece
{
public:
    knight(char c);
    void getpossiblemoves(position pos, board& b);
};

class rook : public piece
{
public:
    rook(char c);
    void getpossiblemoves(position pos, board& b);
};

class bishop : public piece
{
public:
    bishop(char c);
    void getpossiblemoves(position pos, board& b);
};

class queen : public piece
{
public:
    queen(char c);
    void getpossiblemoves(position pos, board& b);
};

class king : public piece
{
public:
    king(char c);
    void getpossiblemoves(position pos, board& b);
};

class pawn : public piece
{
public:
    pawn(char c);
    void getpossiblemoves(position pos, board& b);
};

// Move validator class
class movevalidator
{
public:
    position findking(char color, board& b);
    bool isincheck(char color, board& b);
    bool issafeaftermove(chessmove m, board& b);
    bool islegalmove(chessmove m, board& b);
    void applymove(chessmove m, board& b);
    bool haslegal(char color, board& b);
    bool ischeckmate(char color, board& b);
};

#endif