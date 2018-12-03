#include <iostream>
#include <ncurses.h>
#include <string>



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
void draw(int **field, int coordX, int coordY, int width, int height) {
  // clear();

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

bool checkWinner(int **field,
                 const int &width,
                 const int &height,
                 const int &n,
                 int cx,
                 int cy) {

  // vert
  std::string out  = "start = " + std::to_string(cy + (n - 1)) + "\n";
  out  += "end = " + std::to_string(cy + (n - 1)) + "\n";
  out  += "sb = " + std::to_string(nilIfNegative(cy - (n - 1))) + "\n";
  out  += "eb = " + std::to_string(numberIfMax(cy + (n - 1), height - 1) - n + 1) + "\n";
    addstr(out.c_str());
  for (int y = nilIfNegative(cy - (n - 1));
       y <= numberIfMax(cy + (n - 1), height - 1) - n + 1;
       ++y) { //(1+2*(n-1))
    int checkElement = field[y][cx];
    out  = "y = " + std::to_string(y) + "\n";
    addstr(out.c_str());

    bool result = true;
    for (int c = 1; c < n; ++c) {
      out = "c = " + std::to_string(c) + "";
      addstr(out.c_str());
      if (field[y + c][cx] != checkElement) {
        result = false;
        break;
      }
      out = "y+c = " + std::to_string(c + y) + "";
      addstr(out.c_str());
    }
    out = "r = " + std::to_string(result) + "\n";
    addstr(out.c_str());
    if (result) {
      return true;
    }
  }

  /*      // vertical lines
  for (int i = 0; i < height - 1; ++i) {
    for (int j = 0; j < width - 1; ++j) {
      if (field[j][i] == field[j + 1][i]) {
        vert++;
      }
    }
    if (vert == numberToWin ) {
            return true;
          }
    vert = 1;
  } */

  // x
  /* if (field[i][j] == field[i + 1][j + 1]) {
     x1++;
   } else {
     x1 = 0;
   }
   if (field[i][width - 2 - j] == field[i + i][width - 3 - j]) {
     x2++;
   } else {
     x2 = 0;
   } */
  return false;
}

bool checkDraw(int numberOfMoves, int width, int height) {
  return (width * height == numberOfMoves);
}

int main() {
  int width, height;
  int numberToWin;
  int numberOfMoves = 0;

  std::cin >> width >> height;
  std::cin >> numberToWin;
  std::cout << "\n";

  int **field = new int *[height];
  for (int i = 0; i < height; i++) {
    field[i] = new int[width];
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
  for (int i = 0; i < width; i++) {
    delete[] field[i];
  }
  delete[] field;

  return 0;
}