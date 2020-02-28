//
//  Sudoku.h
//  Sudoku
//
//  Created by Haley Lai on 2/27/20.
//  Copyright © 2020 Haley Lai. All rights reserved.
//

#ifndef Sudoku_h
#define Sudoku_h

#include <random>

using namespace std;

class Sudoku {
private:
   static const int blockSize;
   vector<vector<int>> board;
   int numSolus;
   bool solver(int counts, int posX, int posY, int digit);
   void allSolu(vector<vector<int>> board, int counts, int posX, int posY, int digit);
   void randomFillBoard();
   bool legalPlace(vector<vector<int>> &board, int posX, int posY, int digit);
   int existNum ();

public:
   Sudoku();
   void solver();
   void generateBoard();
   void printBoard();
};

const int Sudoku::blockSize = 3;

bool Sudoku::solver(int counts, int posX, int posY, int digit) {
   if (counts == board.size() * board.size()) return true;

   if (counts == 0) counts = existNum();

   if (posY >= board.size()) {
      if (solver(counts, 0, 0, digit + 1)) return true;
      return false;
   }

   int limitY = posY + blockSize, limitX = posX + blockSize;
   int lastBlock = 2 * blockSize;
   for (int y = posY; y < limitY; y++) {
      for (int x = posX; x < limitX; x++) {
         bool exist = false;
         if (board[y][x] == digit) exist = true;
         else if (board[y][x] != 0 || !(legalPlace(board, x, y, digit))) continue;
         else board[y][x] = digit;
         if (x >= lastBlock) {
            if (exist) {
               if (solver(counts, 0, posY + blockSize, digit)) return true;
            }
            else if (solver(counts + 1, 0, posY + blockSize, digit)) return true;
            if (!exist) board[y][x] = 0;
         }
         else {
            if (exist) {
               if (solver(counts, posX + blockSize, posY, digit)) return true;
            }
            else if (solver(counts + 1, posX + blockSize, posY, digit)) return true;
            if (!exist) board[y][x] = 0;
         }
      }
   }
   return false;
}

void Sudoku::allSolu(vector<vector<int>> board, int counts, int posX, int posY, int digit) {
   if (counts == board.size() * board.size()) {
//      printBoard(board);
      numSolus++;
      return;
   }

   if (counts == 0) counts = existNum();

   if (posY >= board.size()) {
      allSolu(board, counts, 0, 0, digit + 1);
      return;
   }

   int limitY = posY + blockSize, limitX = posX + blockSize;
   int lastBlock = 2 * blockSize;
   for (int y = posY; y < limitY; y++) {
      for (int x = posX; x < limitX; x++) {
         bool exist = false;
         if (board[y][x] == digit) exist = true;
         else if (board[y][x] != 0 || !(legalPlace(board, x, y, digit))) continue;
         else board[y][x] = digit;
         if (x >= lastBlock) {
            if (exist) allSolu(board, counts, 0, posY + blockSize, digit);
            else allSolu(board, counts + 1, 0, posY + blockSize, digit);
            if (!exist) board[y][x] = 0;
         }
         else {
            if (exist) allSolu(board, counts, posX + blockSize, posY, digit);
            else allSolu(board, counts + 1, posX + blockSize, posY, digit);
            if (!exist) board[y][x] = 0;
         }
      }
   }
}

void Sudoku::randomFillBoard() {
   vector<int> number = {1, 2, 3, 4, 5, 6, 7, 8, 9};
   auto rng = default_random_engine (random_device{}());
   for (int init = 0; init < board.size(); init += blockSize) {
      shuffle(begin(number), end(number), rng);
      int index = 0, limit = init + blockSize;
      for (int i = init; i < limit; i++) {
         for (int j = init; j < limit; j++) {
            board[i][j] = number[index];
            index++;
         }
      }
   }
//   printBoard();
   solver(0, 0, 0, 1);
}

Sudoku::Sudoku() {
   numSolus = 0;
   board = {
      {0,0,0   ,0,0,0    ,0,0,0},
      {0,0,0   ,0,0,0    ,0,0,0},
      {0,0,0   ,0,0,0    ,0,0,0},

      {0,0,0   ,0,0,0    ,0,0,0},
      {0,0,0   ,0,0,0    ,0,0,0},
      {0,0,0   ,0,0,0    ,0,0,0},

      {0,0,0   ,0,0,0    ,0,0,0},
      {0,0,0   ,0,0,0    ,0,0,0},
      {0,0,0   ,0,0,0    ,0,0,0}
   };
}

void Sudoku::solver() {
   solver(0, 0, 0, 1);
   cout << "Solution: " << endl;
   printBoard();
}

void Sudoku::generateBoard() {
   randomFillBoard();

   int counts = board.size() * board.size();
   int digit = 0;
   while (counts > 20) {
      numSolus = 0;
      int y = rand() % board.size(), x = rand() % board.size();
      digit = board[y][x];
      board[y][x] = 0;
      allSolu(board, 0, 0, 0, 1);
      if (numSolus == 1) counts--;
      else board[y][x] = digit;
   }
   cout << "Board: " << endl;
   printBoard();
//   cout << "Solution: " << endl;
//   solver(board, 0, 0, 0, 1);
}

int Sudoku::existNum () {
   int counts = 0;
   for (int i = 0; i < board.size(); i++)
      for (int j =0; j < board.size(); j++)
         if (board[i][j] != 0) counts++;
   return counts;
}

bool Sudoku::legalPlace(vector<vector<int>> &board, int posX, int posY, int digit) {
//   horizontal detectation
   for (int x = 0; x < board.size(); x++)
      if (x != posX && board[posY][x] == digit) return false;

//   vertical detectation
   for (int y = 0; y < board.size(); y++)
      if (y != posY && board[y][posX] == digit) return false;

//   regional detectation
   int x = 0, y = 0, lastBlock = 2 * blockSize;
   if (posX >= lastBlock) x = lastBlock;
   else if (posX >= blockSize && posX < lastBlock) x = blockSize;

   if (posY >= lastBlock) y = lastBlock;
   else if (posY >= blockSize && posY < lastBlock) y = blockSize;

   int limitY = y + blockSize, limitX = x + blockSize;
   for (; y < limitY; y++)
      for (; x < limitX; x++)
         if ((x != posX || y != posY) && board[y][x] == digit) return false;
   return true;
}

void Sudoku::printBoard() {
   for (int i = 0; i < board.size(); i++) {
      for (int j = 0; j < board.size(); j++)
         cout << board[i][j] << " ";
      cout << endl;
   }
   cout << endl;
}

#endif /* Sudoku_h */
