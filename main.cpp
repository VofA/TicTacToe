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

void draw(int** field, int coordX, int coordY, int width, int height) {
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

bool checkWinner(int **field, int width, int height) {
  for (int i = 0; i < height - 2; ++i) {
    for (int z = 0; z < width - 2; ++z) {
      if ((field[i][z] == field[i][z + 1] && field[i][z] == field[i][z + 2]) ||
          (field[i + 1][z] == field[i + 1][z + 1] && field[i + 1][z] == field[i + 1][z + 2]) ||
          (field[i + 2][z] == field[i + 2][z + 1] && field[i + 2][z] == field[i][z + 2]) || 

          // vertical lines
          (field[i][z] == field[i + 1][z] && field[i + 1][z] == field[i + 2][z]) ||
          (field[i][z + 1] == field[i + 1][z + 1] && field[i + 1][z + 1] == field[i + 2][z + 1]) ||
          (field[i][z + 2] == field[i + 1][z + 2] && field[i + 1][z + 2] == field[i + 2][z + 2]) ||

          // x
          (field[i][z] == field[i + 1][z + 1] && field[i + 1][z + 1] == field[i + 2][z + 2]) ||
          (field[i + 2][z] == field[i + 1][z + 1] && field[i + 1][z + 1] == field[i][z + 2])) { 
            return true;
          } 
    }
  }
  return false;
}

int main() {
  
  int width, height;
  int** field = new int* [height];   
  for (int i = 0 ; i < height; i++) {
    field[i] = new int[width];  
  }


  std::cin >> width >> height;
  std::cout << "\n";

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

    if (x + relativeX >= 0 && x + relativeX <= width - 1 && y + relativeY >= 0 &&
        y + relativeY <= height - 1) {
      y += relativeY;
      x += relativeX;
    }

    if (enter) {
      if (field[y][x] < 2) {
        addstr("Already taken\n");
        continue;
      }

      field[y][x] = currentPlayer;

      if (checkWinner(field, width, height)) {
        break;
      }

      currentPlayer = !currentPlayer;
    }

    draw(field, x, y, width, height);
  }

  endwin();

  if (checkWinner(field, width, height)) {
    std::cout << "Win: " << getPlayer(currentPlayer) << "\n";
  }
  for (int i = 0; i < width; i++) {
    delete []field[i];        
  }
  delete []field;

  return 0;
}