#include <iostream>

int field[3][3];

bool error(int x, int y) {
    if (x < 0 || x > 2 || y < 0 || y > 2) {
        std::cout << "Repeat enter\n";
        return true;
    } else {
        return false;
    }    
}

void draw() {
    
    for(int i = 0; i < 3; i++)
    {
        
        for(int j = 0; j < 3; j++)
        {
            if (field[i][j] != 1 && field[i][j] != 0) {
                std::cout << " ";
                if (j != 2) {
                    std::cout << "|";
                }
            } else {
                
                switch (field[i][j])
                {
                    case 0:
                        std::cout << "o";
                        break;
                       
                
                    default:
                        std::cout <<  "x";    
                        break;
                }
                if (j != 2) {
                    std::cout << "|";
                }
            }
        }
        std::cout << "\n";
    }
    
}

bool checkWinner() {

    if ((field[0][0] == field[0][1] && field[0][1] == field[0][2]) || 
        (field[1][0] == field[1][1] && field[1][1] == field[1][2]) || 
        (field[2][0] == field[2][1] && field[2][1] == field[2][2]) || 

        //vertical lines
        (field[0][0] == field[1][0] && field[1][0] == field[2][0]) || 
        (field[0][1] == field[1][1] && field[1][1] == field[2][1]) || 
        (field[0][2] == field[1][2] && field[1][2] == field[2][2]) ||
        
        //x
        (field[0][0] == field[1][1] && field[1][1] == field[2][2]) || 
        (field[2][0] == field[1][1] && field[1][1] == field[0][2])
        ) {
            return true;
        } else {
            return false;
        }

}

int main() {
    int x, y;
    const int PLAYER_1 = 1, PLAYER_2 = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            field[i][j] = 3 * i + j + 2;
        }
    }
    bool game = true, pl = true;

    while (game) {
        std::cin >> x >> y;
        if (error(x, y)) {
            continue;
        }
        if (field[x][y] == 1 || field[x][y] == 0) {
            std::cout << "Already taken\n";
            continue;
        }
        if (pl == true) {
            field[x][y] = PLAYER_1;
            draw();
            pl = false;
        } else {
            field[x][y] = PLAYER_2;
            draw();
            pl = true;
        }
        
        if (checkWinner()) {
            if (pl == false) {
                std::cout << "player 1 won\n";
                game = false;
            } else {
                std::cout << "player 2 won\n";
                game = false;
            }
        } 
        
    }

    return 0;
}