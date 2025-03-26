#include <iostream>
#include <vector>
#include <cstdlib>   // For rand() and system("cls")
#include <windows.h> // For Sleep(), cursor manipulation
#include <conio.h>   // For kbhit() and getch() - Console IO
#include <ctime> // For time()
#include <algorithm> // For max()
using namespace std;

const int WIDTH = 10;
const int HEIGHT = 20;
const int BLOCK_SIZE = 2;
const int LEVEL_UP_LINES = 5;
const int BASE_DROP_SPEED = 200;
const int SPEED_INCREASE = 50;

const vector<vector<vector<int>>> SHAPES = {
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // I
    {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}},                         // J
    {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}},                         // L
    {{1, 1}, {1, 1}},                                          // O
    {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}},                         // S
    {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}},                         // T
    {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}                          // Z
};

HANDLE hConsole;

void hideCursor(){
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // Hide the cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void showCursor(){
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true; // Show the cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

class Tetris{
private:
    vector<vector<int>> board;
    vector<vector<int>> currentPiece;
    int currentX, currentY, currentShape;
    int score, level, linesCleared, dropSpeed;
    bool gameOver;
    bool paused;
    clock_t lastDropTime;

    void rotatePiece(){
        if(paused)return;
        
        vector<vector<int>> rotated(currentPiece[0].size(), vector<int>(currentPiece.size()));
        for(size_t i = 0; i < currentPiece.size(); ++i){
            for(size_t j = 0; j < currentPiece[0].size(); ++j){
                rotated[j][currentPiece.size() - 1 - i] = currentPiece[i][j];
            }
        }

        if(isValidPosition(rotated, currentX, currentY)){
            currentPiece = rotated;
        }
    }

    bool isValidPosition(const vector<vector<int>>& piece, int x, int y){
        for(size_t i = 0; i < piece.size(); ++i){
            for(size_t j = 0; j < piece[0].size(); ++j){
                if(piece[i][j] != 0){
                    int newX = x + j;
                    int newY = y + i;
                    
                    if(newX < 0 || newX >= WIDTH || newY >= HEIGHT){
                        return false;
                    }
                    
                    if(newY >= 0 && board[newY][newX] != 0){
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void placePiece(){
        for(size_t i = 0; i < currentPiece.size(); ++i) {
            for(size_t j = 0; j < currentPiece[0].size(); ++j){
                if(currentPiece[i][j] != 0){
                    int y = currentY + i;
                    int x = currentX + j;
                    if(y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH){
                        board[y][x] = currentShape + 1;
                    }
                }
            }
        }
    }

    void clearLines(){
        int linesClearedThisTurn = 0;
        for(int y = HEIGHT - 1; y >= 0; --y){
            bool lineComplete = true;
            for(int x = 0; x < WIDTH; ++x){
                if(board[y][x] == 0){
                    lineComplete = false;
                    break;
                }
            }
            
            if(lineComplete){
                linesClearedThisTurn++;
                for(int yy = y; yy > 0; --yy){
                    board[yy] = board[yy - 1];
                }
                board[0] = vector<int>(WIDTH, 0);
                y++;
            }
        }
        
        if(linesClearedThisTurn > 0){
            score += (linesClearedThisTurn * linesClearedThisTurn) * 100;
            linesCleared += linesClearedThisTurn;
            
            if(linesCleared >= LEVEL_UP_LINES){
                level++;
                linesCleared = 0;
                dropSpeed = max(10, dropSpeed - SPEED_INCREASE);
            }
        }
    }

    void newPiece(){
        currentShape = rand() % SHAPES.size();
        currentPiece = SHAPES[currentShape];
        currentX = WIDTH / 2 - currentPiece[0].size() / 2;
        currentY = -1;
        
        if(!isValidPosition(currentPiece, currentX, currentY)){
            gameOver = true;
        }
    }

    void hardDrop(){
        if(paused) return;
        
        while(isValidPosition(currentPiece, currentX, currentY + 1)){
            currentY++;
        }
        placePiece();
        clearLines();
        newPiece();
        lastDropTime = clock();
    }

    void softDrop(){
        if(paused) return;
        
        if(isValidPosition(currentPiece, currentX, currentY + 1)){
            currentY++;
        } else {
            placePiece();
            clearLines();
            newPiece();
        }
        lastDropTime = clock();
    }

    void movePiece(int dx){
        if(paused) return;
        
        if(isValidPosition(currentPiece, currentX + dx, currentY)){
            currentX += dx;
        }
    }

    void draw(){
        cout << "\033[H";

        for(int y = 0; y < HEIGHT; ++y){
            cout << "|";
            for(int x = 0; x < WIDTH; ++x){
                if(board[y][x] != 0){
                    SetConsoleTextAttribute(hConsole, board[y][x] + 7);
                    cout << "[]";
                    SetConsoleTextAttribute(hConsole, 7);
                } else {
                    cout << "  ";
                }
            }
            cout << "|" << endl;
        }
        
        cout << "+";
        for(int x = 0; x < WIDTH; ++x) cout << "--";
        cout << "+" << endl;
        
        if (!paused){
            for(size_t i = 0; i < currentPiece.size(); ++i){
                for(size_t j = 0; j < currentPiece[0].size(); ++j){
                    if(currentPiece[i][j] != 0){
                        int y = currentY + i;
                        int x = currentX + j;
                        if(y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH){
                            COORD coord = { (SHORT)(x * BLOCK_SIZE + 1), (SHORT)(y + 1) };
                            SetConsoleCursorPosition(hConsole, coord);
                            SetConsoleTextAttribute(hConsole, currentShape + 8);
                            cout << "[]";
                            SetConsoleTextAttribute(hConsole, 7);
                        }
                    }
                }
            }
        }
        
        COORD infoPos = { 0, (SHORT)(HEIGHT + 2) };
        SetConsoleCursorPosition(hConsole, infoPos);
        cout << "Score: " << score << "  Level: " << level << "  Lines: " << linesCleared << "/" << LEVEL_UP_LINES << endl;
        cout << "Press 'P' to pause, 'ESC' to Quit" << endl;
        
        if(paused){
            COORD pausePos = { (SHORT)(WIDTH-3), (SHORT)(HEIGHT/2-2) };
            SetConsoleCursorPosition(hConsole, pausePos);
            cout << "PAUSED - 'P'";
        }
    }

    void showStartScreen(){
        system("cls");
        cout << "================== TETRIS GAME ==================\n";
        cout << "Welcome to the classic Tetris game!\n";
        cout << "Control the falling tetrominoes using:\n";
        cout << "  <- Left Arrow  - Move Left\n";
        cout << "  -> Right Arrow - Move Right\n";
        cout << "  Up Arrow    - Rotate Piece\n";
        cout << "  Down Arrow  - Soft Drop\n";
        cout << "  SPACEBAR        - Hard Drop\n";
        cout << "  P               - Pause Game\n";
        cout << "  ESC             - Quit Game\n";
        cout << "Stack blocks, clear lines, and aim for a high score!\n";
        cout << "Press 'ENTER' key to start...\n";
        
        while(true){
            if(_kbhit()){
                int key = _getch();
                if (key == 13) break;
            }
            Sleep(100);
        }
        
        cout << "\n================= GAME STARTED! =================\n";
        Sleep(600);
    }

public:
    Tetris() : board(HEIGHT, vector<int>(WIDTH, 0)), 
               score(0), level(1), linesCleared(0), 
               dropSpeed(BASE_DROP_SPEED), gameOver(false),
               paused(false) {
        srand(static_cast<unsigned int>(time(nullptr)));
        newPiece();
        lastDropTime = clock();
    }

    void run(){
        hideCursor();
        showStartScreen();
        system("cls");
        
        while(!gameOver){
            draw();
            
            if(_kbhit()){
                int key = _getch();
                
                if(key == 224 || key == 0){
                    key = _getch();
                    if(!paused){
                        switch(key){
                            case 72: rotatePiece(); break;
                            case 75: movePiece(-1); break;
                            case 77: movePiece(1); break;
                            case 80: softDrop(); break;
                        }
                    }
                } 
                else{
                    switch(key){
                        case 32: if (!paused) hardDrop(); break;
                        case 27: gameOver = true; break;
                        case 'x':
                        case 'X': if (!paused) rotatePiece(); break;
                        case 'p':
                        case 'P': paused = !paused; break;
                    }
                }
            }
            
            if(!paused){
                clock_t currentTime = clock();
                if((currentTime - lastDropTime) * 1000 / CLOCKS_PER_SEC >= dropSpeed){
                    softDrop();
                    lastDropTime = currentTime;
                }
            }
            
            Sleep(45); 
        }
        
        system("cls");
        cout << "\n\n================== GAME OVER! ==================\n\n";
        cout << "Final Score: " << score << "\n";
        cout << "Level Reached: " << level << "\n";
        
        showCursor(); 
    }
};

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
    
    Tetris game;
    game.run();
    
    return 0;
}