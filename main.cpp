#include <exception>
#include <string>
#include <iostream>
#include <SDL.h>


using namespace std;

#include "game.h"
int main(int argv, char** args) {

   Game game(750,640);
   game.game_main();
   return 0;
}
