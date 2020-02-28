//
//  main.cpp
//  Sudoku
//
//  Created by Haley Lai on 2/27/20.
//  Copyright © 2020 Haley Lai. All rights reserved.
//

#include <iostream>
#include "Sudoku.h"


int main(int argc, const char * argv[]) {
   srand(time(NULL));
   Sudoku game;
   game.generateBoard();
   game.solver();

   return 0;
}
