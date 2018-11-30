#include <iostream>
#include <string>

int a[3][3];
const int PLAYER_O = 0;
const int PLAYER_X = 1;

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
	std::cout << "  1 2 3 -> first\n";
	for (int y = 0; y < 3; ++y) {
		std::cout << y + 1 << " ";
		for (int x = 0; x < 3; ++x) {
			std::cout << getPlayer(a[y][x]) << "|";
		}
		std::cout << "\n";
	}
	std::cout << "y\n";
}

bool check() {
	if ( (a[0][0] == a[1][1] && a[1][1] == a[2][2]) ||
	     (a[0][2] == a[1][1] && a[1][1] == a[2][0]) ||
	     (a[1][0] == a[1][1] && a[1][1] == a[1][2]) ||
	     (a[0][1] == a[1][1] && a[1][1] == a[2][1]) ||
	     (a[0][0] == a[0][1] && a[0][1] == a[0][2]) ||
	     (a[0][0] == a[1][0] && a[1][0] == a[2][0]) ||
	     (a[2][0] == a[2][1] && a[1][1] == a[2][2]) ||
	     (a[2][2] == a[1][2] && a[1][2] == a[0][2])
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
	
	int current = 1;
	int x, y;
	for (int i = 0; i < 9; ++i) {
		std::cout << "Enter coordinates: ";
		std::cin >> x >> y;

		x--;
		y--;

		if (a[y][x] == PLAYER_O || a[y][x] == PLAYER_X) {
			std::cout << "Coordinates is busy!\n";
			i--;
			continue;
		}

		a[y][x] = current;

		draw();

		int result = check();
		if (result) {
			std::cout << "Winner is " << getPlayer(current) << ". ";
			break;
		}

		if (current == 1) {
			current = 0;
		} else {
			current = 1;
		}
	}

	return 0;
}