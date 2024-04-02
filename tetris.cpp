#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <ctime>
#include "tetris.h"
#include "random.h"

using namespace Random;

void addFigureToBoard();
void adjustShadow();
void boardPrinter();
std::vector<std::vector<block*>> boardSetup();
void figureToShadow();
bool isGameOver();
void lowerFigure();
figure* newFigure();
bool removeCompleteRows();
void rotateFigure();
void userInput();

bool gameOver;
int score{};
int level{-1};
char figureMove{'0'};
double timeBetweenCells;
std::vector<std::vector<block*>> board;
figure* fallingFigure;
clock_t lastTimeBlockLowered = std::clock();

int main(){
    std::cout << 
"Welcome to my tetris game, uhh, rules can be looked up online." << std::endl <<
"The controls for this game will just be:" << std::endl <<
"'d' to drop a figure to as low as it can go," << std::endl <<
"'f' to rotate a figure clockwise," << std::endl <<
"'j' to shift the figure to the left, and" << std::endl <<
"'k' to shift the figure to the right." << std::endl << std::endl;

std::cout << "To start, just choose a level (a number between 1 and 15 inclusive): ";
    while(level < 1 || level > 15){
        std::cin >> level;
        if(level < 1 || level > 15){
            std::cout << "You did not enter one of the options, please try again: ";
        }
    }
    switch(level){
        case(1):
            timeBetweenCells = (1.0/60.0)/0.01667;
            break;
        case(2):
            timeBetweenCells = (1.0/60.0)/0.021017;
            break;
        case(3):
            timeBetweenCells = (1.0/60.0)/0.026977;
            break;
        case(4):
            timeBetweenCells = (1.0/60.0)/0.035256;
            break;
        case(5):
            timeBetweenCells = (1.0/60.0)/0.04693;
            break;
        case(6):
            timeBetweenCells = (1.0/60.0)/0.06361;
            break;
        case(7):
            timeBetweenCells = (1.0/60.0)/0.0879;
            break;
        case(8):
            timeBetweenCells = (1.0/60.0)/0.1236;
            break;
        case(9):
            timeBetweenCells = (1.0/60.0)/0.1775;
            break;
        case(10):
            timeBetweenCells = (1.0/60.0)/0.2598;
            break;
        case(11):
            timeBetweenCells = (1.0/60.0)/0.388;
            break;
        case(12):
            timeBetweenCells = (1.0/60.0)/0.59;
            break;
        case(13):
            timeBetweenCells = (1.0/60.0)/0.92;
            break;
        case(14):
            timeBetweenCells = (1.0/60.0)/1.46;
            break;
        case(15):
            timeBetweenCells = (1.0/60.0)/2.36;
            break;
    }
    std::cout << std::endl << 
    "Alright, press any character when you are ready to begin";
    system("stty raw");
    char start{};
    std::cin >> start;
    //set up game
    board = boardSetup();
    fallingFigure = newFigure();

    //start user input loop
    std::future<void> userInputFuture = std::async(userInput);

    //start game printing loop

    //game process
    while(!gameOver){
        if((double)(double(std::clock() - lastTimeBlockLowered)/CLOCKS_PER_SEC) >= timeBetweenCells){
            lowerFigure();
            lastTimeBlockLowered = std::clock();
            boardPrinter();
        }
        if(removeCompleteRows()){
            boardPrinter();
        }
        gameOver = isGameOver();
    }
    system("stty cooked");
    std::cout << std::endl << std::endl << "GGs gamer! Your score was " << score << "." << std::endl;

}

void addFigureToBoard(){
    board[fallingFigure->first->y][fallingFigure->first->x] = new block(fallingFigure->first->x, fallingFigure->first->y, fallingFigure->figureColor);
    board[fallingFigure->second->y][fallingFigure->second->x] = new block(fallingFigure->second->x, fallingFigure->second->y, fallingFigure->figureColor);
    board[fallingFigure->third->y][fallingFigure->third->x] = new block(fallingFigure->third->x, fallingFigure->third->y, fallingFigure->figureColor);
    board[fallingFigure->fourth->y][fallingFigure->fourth->x] = new block(fallingFigure->fourth->x, fallingFigure->fourth->y, fallingFigure->figureColor);
    delete fallingFigure;
    fallingFigure = newFigure();
    return;
}

