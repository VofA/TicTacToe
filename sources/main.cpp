#include "net.hpp"
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

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

int nilIfNegative(const int &number) {
  if (number < 0) {
    return 0;
  }
  return number;
}

int numberIfMax(const int &number, const int &max) {
  if (number > max) {
    return max;
  }
  return number;
}

void draw(const std::vector<std::vector<int>> &field,
          const int &coordX,
          const int &coordY,
          const int &width,
          const int &height,
          const bool &myTurn) {
  clear();

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
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

      if (x != width - 1) {
        addstr("|");
      }
    }
    addstr("\n");
  }

  refresh();
}

bool checkWinner(const std::vector<std::vector<int>> &field,
                 const int &width,
                 const int &height,
                 const int &n,
                 const int &cx,
                 const int &cy) {

  for (int y = nilIfNegative(cy - (n - 1));
       y <= numberIfMax(cy + (n - 1), height - 1) - n + 1;
       ++y) {
    int checkElement = field[y][cx];

    bool result = true;
    for (int c = 1; c < n; ++c) {

      if (field[y + c][cx] != checkElement) {
        result = false;
        break;
      }
    }
    if (result) {
      return true;
    }
  }
  for (int x = nilIfNegative(cx - (n - 1));
       x <= numberIfMax(cx + (n - 1), width - 1) - n + 1;
       ++x) {
    int checkElement = field[cy][x];

    bool result = true;
    for (int c = 1; c < n; ++c) {

      if (field[cy][x + c] != checkElement) {
        result = false;
        break;
      }
    }

    if (result) {
      return true;
    }
  }

  return false;
}

bool checkDraw(const int &numberOfMoves, const int &width, const int &height) {
  return (width * height == numberOfMoves);
}

int main(int argc, char *argv[]) {
  int width, height;
  int numberToWin;
  int numberOfMoves = 0;

  TTTNet *net;

  if (argc == 1) {
    net = new TTTNet();
  } else {
    net = new TTTNet(argv[1]);
  }

  if ((*net).isServer()) {
    std::cout << "Enter width, height, size: " << std::endl;
    std::cin >> width >> height;
    std::cin >> numberToWin;
    std::cout << std::endl;

    (*net).write("s" + std::to_string(width) + std::to_string(height));
  } else {
    boost::array<char, 128> buf;
    buf = (*net).read();

    width  = buf[1] - '0';
    height = buf[2] - '0';
  }

  std::vector<std::vector<int>> field;
  field.resize(height);

  for (int i = 0; i < height; ++i) {
    field[i].resize(width);
  }

  bool currentPlayer = PLAYER_X;
  bool myTurn        = (*net).isServer();
  int x = 1, y = 1;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      field[y][x] = 3 * y + x + 2;
    }
  }

  initscr();
  draw(field, x, y, width, height, myTurn);

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

        if (x + relativeX >= 0 && x + relativeX <= width - 1 &&
            y + relativeY >= 0 && y + relativeY <= height - 1) {
          y += relativeY;
          x += relativeX;
        }

        (*net).write("m" + std::to_string(x) + std::to_string(y));

        draw(field, x, y, width, height, myTurn);
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

        draw(field, x, y, width, height, myTurn);

      } while (buf[0] == 'm');

      x = buf[1] - '0';
      y = buf[2] - '0';
    }

    field[y][x] = currentPlayer;
    numberOfMoves++;

    if (checkWinner(field, width, height, numberToWin, x, y)) {
      break;
    }
    if (checkDraw(numberOfMoves, width, height)) {
      break;
    }

    currentPlayer = !currentPlayer;
    myTurn        = !myTurn;

    draw(field, x, y, width, height, myTurn);
  }

  endwin();

  if (checkWinner(field, width, height, numberToWin, x, y)) {
    std::cout << "Win: " << getPlayer(currentPlayer) << "\n";
  }
  if (checkDraw(numberOfMoves, width, height)) {
    std::cout << "it's a draw"
              << "\n";
  }

  return 0;
}