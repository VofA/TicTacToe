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
          const int &height) {
  clear();

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      std::string out;
      if (coordX == x && coordY == y) {
        out = "[" + getPlayer(field[y][x]) + "]";
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

int main() {
  int width, height;
  int numberToWin;
  int numberOfMoves = 0;

  std::cin >> width >> height;
  std::cin >> numberToWin;
  std::cout << "\n";

  std::vector<std::vector<int>> field;
  field.resize(height);

  for (int i = 0; i < height; ++i) {
    field[i].resize(width);
  }

  initscr();
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      field[y][x] = 3 * y + x + 2;
    }
  }
  draw(field, 1, 1, width, height);

  bool currentPlayer = PLAYER_X;

  int x = 1, y = 1;

  while (true) {
    char key = getch();
    clear();

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

    if (x + relativeX >= 0 && x + relativeX <= width - 1 &&
        y + relativeY >= 0 && y + relativeY <= height - 1) {
      y += relativeY;
      x += relativeX;
    }

    if (enter) {
      if (field[y][x] < 2) {
        addstr("Already taken\n");
        continue;
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
    }

    draw(field, x, y, width, height);
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