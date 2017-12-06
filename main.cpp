#include <iostream>
#include <windows.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
class Game;
class Player;
class Ship;
class Board;
class ShadowBoard;
class Coordinate;

#define BOARD_SIZE 7



int main(int argc, char** argv) {
	std::string player1Name, player2Name;
	bool gameOver = false;
	
	
	std::cout << "BATTLESHIP\n";
	std::cout << "Insert player1 name:\n";
	std::cin >> player1Name;
	std::cout << "Insert player2 name:\n";
	std::cin >> player2Name;
	
	//DEFINE TWO PLAYERS AND ASSIGN TO GAME
	std::cout << "DEFINE TWO PLAYERS AND ASSIGN TO GAME\n";
	
	//BUILD BOARD AND ASSIGN TO GAME
	std::cout << "BUILD BOARD AND ASSIGN TO GAME\n";
	
	//DEFINE TWO SHADOW BOARD AND ASSIGN TO PLAYERS
	std::cout << "DEFINE TWO SHADOW BOARD AND ASSIGN TO PLAYERS\n";
	
	//DEFINE SHIPS POSITION OF TWO PLAYERS AND ASSIGN THEM TO PLAYERS
	std::cout << "DEFINE SHIPS POSITION OF TWO PLAYERS AND ASSIGN THEM TO PLAYERS\n";
	
	
	std::cout << "GAME START!!!" << std::endl;
	std::cout << player1Name << " vs " << player2Name << std::endl;
	
	int i=0;	
	//GAME LOOP ENDS WHEN ONE PLAYER DESTROY ALL SHIPS OF OTHER ONES
	while(!gameOver){
		std::cout << player1Name << " game: " << i << std::endl;
		Sleep(1000);
		std::cout << player2Name << " game: " << i << std::endl;
		Sleep(1000);
		i++;
		if(i>2)
			gameOver=true;
	}
	
	std::cout << player1Name << " WIN!!!" << std::endl;
}