void adjustShadow(){
    fallingFigure->shadow->figureColor = color::black;
    fallingFigure->shadow->figureShape = fallingFigure->figureShape;
    fallingFigure->shadow->first->x = fallingFigure->first->x;
    fallingFigure->shadow->second->x = fallingFigure->second->x;
    fallingFigure->shadow->third->x = fallingFigure->third->x;
    fallingFigure->shadow->fourth->x = fallingFigure->fourth->x;
    int verticalShift{};
    while((verticalShift + fallingFigure->first->y < 24) && (verticalShift + fallingFigure->second->y < 24) && (verticalShift + fallingFigure->third->y < 24) && (verticalShift + fallingFigure->fourth->y < 24) && (!board[fallingFigure->first->y + verticalShift][fallingFigure->first->x]) && (!board[fallingFigure->second->y + verticalShift][fallingFigure->second->x]) && (!board[fallingFigure->third->y + verticalShift][fallingFigure->third->x]) && (!board[fallingFigure->fourth->y + verticalShift][fallingFigure->fourth->x])){
        ++verticalShift;
    }
    fallingFigure->shadow->first->y = fallingFigure->first->y + verticalShift - 1;
    fallingFigure->shadow->second->y = fallingFigure->second->y + verticalShift - 1;
    fallingFigure->shadow->third->y = fallingFigure->third->y + verticalShift - 1;
    fallingFigure->shadow->fourth->y = fallingFigure->fourth->y + verticalShift - 1;
    return;
}

void boardPrinter(){
    adjustShadow();
    system("stty cooked");
    int skipLine{};
    while(skipLine < 30){
        std::cout << std::endl;
        ++skipLine;
    }
    std::cout << "Score: " << score << std::endl << std::endl;
    int row{};
    int column{};
    while(row < 24){
        printf(WHITE);
        std::cout << "|";
        column = 0;
        while(column < 10){
            if(fallingFigure && (fallingFigure->first->x == column && fallingFigure->first->y == row) || (fallingFigure->second->x == column && fallingFigure->second->y == row) || (fallingFigure->third->x == column && fallingFigure->third->y == row) || (fallingFigure->fourth->x == column && fallingFigure->fourth->y == row)){
                switch(fallingFigure->figureColor){
                    case(color::blue):
                        printf(BLUE);
                        break;
                    case(color::yellow):
                        printf(YELLOW);
                        break;
                    case(color::red):
                        printf(RED);
                        break;
                    case(color::purple):
                        printf(PURPLE);
                        break;
                    case(color::green):
                        printf(GREEN);
                        break;
                    case(color::white):
                        printf(WHITE);
                        break;
                    case(color::cyan):
                        printf(CYAN);
                        break;
                }
                std::cout << "[]";
            }else{
                if(board[row][column]){
                    switch(board[row][column]->blockColor){
                        case(color::blue):
                            printf(BLUE);
                            break;
                        case(color::yellow):
                            printf(YELLOW);
                            break;
                        case(color::red):
                            printf(RED);
                            break;
                        case(color::purple):
                            printf(PURPLE);
                            break;
                        case(color::green):
                            printf(GREEN);
                            break;
                        case(color::white):
                            printf(WHITE);
                            break;
                        case(color::cyan):
                            printf(CYAN);
                            break;
                    }
                    std::cout << "[]";
                }else{
                    if(fallingFigure && (fallingFigure->shadow->first->x == column && fallingFigure->shadow->first->y == row) || (fallingFigure->shadow->second->x == column && fallingFigure->shadow->second->y == row) || (fallingFigure->shadow->third->x == column && fallingFigure->shadow->third->y == row) || (fallingFigure->shadow->fourth->x == column && fallingFigure->shadow->fourth->y == row)){
                        printf(BLACK);
                        std::cout << "[]";  
                    }else{
                        if(row == 3){
                            printf(WHITE);
                            std::cout << " .";
                        }else{
                            std::cout << "  ";
                        }
                    }
                }
            }
            ++column;
        }
        printf(WHITE);
        std::cout << "|" << std::endl;
        ++row;
    }
    printf(WHITE);
    column = 0;
    std::cout << "|";
    while(column < 10){
        std::cout << "@@";
        ++column;
    }
    std::cout << "|" << std::endl;
    system("stty raw");
    return;
}

