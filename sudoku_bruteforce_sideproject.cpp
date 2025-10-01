#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

char S[9][9]; //S stand for Sudoku, so r:Row, c:Column
int x[81], y[81]; //x:Row, y:Column //used to store the coordinates of empty cells
bool tick[81][9]; // if cell i has checked number j+1
int emptyCells = 0;
int stepCount = 0;

void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void render() {
    cout <<S[0][0]<<' '<<S[0][1]<<' ' <<S[0][2]<<' '<<'|'<<' '<<S[0][3]<<' '<<S[0][4]<<' '<<S[0][5]<<' '<<'|'<<' '<<S[0][6]<<' '<<S[0][7]<<' '<<S[0][8]<<endl;
    cout <<S[1][0]<<' '<<S[1][1]<< ' '<<S[1][2]<<' '<<'|'<<' '<<S[1][3]<<' '<<S[1][4]<<' '<<S[1][5]<<' '<<'|'<<' '<<S[1][6]<<' '<<S[1][7]<<' '<<S[1][8]<<endl;
    cout <<S[2][0]<<' '<<S[2][1]<< ' '<<S[2][2]<<' '<<'|'<<' '<<S[2][3]<<' '<<S[2][4]<<' '<<S[2][5]<<' '<<'|'<<' '<<S[2][6]<<' '<<S[2][7]<<' '<<S[2][8]<<endl;
    cout <<"---------------------"<<endl;
    cout <<S[3][0]<<' '<<S[3][1]<<' ' <<S[3][2]<<' '<<'|'<<' '<<S[3][3]<<' '<<S[3][4]<<' '<<S[3][5]<<' '<<'|'<<' '<<S[3][6]<<' '<<S[3][7]<<' '<<S[3][8]<<endl;
    cout <<S[4][0]<<' '<<S[4][1]<< ' '<<S[4][2]<<' '<<'|'<<' '<<S[4][3]<<' '<<S[4][4]<<' '<<S[4][5]<<' '<<'|'<<' '<<S[4][6]<<' '<<S[4][7]<<' '<<S[4][8]<<endl;
    cout <<S[5][0]<<' '<<S[5][1]<< ' '<<S[5][2]<<' '<<'|'<<' '<<S[5][3]<<' '<<S[5][4]<<' '<<S[5][5]<<' '<<'|'<<' '<<S[5][6]<<' '<<S[5][7]<<' '<<S[5][8]<<endl;
    cout <<"---------------------"<<endl;
    cout <<S[6][0]<<' '<<S[6][1]<<' ' <<S[6][2]<<' '<<'|'<<' '<<S[6][3]<<' '<<S[6][4]<<' '<<S[6][5]<<' '<<'|'<<' '<<S[6][6]<<' '<<S[6][7]<<' '<<S[6][8]<<endl;
    cout <<S[7][0]<<' '<<S[7][1]<< ' '<<S[7][2]<<' '<<'|'<<' '<<S[7][3]<<' '<<S[7][4]<<' '<<S[7][5]<<' '<<'|'<<' '<<S[7][6]<<' '<<S[7][7]<<' '<<S[7][8]<<endl;
    cout <<S[8][0]<<' '<<S[8][1]<< ' '<<S[8][2]<<' '<<'|'<<' '<<S[8][3]<<' '<<S[8][4]<<' '<<S[8][5]<<' '<<'|'<<' '<<S[8][6]<<' '<<S[8][7]<<' '<<S[8][8]<<endl;
    cout << endl;
    cout << "Step count: " << stepCount++ << endl;

}

void render_to_output() {
    ofstream fo; fo.open("out_SUDOKU.txt");
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            fo << S[r][c] << ' ';
        }
        fo << endl;
    }
    fo.close();
}

bool check(int currentx, int currenty) {
    int num = S[currentx][currenty];
    //check row
    for (int c = 0; c < 9; c++) {
        if (c != currenty && S[currentx][c] == num) return false;
    }
    //check column
    for (int r = 0; r < 9; r++) {
        if (r != currentx && S[r][currenty] == num) return false;
    }
    //check 3x3 box
    int boxStartX = (currentx / 3) * 3;
    int boxStartY = (currenty / 3) * 3;
    for (int r = boxStartX; r < boxStartX + 3; r++) {
        for (int c = boxStartY; c < boxStartY + 3; c++) {
            if ((r != currentx && c != currenty) && S[r][c] == num) return false;
        }
    }
    return true;
}

bool BF_recursion(int current) {

    //for the purpose of visualization
    render();
    sleep(50);
    system("cls");

    if (current == emptyCells) return true; //base case: all empty cells are filled correctly
    int r = x[current];
    int c = y[current];

    for (int num = 1; num <= 9; num++) {
        if (!tick[current][num-1]) { //if number num has not been checked for current cell
            S[r][c] = '0' + num; //try to fill current cell with number num
            tick[current][num-1] = true; //mark number num as checked for current cell
            if (check(r, c)) { //if filling current cell with number num is valid
                if (BF_recursion(current + 1)) return true; //recurse to next empty cell
            }
            S[r][c] = '.'; //reset current cell if filling with number num is not valid or if recursion did not lead to a solution
        }
    }

    //reset tick for current cell before backtracking (if all numbers have been tried and none worked) 
    for (int i = 0; i < 9; i++) tick[current][i] = false;

    return false; //backtrack
}



int main() {
    ifstream fi; fi.open("SUDOKU.txt");
    string line;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            fi >> S[r][c];
            if (S[r][c] == '.') {
                x[emptyCells] = r;
                y[emptyCells] = c;
                emptyCells++;
            }
        }
    }

    render();
    system("pause");
    
    system("cls");
    BF_recursion(0);
    render();
    system("pause");

    render_to_output(); //system("pause");

    fi.close();
    return 0;
}
