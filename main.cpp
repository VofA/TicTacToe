#include <iostream>
#include <ncurses.h>
#include <string>

int field[3][3];

const int PLAYER_X = 1;
const int PLAYER_O = 0;
std::string getPlayer(int player) {
  switch (player) {
    case PLAYER_X:
      return "x";
    case PLAYER_O:
      return "o";
    default:
      return " ";
  }
}

void draw(int coordX, int coordY) {
  clear();

  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      std::string out;
      if (coordX == x && coordY == y) {
        out = "[" + getPlayer(field[y][x]) + "]";
      } else {
        out = " " + getPlayer(field[y][x]) + " ";
      }
      addstr(out.c_str());

      if (x != 2) {
        addstr("|");
      }
    }
    addstr("\n");
  }
}

bool checkWinner() {
  return ((field[0][0] == field[0][1] && field[0][1] == field[0][2]) ||
          (field[1][0] == field[1][1] && field[1][1] == field[1][2]) ||
          (field[2][0] == field[2][1] && field[2][1] == field[2][2]) ||

          // vertical lines
          (field[0][0] == field[1][0] && field[1][0] == field[2][0]) ||
          (field[0][1] == field[1][1] && field[1][1] == field[2][1]) ||
          (field[0][2] == field[1][2] && field[1][2] == field[2][2]) ||

          // x
          (field[0][0] == field[1][1] && field[1][1] == field[2][2]) ||
          (field[2][0] == field[1][1] && field[1][1] == field[0][2]));
}

int main() {
  initscr();

  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      field[y][x] = 3 * y + x + 2;
    }
  }
  draw(1, 1);

  bool currentPlayer = PLAYER_X;

  int x = 1, y = 1;

  while (true) {
    char key = getch();

    int relativeX = 0;
    int relativeY = 0;

    bool enter = false;

    switch (key) {
      case 'w':
        relativeY--;
        break;
      case 'a':
        relativeX--;
        break;
      case 's':
        relativeY++;
        break;
      case 'd':
        relativeX++;
        break;
      case (char)10:
        enter = true;
        break;
    }

    if (x + relativeX >= 0 && x + relativeX <= 2 && y + relativeY >= 0 &&
        y + relativeY <= 2) {
      y += relativeY;
      x += relativeX;
    }

    if (enter) {
      if (field[y][x] < 2) {
        addstr("Already taken\n");
        continue;
      }

      field[y][x] = currentPlayer;

      if (checkWinner()) {
        break;
      }

      currentPlayer = !currentPlayer;
    }

    draw(x, y);
  }

  endwin();

  if (checkWinner()) {
    std::cout << "Win: " << getPlayer(currentPlayer) << "\n";
  }

  return 0;
}