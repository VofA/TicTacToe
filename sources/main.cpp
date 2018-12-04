#include "net.hpp"
#include <iostream>
#include <ncurses.h>
#include <string>

const int PLAYER_X = 1;
const int PLAYER_O = 0;
std::string getPlayer(const int &player) {
  switch (player) {
    case PLAYER_X:
      return "x";
    case PLAYER_O:
      return "o";
    default:
      return " ";
  }
}

void draw(const int (&field)[3][3],
          const int &coordX,
          const int &coordY,
          const bool &myTurn) {
  clear();

  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      std::string out;
      if (coordX == x && coordY == y) {
        if (myTurn) {
          out = "[" + getPlayer(field[y][x]) + "]";
        } else {
          out = "{" + getPlayer(field[y][x]) + "}";
        }
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
  if (!myTurn) {
    addstr("Wait your opponent's turn\n");
  }

  refresh();
}

bool checkWinner(const int (&field)[3][3]) {
  return ( // Horizontal lines
      (field[0][0] == field[0][1] && field[0][1] == field[0][2]) ||
      (field[1][0] == field[1][1] && field[1][1] == field[1][2]) ||
      (field[2][0] == field[2][1] && field[2][1] == field[2][2]) ||

      // Vertical lines
      (field[0][0] == field[1][0] && field[1][0] == field[2][0]) ||
      (field[0][1] == field[1][1] && field[1][1] == field[2][1]) ||
      (field[0][2] == field[1][2] && field[1][2] == field[2][2]) ||

      // X
      (field[0][0] == field[1][1] && field[1][1] == field[2][2]) ||
      (field[2][0] == field[1][1] && field[1][1] == field[0][2]));
}

int main(int argc, char *argv[]) {
  TTTNet *net;

  if (argc == 1) {
    net = new TTTNet();
  } else {
    net = new TTTNet(argv[1]);
  }

  int field[3][3];

  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      field[y][x] = 3 * y + x + 2;
    }
  }

  int x = 1, y = 1;
  bool currentPlayer = PLAYER_X;
  bool myTurn        = (*net).isServer();

  initscr();
  draw(field, x, y, myTurn);

  while (true) {
    if (myTurn) {
      bool enter = false;
      while (!enter) {
        char key = getch();

        int relativeX = 0;
        int relativeY = 0;

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

        (*net).write("m" + std::to_string(x) + std::to_string(y));

        draw(field, x, y, myTurn);
      }
      if (field[y][x] < 2) {
        addstr("Already taken\n");
        continue;
      }

      (*net).write("e" + std::to_string(x) + std::to_string(y));
    } else {
      boost::array<char, 128> buf;

      do {
        buf = (*net).read();

        x = buf[1] - '0';
        y = buf[2] - '0';

        draw(field, x, y, myTurn);

      } while (buf[0] == 'm');

      x = buf[1] - '0';
      y = buf[2] - '0';
    }

    field[y][x] = currentPlayer;

    if (checkWinner(field)) {
      break;
    }

    currentPlayer = !currentPlayer;
    myTurn        = !myTurn;

    draw(field, x, y, myTurn);
  }

  endwin();

  if (checkWinner(field)) {
    std::cout << "Win: " << getPlayer(currentPlayer) << "\n";
  }
  return 0;
}