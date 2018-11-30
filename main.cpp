#include <iostream>
#include <string>
#include <conio.h> //

int a[3][3];
const int PLAYER_O = 0;
const int PLAYER_X = 1;

int positionX = 0;
int positionY = 0;

int move = 0;

int current = PLAYER_X;

std::string getPlayer(int player) {
	switch (player) {
		case PLAYER_X: {
			return "X";
		}
		case PLAYER_O: {
			return "O";
		}
		default: {
			return " ";
		}
	}
}

void draw() {
	system("cls");
	std::cout << "   1   2   3 -> first\n";
	for (int y = 0; y < 3; ++y) {
		std::cout << y + 1 << " ";
		for (int x = 0; x < 3; ++x) {
			if (positionX == x && positionY == y) {
				std::cout  << "[" << getPlayer(a[y][x]) << "]";
			} else {
				std::cout  << " " << getPlayer(a[y][x]) << " ";
			}
			if (x != 2) {
				std::cout  << "|";
			}
		}
		std::cout << "\n";
	}
	std::cout << "second\n\nMove player: " << getPlayer(current) << "\n";
}

bool check() {
	if ( (a[0][0] == a[1][1] && a[1][1] == a[2][2]) || // main
	     (a[0][2] == a[1][1] && a[1][1] == a[2][0]) || // primary
			 
			 (a[0][0] == a[0][1] && a[0][1] == a[0][2]) || // |1
	     (a[1][0] == a[1][1] && a[1][1] == a[1][2]) || // |2
			 (a[2][0] == a[2][1] && a[2][1] == a[2][2]) || // |3
			 
			 (a[0][0] == a[1][0] && a[1][0] == a[2][0]) || // -1
			 (a[0][1] == a[1][1] && a[1][1] == a[2][1]) || // -2
	     (a[0][2] == a[1][2] && a[1][2] == a[2][2])    // -3
	) {
		return true;
	} else {
		return false;
	}
}

int main() {
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			a[y][x] = 10 * (y + 1) + x;
		}
	}

	draw();
	
	while (true) {
		int key = getch();
		
		int relativeX = 0;
		int relativeY = 0;

		bool enter = false;

		switch (key) {
			case (int)'w': {
				relativeY = -1;
				break;
			}
			case (int)'a': {
				relativeX = -1;
				break;
			}
			case (int)'s': {
				relativeY = 1;
				break;
			}
			case (int)'d': {
				relativeX = 1;
				break;
			}
			case 13: { // ENTER
				enter = true;
				break;
			}
			default: {
				continue;
			}
		}

		if (positionX + relativeX < 0 || positionX + relativeX > 2 ||
				positionY + relativeY < 0 || positionY + relativeY > 2) {
			continue;
		}

		positionX += relativeX;
		positionY += relativeY;

		if (enter) {
			if (a[positionY][positionX] == PLAYER_O || a[positionY][positionX] == PLAYER_X) {
				std::cout << "Coordinates is busy!\n";
				continue;
			}

			a[positionY][positionX] = current;

			int result = check();
			if (result) {
				draw();
				std::cout << "Winner is " << getPlayer(current) << ".\n";
				break;
			}
			
			move++;
			if (move > 8) {
				draw();
				std::cout << "No one win :(\n";
				break;
			}

			current = (current == 1) ? 0 : 1;
		}

		draw();
	}

	return 0;
}