std::vector<std::vector<block*>> boardSetup(){
    std::vector<std::vector<block*>> board;
    int iterator{};
    int iterator1{};
    while(iterator < 24){
        board.push_back({});
        iterator1 = 0;
        while(iterator1 < 10){
            board[iterator].push_back(nullptr);
            ++iterator1;
        }
        ++iterator;
    }
    return board;
}

void figureToShadow(){
    fallingFigure->first->x = fallingFigure->shadow->first->x;
    fallingFigure->first->y = fallingFigure->shadow->first->y;
    fallingFigure->second->x = fallingFigure->shadow->second->x;
    fallingFigure->second->y = fallingFigure->shadow->second->y;
    fallingFigure->third->x = fallingFigure->shadow->third->x;
    fallingFigure->third->y = fallingFigure->shadow->third->y;
    fallingFigure->fourth->x = fallingFigure->shadow->fourth->x;
    fallingFigure->fourth->y = fallingFigure->shadow->fourth->y;
    addFigureToBoard();
    return;
}

bool isGameOver(){
    int row{3};
    int column;
    while(row >= 0){
        column = 0;
        while(column < 10){
            if(board[row][column]){
                return 1;
            }
            ++column;
        }
        --row;
    }
    return 0;
}

void lowerFigure(){
    if(fallingFigure->first->y + 1 > 23 || fallingFigure->second->y + 1 > 23 || fallingFigure->third->y + 1 > 23 || fallingFigure->fourth->y + 1 > 23 || (board[fallingFigure->first->y + 1][fallingFigure->first->x]) || (board[fallingFigure->second->y + 1][fallingFigure->second->x]) || (board[fallingFigure->third->y + 1][fallingFigure->third->x]) || (board[fallingFigure->fourth->y + 1][fallingFigure->fourth->x])){
        addFigureToBoard();
        return;
    }else{
        fallingFigure->first->y += 1;
        fallingFigure->second->y += 1;
        fallingFigure->third->y += 1;
        fallingFigure->fourth->y += 1;
        return;
    }
}

figure* newFigure(){
    figure* fallingFigure = new figure;
    fallingFigure->figureShape = (shape)get(0, 6);
    switch(fallingFigure->figureShape){
        case(shape::i):
            fallingFigure->first->x = 4;
            fallingFigure->first->y = 0;
            fallingFigure->second->x = 4;
            fallingFigure->second->y = 1;
            fallingFigure->third->x = 4;
            fallingFigure->third->y = 2;
            fallingFigure->fourth->x = 4;
            fallingFigure->fourth->y = 3;
            fallingFigure->figureColor = color::blue;
            break;
        case(shape::j):
            fallingFigure->first->x = 5;
            fallingFigure->first->y = 0;
            fallingFigure->second->x = 5;
            fallingFigure->second->y = 1;
            fallingFigure->third->x = 5;
            fallingFigure->third->y = 2;
            fallingFigure->fourth->x = 4;
            fallingFigure->fourth->y = 2;
            fallingFigure->figureColor = color::green;
            break;
        case(shape::l):
            fallingFigure->first->x = 4;
            fallingFigure->first->y = 0;
            fallingFigure->second->x = 4;
            fallingFigure->second->y = 1;
            fallingFigure->third->x = 4;
            fallingFigure->third->y = 2;
            fallingFigure->fourth->x = 5;
            fallingFigure->fourth->y = 2;
            fallingFigure->figureColor = color::purple;
            break;
        case(shape::o):
            fallingFigure->first->x = 4;
            fallingFigure->first->y = 0;
            fallingFigure->second->x = 5;
            fallingFigure->second->y = 0;
            fallingFigure->third->x = 5;
            fallingFigure->third->y = 1;
            fallingFigure->fourth->x = 4;
            fallingFigure->fourth->y = 1;
            fallingFigure->figureColor = color::red;
            break;
        case(shape::s):
            fallingFigure->first->x = 6;
            fallingFigure->first->y = 1;
            fallingFigure->second->x = 5;
            fallingFigure->second->y = 1;
            fallingFigure->third->x = 5;
            fallingFigure->third->y = 2;
            fallingFigure->fourth->x = 4;
            fallingFigure->fourth->y = 2;
            fallingFigure->figureColor = color::white;
            break;
        case(shape::t):
            fallingFigure->first->x = 4;
            fallingFigure->first->y = 1;
            fallingFigure->second->x = 5;
            fallingFigure->second->y = 1;
            fallingFigure->third->x = 6;
            fallingFigure->third->y = 1;
            fallingFigure->fourth->x = 5;
            fallingFigure->fourth->y = 2;
            fallingFigure->figureColor = color::yellow;
            break;
        case(shape::z):
            fallingFigure->first->x = 4;
            fallingFigure->first->y = 1;
            fallingFigure->second->x = 5;
            fallingFigure->second->y = 1;
            fallingFigure->third->x = 5;
            fallingFigure->third->y = 2;
            fallingFigure->fourth->x = 6;
            fallingFigure->fourth->y = 2;
            fallingFigure->figureColor = color::cyan;
            break;
    }
    fallingFigure->shadow = new figure;
    fallingFigure->shadow->first = new block(0, 0, color::blue);
    fallingFigure->shadow->second = new block(0, 0, color::blue);
    fallingFigure->shadow->third = new block(0, 0, color::blue);
    fallingFigure->shadow->fourth = new block(0, 0, color::blue);
    return fallingFigure;
}

