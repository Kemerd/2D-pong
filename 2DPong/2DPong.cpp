// 2DPong.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

// Macros
typedef std::chrono::high_resolution_clock Clock;
#define print std::cout

// Essentially a 2D pong game, that speeds up as time goes on.
// You hit Q or E. Q swaps you to the left side, E to the right side.

int iPlayerPos = 0;
bool bLose = false;

// Our game line function.
class GameLine {

	private:

		double dPosition, dVelocity, dAcceleration, dImpulse  = 0.0; // Counted in m/s
		//static GameLine * currentInstance;
		
	public:

		double dMinPos = 0.0, dMaxPos = 10.0;

		// Our constructor, called upon class creation.
		// Pass: x is an integer defining the length of our line,
		// p will be our impulse; how fast acceleration increases.
		GameLine( int x = 10, int p = 1 ) {
			dMaxPos = x;
			dImpulse = p;
			//currentInstance = this;
		}

		// A function meant to be called once every ten milliseconds.
		void Tick() {
			dPosition += dVelocity / 100;
			dVelocity *= dAcceleration / 100;
			dAcceleration *= dImpulse / 100;
		}

		// Swaps the direction of our particle.
		void SwapDir() {
			dVelocity *= -1;
		}
		
		// A function return the position of our line.
		double GetPos() {
			return dPosition;
		}

		// A helper function to get the latest instance.
		//static GameLine& GetInstance() {
		//	return *currentInstance;
		//}

};


// Converts the line to ASCII, and spits it out.
void PrintLine( GameLine &game ) {

	int iParticlePos = std::round( game.GetPos() );

	// A lambda function to get rid of repeat code, but maintain scope.
	auto printMid = [ &game, iParticlePos ]() {
		for (int i = 0; i < game.dMaxPos; i++) {

			if (i == iParticlePos) {
				print << "O";
			}
			else {
				print << "-";
			}

		}
	};
	
	// Check if we have the left or right selected, print as follows.
	if (iPlayerPos == 0) {
		print << "[";
		printMid();
		print << ">\n";
	} else {
		print << "<";
		printMid();
		print << "]\n";
	}


}

// A function that runs our game logic.
void GameTick( GameLine &game ) {

	// Terminate this function upon loss.
	if (bLose) { std::terminate(); }

	// Sleep for 10 milliseconds on this thread.
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	PrintLine( game );
	game.Tick();

	// Check if we have a loss.
	if( ( game.GetPos() >= game.dMaxPos ) && ( iPlayerPos != 1 ) ) {
		bLose = true;
	}
	else if ( (game.GetPos() <= game.dMinPos) && (iPlayerPos != 0) ) {
		bLose = true;
	}
 
}

// This is a function that listens for input and sets our position based on that.
void InputListen() {

	// Terminate this function upon loss.
	if (bLose) { std::terminate(); }

	// Grab our input.
	char cInput;
	std::cin >> cInput;

	// Input cases.
	if (cInput == 'q') {
		iPlayerPos = 0;
		InputListen();
	}
	else if (cInput == 'e') {
		iPlayerPos = 1;
		InputListen();
	}

}

// A function to begin a new game.
void NewGame() {

	GameLine * MainGame = new GameLine(10);

	std::thread one(InputListen);

	while (!bLose) {
		std::thread two(GameTick, *MainGame); // Need to fix timing
	}

	delete MainGame;

	print << "You lose! Play again? ( Y / N )";

	char cInput;
	std::cin >> cInput;

	if (cInput == 'Y') {
		NewGame();
	}
	else {
		print << "Goodbye!";
		exit(0);
	}

}

// Our main function. Handles new instances of the game, and win/lose conditions.
int main() {

	NewGame();

}