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

bool checkWinner(int **field, int width, int height, int numberToWin) {
    int vert = 1, horiz = 1, x1 = 0, x2 = 0;

    for (int i = 0; i < height - 1; ++i) {
      for (int j = 0; j < width - 1; ++j) {
        if (field[i][j] == field[i][j + 1]) {
          horiz++;
        } 
      }
      if (horiz == numberToWin) {
              return true;
            } 
      horiz = 1;
    }
        
          // vertical lines
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
    }

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

int main() {
  int width, height, numberToWin;
  


  std::cin >> width >> height;
  std::cin >> numberToWin;
  std::cout << "\n";

  int** field = new int* [height];   
  for (int i = 0 ; i < height; i++) {
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

      if (checkWinner(field, width, height, numberToWin)) {
        break;
      }

      currentPlayer = !currentPlayer;
    }

    draw(field, x, y, width, height);
  }

  endwin();

  if (checkWinner(field, width, height, numberToWin)) {
    std::cout << "Win: " << getPlayer(currentPlayer) << "\n";
  }
  for (int i = 0; i < width; i++) {
    delete []field[i];        
  }
  delete []field;

  return 0;
}