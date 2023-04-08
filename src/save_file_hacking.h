#ifndef SAVE_FILE_HACKING_H
#define SAVE_FILE_HACKING_H

#include <fstream>
#include <bitset>

#include "identify_scene.h"
//This file was provided by lab instructors. However, it can be considered as the feature of this game.

#define PADDING 500 // bytes
#define NAMESIZE 50
#define PASSSIZE 50
#define BOARDSIZE 999
#define URLSIZE 100

struct State {                           //Representing a board state
    int p, q;                           // Size of the board game
    int p_, q_;                         // Current cursor position
    char board[BOARDSIZE];              // Current board state
    char file_background[URLSIZE];      // Link to background file. This variable’s value is NULL if there is no current background
    // 500 byte NULL
};

struct Date {
    int dd, mm, yy;
};

struct Record {
    Date date;                          // Date of completed record
    int points;                         // points achieved
    // 500 byte NULL
};

struct savefile{
    char mask;                          // You are required to transfer all char-type variables by performing xor each with the mask-variable, bit-by-bit.
    char name[NAMESIZE];                // username
    char password[PASSSIZE];            // password
    // 500 byte NULL

    Record record[5]; // List of sorted best records
    State state[5]; // List of save state
};

#endif // SAVE_FILE_HACKING_H