bool removeCompleteRows(){
    int completeRowsCount{};
    int row{23};
    int column{};
    bool deleteCurrentRow{};
    while(row > 0){
        column = 0;
        deleteCurrentRow = 0;
        while(column < 10){
            if(!board[row][column]){
                break;
            }else{
                if(column == 9){
                    deleteCurrentRow = 1;
                    break;
                }
                ++column;
            }
        }
        if(deleteCurrentRow){
            ++completeRowsCount;
            column = 0;
            //delete row
            while(column < 10){
                delete board[row][column];
                board[row][column] = nullptr;
                ++column;
            }
            //now collapse what was above it
            int tempRow{row};
            while(tempRow > 0){
                column = 0;
                while(column < 10){
                    board[tempRow][column] = board[tempRow - 1][column];
                    board[tempRow - 1][column] = nullptr;
                    ++column;
                }
                --tempRow;
            }
        }
        --row;
    }
    switch(completeRowsCount){
        case(0):
            break;
        case(1):
            score += 40 * (level);
            break;
        case(2):
            score += 100 * (level);
            break;
        case(3):
            score += 300 * (level);
            break;
        case(4):
            score += 1200 * (level);
            break;
    }
    if(completeRowsCount > 0){
        return 1;
    }else{
        return 0;
    }
}

