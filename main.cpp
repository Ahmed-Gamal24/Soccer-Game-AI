#include "soccerPitch.h"
#include "SDL3/SDL.h"
#include <iostream>

using namespace std;

int main()
{

    // Display game start information
    cout << "=== SOCCER AI GAME ===" << endl;
    cout << "\nGame Rules:" << endl;
    cout << "- Two teams (Red and Blue) compete to score goals" << endl;
    cout << "- First team to score 3 goals wins the game" << endl;
    cout << "\nPlayer Behaviors:" << endl;
    cout << "- Players use steering behaviors (seek, arrive)" << endl;
    cout << "- State machine controls player actions" << endl;
    cout << "- Teams switch between Attacking and Defending based on ball possession" << endl;
    cout << "\nWatch the game unfold in the window!" << endl;
    cout << "\nStarting game...\n"
         << endl;

    clsSoccerPitch gameSpace;
    gameSpace.render();

    cout << "\n=== Game Ended ===" << endl;
    cout << "Final Score - Red: " << gameSpace.getRedScore()
         << " Blue: " << gameSpace.getBlueScore() << endl;

    return 0;
}
