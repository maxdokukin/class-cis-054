//
//  main.c
//  TICTACTOE
//
//  Created by Max Dokukin on 15/03/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>



//FUNCTIONS WITH BOARD//
void makeSelection(char board[3][3], char currentPlayer); //handles player's field selection
char checkWin(char board[3][3]); //check is there is a winner
int isAvailable(char board[3][3], int pos); //returns if selected spot on the board available


//I/O FUNCTIONS//
int inputInt(int allowedMinVal, int allowedMaxVal); //inputs int in restricted range
void printBoard(char board[3][3]); //prints board
void clearConsole(void); //clears output console
void printGameResult(char board[3][3], char winner); //prints game result
void printHeader(char turn); //prints header 


//PLAYER FUNCTIONS//
char swapPlayers(char currentPlayer); //swaps player from one to another
char getRandomPlayer(void); //return random player



int main(int argc, const char * argv[]) {
    
    srand((unsigned int) time(NULL));

    char currentPlayer = getRandomPlayer();
    char winner = 0;
    char board[3][3] = { {'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'} };

    //main loop
    for(int moveCount = 0; moveCount < 9 && !winner; moveCount ++){

        clearConsole();
        printHeader(currentPlayer);
        printBoard(board);

        makeSelection(board, currentPlayer);
        
        winner = checkWin(board);
        
        currentPlayer = swapPlayers(currentPlayer);
    }
    
    printGameResult(board, winner);

    return 0;
}



//FUNCTIONS WITH BOARD//
void makeSelection(char board[3][3], char currentPlayer){
        
    int playerChoice = -1;
    
    while(playerChoice == -1){
        
        printf("Player %c, make your choice : ", currentPlayer);

        playerChoice = inputInt(1, 9);
                
        if(!isAvailable(board, playerChoice - 1)){
            
            playerChoice = -1;
            printf("This one already checked!\n");
        }
    }
    
    *(*board + playerChoice - 1) = currentPlayer;
}


char checkWin(char board[3][3]){
    
    char horizontalKeyChar = 0, verticalKeyChar = 0;
    
    for(int i = 0; i < 3; i++){

        //horizontal rows
        horizontalKeyChar = board[i][0];
        if(board[i][1] == horizontalKeyChar && board[i][2] == horizontalKeyChar)
           return horizontalKeyChar;
        
        //vertical rows
        verticalKeyChar = board[0][i];
        if(board[1][i] == verticalKeyChar && board[2][i] == verticalKeyChar)
           return verticalKeyChar;
    }
    
    //diagonals
    horizontalKeyChar = board[1][1];
    
    if((board[0][0] == horizontalKeyChar && board[2][2] == horizontalKeyChar) ||
       (board[0][2] == horizontalKeyChar && board[2][0] == horizontalKeyChar))
        return horizontalKeyChar;
    
    return 0;
}


int isAvailable(char board[3][3], int pos){
    
    return *(*board + pos) != 'X' && *(*board + pos) != 'O';
}



//I/O FUNCTIONS//
int inputInt(int allowedMinVal, int allowedMaxVal){
    
    int inputVal = 0;
    
    int scanfCount = scanf("%d", &inputVal);
    fflush(stdin);
    
    if(scanfCount != 1){
        
        printf("Illegal entry\n");
        return -1;
    }
    
    if(inputVal < allowedMinVal || inputVal > allowedMaxVal){
        
        printf("Value not allowed!\n");
        return -1;
    }
    return inputVal;
}


void printBoard(char board[3][3]){

    printf("|-------|-------|-------|\n");
    printf("|       |       |       |\n");

    for(int i = 0; i < 3; i++){
        
        for(int j = 0; j < 3; j++)
            printf("|   %c   ", board[i][j]);
        
        printf("|\n|       |       |       |\n");
        printf("|-------|-------|-------|\n");
        if(i < 2)
            printf("|       |       |       |\n");

    }
}


void clearConsole(){
    
    for(int i = 0; i < 20; i++){
        
        printf("\n");
    }
}


void printGameResult(char board[3][3], char winner){
    
    clearConsole();

    if(winner)
        printf("-------------------------\n|         %1c won!        |\n-------------------------\n", winner);
    else
        printf("-------------------------\n|          Draw         |\n-------------------------\n");
    printBoard(board);
}


void printHeader(char turn){
    
    printf("-------------------------\n|         %1c turn        |\n-------------------------\n", turn);
}


//PLAYER FUNCTIONS//
char swapPlayers(char currentPlayer){
    
    return currentPlayer == 'X' ? 'O' : 'X';
}


char getRandomPlayer(){
        
    return rand() % 2 == 1 ? 'X' : 'O';
}