void rotateFigure(){
    if(fallingFigure->figureShape == shape::o){
        return;
    }else{
        //new row is original column
        //new column is row/column max minus original row
        int newFirstx{(2 - (fallingFigure->first->y - fallingFigure->second->y + 1)) + (fallingFigure->second->x - 1)};
        int newFirsty{(fallingFigure->first->x - fallingFigure->second->x + 1) + fallingFigure->second->y - 1};
        int newSecondx{fallingFigure->second->x};
        int newSecondy{(fallingFigure->second->x - fallingFigure->second->x + 1) + fallingFigure->second->y - 1};
        int newThirdx{(2 - (fallingFigure->third->y - fallingFigure->second->y + 1)) + (fallingFigure->second->x - 1)};
        int newThirdy{(fallingFigure->third->x - fallingFigure->second->x + 1) + fallingFigure->second->y - 1};
        int newFourthx{(2 - (fallingFigure->fourth->y - fallingFigure->second->y + 1)) + (fallingFigure->second->x - 1)};
        int newFourthy{(fallingFigure->fourth->x - fallingFigure->second->x + 1) + fallingFigure->second->y - 1};

        int cap{};
        if(fallingFigure->figureShape == shape::i){
            cap = 2;
        }else{
            cap = 1;
        }
        int shift{0};
        bool plusOrMinus{1};
        while(shift <= cap){
            if(plusOrMinus){
                if(newFirstx + shift >= 0 && newFirstx + shift < 10 && newFirsty < 24 && newFirsty >= 0 && newSecondx + shift >= 0 && newSecondx + shift < 10 && newSecondy < 24 && newSecondy >= 0 && newThirdx + shift >= 0 && newThirdx + shift < 10 && newThirdy < 24 && newThirdy >= 0 && newFourthx + shift >= 0 && newFourthx + shift < 10 && newFourthy < 24 && newFourthy >= 0 && !board[newFirsty][newFirstx + shift] && !board[newSecondy][newSecondx + shift] && !board[newThirdy][newThirdx + shift] && !board[newFourthy][newFourthx + shift]){
                    break;
                }
                plusOrMinus = 0;
            }else{
                if(newFirstx - shift >= 0 && newFirstx - shift < 10 && newFirsty < 24 && newFirsty >= 0 && newSecondx - shift >= 0 && newSecondx - shift < 10 && newSecondy < 24 && newSecondy >= 0 && newThirdx - shift >= 0 && newThirdx - shift < 10 && newThirdy < 24 && newThirdy >= 0 && newFourthx - shift >= 0 && newFourthx - shift < 10 && newFourthy < 24 && newFourthy >= 0 && !board[newFirsty][newFirstx - shift] && !board[newSecondy][newSecondx - shift] && !board[newThirdy][newThirdx - shift] && !board[newFourthy][newFourthx - shift]){
                    break;
                }
                ++shift;
                plusOrMinus = 1;
            }
        }
        if(!(shift <= cap)){
            return;
        }
        if(!plusOrMinus){
            shift = -shift;
        }
        fallingFigure->first->x = newFirstx + shift;
        fallingFigure->first->y = newFirsty + shift;
        fallingFigure->second->x = newSecondx + shift;
        fallingFigure->second->y = newSecondy + shift;
        fallingFigure->third->x = newThirdx + shift;
        fallingFigure->third->y = newThirdy + shift;
        fallingFigure->fourth->x = newFourthx + shift;
        fallingFigure->fourth->y = newFourthy + shift;
    }
}

void shiftFigure(bool direction){
    int shift{};
    direction ? shift = 1 : shift = -1;
    if((board[fallingFigure->first->y][fallingFigure->first->x + shift]) || (board[fallingFigure->second->y][fallingFigure->second->x + shift]) || (board[fallingFigure->third->y][fallingFigure->third->x + shift]) || (board[fallingFigure->fourth->y][fallingFigure->fourth->x + shift]) || fallingFigure->first->x + shift < 0 || fallingFigure->first->x + shift > 9 || fallingFigure->second->x + shift < 0 || fallingFigure->second->x + shift > 9 || fallingFigure->third->x + shift < 0 || fallingFigure->third->x + shift > 9 || fallingFigure->fourth->x + shift < 0 || fallingFigure->fourth->x + shift > 9){
        return;
    }else{
        fallingFigure->first->x = fallingFigure->first->x + shift;
        fallingFigure->second->x = fallingFigure->second->x + shift;
        fallingFigure->third->x = fallingFigure->third->x + shift;
        fallingFigure->fourth->x = fallingFigure->fourth->x + shift;
        return;
    }
}

void userInput(){
    while(1){
        if(gameOver){
            return;
        }else{
            system("stty raw");
            std::cin >> figureMove; 
            switch(figureMove){
                case('d'):
                    figureToShadow();
                    break;
                case('D'):
                    figureToShadow();
                    break;
                case('f'):
                    rotateFigure();
                    break;
                case('F'):
                    rotateFigure();
                    break;
                case('j'):
                    shiftFigure(0);
                    break;
                case('J'):
                    shiftFigure(0);
                    break;
                case('k'):
                    shiftFigure(1);
                    break;
                case('K'):
                    shiftFigure(1);
                    break;
            }
            if(!gameOver){
                removeCompleteRows();
                boardPrinter();
            }
        }
    }
}
