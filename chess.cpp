#include<iostream>
#include<cstring>
#include<conio.h>
#include<dos.h>
#include<Windows.h>
#include<fstream>
#include<iomanip>
#include<stdlib.h>
#include<ctime>
using namespace std;

//*******************************************|           GLOBAL VARIABLES            |***************************************
char turn = 'A';    //for turns

//for time couner
int rapidTimerA = 600, rapidTimerB = 600;
int blitzTimerA = 300, blitzTimerB = 300;
int classicTimerA = 0, classicTimerB = 0;
int hourA=0, minA=0, secA=0;
int hourB=0, minB=0, secB=0;

//for check on player White & Black
bool checkA = false, checkB = false;

//for storing previous moves position
int prevCoordinate[8] = { 9,9,9,9,9,9,9,9 };

//contain opposite pieces
char opposite[6];


int checkInd[100] = {};
int storeCheckPieceInd[100] = {}, foolmateA = 0, foolmateB = 0;

//for replay file
fstream saveFile1("save1.txt");
fstream saveFile2("save2.txt");
fstream saveFile3("save3.txt");

char saveMoves[10000] = {0};
int indOfSaveMoves = 0;

//for game modes
char mode = '\0';

//for draw(50 consecutive moves neither pawn move or any piece capture)
int consecutiveMoves = 0;
int countPreviousPieces = 32;

//user choice which file he want to open
char fileChoice = '\0';


//****************************************|           END OF GLOBAL VARIABLES           |***************************************

//Functions Header
void display(char val[8][8]);
int playerTurn(char val[8][8], bool& checkA, bool& checkB, int& countAK, int& countAr_1, int& countAr_2, int& countBk, int& countBr_1, int& countBr_2);
void checkDestination(char opposite[]);
void Movements(char board[8][8], int row, int column, int desRow, int desColumn, char opposite[]);
void Movements(char board[8][8], int row, int column, int desRow, int desColumn, char opposite[]);

void gotoxy(int x, int y) {

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD scrn;

	scrn.X = x, scrn.Y = y;

	SetConsoleCursorPosition(h, scrn);

}

//for colouring
void inRed(int word) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 4);
	if (word < 10 && word >= 0)
		cout << "0" << word;
	else
	cout << word;
	SetConsoleTextAttribute(h, 15);
}

//timer classic function
void classicTime(int &timer, int &hour ,int &min, int &sec) {
	clock_t start = clock();
	cout << "Time: " << endl;
	while (timer >= 0 && !_kbhit())
	{
		if (sec == 60 && min == 60)
		{
			min = 0;
			hour++;
		}
		if (sec == 60)
		{
			sec = 0;
			min++;
		}

		cout << hour << ":";
		cout << min << ":";
		if (sec < 10 && sec >= 0)
			cout << "0" << sec;
		else
		{
			cout << sec;
		}

		sec++;
		cout <<'\r';
		Sleep(1000);
		timer++;
		
	}
	cout << '\n';
}

//timer rapid function
void rapidTime(int &timer)
{
	int min, sec;
	clock_t start = clock();
	cout << "Time Remaining: " << endl;
	while (timer >= 0 && !_kbhit())
	{

		min = timer / 60;
		sec = timer % 60;
		if (sec == 0)
		{
			sec = 60;
			min--;
		}
		if (min < 10 && min >= 0) {
			cout << "0" << min << ":";
		}
		else
		{
			cout << min << ":";
		}
		if (sec < 10 && sec >= 0)
			cout << "0" << sec;
		else
		{
			cout << sec;
		}
		cout << '\r';
		Sleep(1000);
		timer--;
		//Check if timer 10 min expired or not
		if ((min==0 && sec==0) || ((clock() - start) / CLOCKS_PER_SEC) >600)
		{
			cout << '\n';
			cout << '\n';

			cout << "GAME OVER... YOU LOST!" << endl;
			cout << '\n';
			system("pause");
			exit(0);
		}
	}
	cout << '\n';
}

//timer blitz function
void blitzTime(int & timer)
{
	int min, sec;
	clock_t start = clock();
	cout << "Time Remaining: " << endl;
	while (timer >= 0 && !_kbhit())
	{

		min = timer / 60;
		sec = timer % 60;
		if (sec == 0)
		{
			sec = 60;
			min--;
		}
		cout << min << ":";
		if (sec < 10 && sec >= 0)
			cout << "0" << sec;
		else
		{
			cout << sec;
		}
		cout << '\r';
		Sleep(1000);
		timer--;
		//Check if 5 min timer expired or not
		if ((min == 0 && sec == 0) || ((clock() - start) / CLOCKS_PER_SEC) > 300)
		{
			cout << '\n';
			cout << '\n';

			cout << "GAME OVER... YOU LOST!" << endl;
			cout << '\n';
			system("pause");
			exit(0);
		}
	}
	cout << '\n';
}

//timer lightning function
void lightningTime()
{
	clock_t start = clock();

	int timer = 15;
	cout << "Time Remaining: " << endl;
	while (timer >= 0 && !_kbhit())
	{
		inRed(timer);
		cout <<'\r';
		Sleep(1000);
		timer--;

		//Check if 15 sec timer expired or not
		if (((clock() - start) / CLOCKS_PER_SEC) > 15)
		{
			cout << '\n';
			cout << '\n';

			//ColorIt(12);
			cout << "GAME OVER... YOU LOST!" << endl;
			//ColorIt(15);
			cout << '\n';
			system("pause");
			exit(0);
		}
	}
	cout << '\n';
}

//convert alphabet column into integer 
int SWITCH(char col) {
	int column = 0;
	switch (col) {
	case 'a': column = 0; break;
	case 'b': column = 1; break;
	case 'c': column = 2; break;
	case 'd': column = 3; break;
	case 'e': column = 4; break;
	case 'f': column = 5; break;
	case 'g': column = 6; break;
	case 'h': column = 7; break;
	}
	return column;
}

//switch for rows
int rowSwitch(char row) {
	int r = 0;
	switch (row) {
	case '8': r = 0; break;
	case '7': r = 1; break;
	case '6': r = 2; break;
	case '5': r = 3; break;
	case '4': r = 4; break;
	case '3': r = 5; break;
	case '2': r = 6; break;
	case '1': r = 7; break;
	}
	return r;
}

//print board of chess
void display(char val[8][8]) {
	//system("cls");
	cout << endl;
	cout << "\t\t\t\t\t  ****************************************************************************\n";
	cout << "\t\t\t\t\t  			         WELCOME TO CHESS	  				       \n";
	cout << "\t\t\t\t\t  ****************************************************************************\n\n";

	cout << "\t\t\t\t\t\t\t      a    b     c     d     e     f     g    h    " << endl;
	cout << "\t\t\t\t\t\t\t  |-----------------------------------------------|" << endl;
	cout << "\t\t\t\t\t\t\t  |     |     |     |     |     |     |     |     |" << endl;
	cout << "\t\t\t\t\t\t\t 8|  " << val[0][0] << "  |  " << val[0][1] << "  |  " << val[0][2] << "  |  " << val[0][3] << "  |  " << val[0][4] << "  |  " << val[0][5] << "  |  " << val[0][6] << "  |  " << val[0][7] << "  |" << endl;
	cout << "\t\t\t\t\t\t\t  |_____|_____|_____|_____|_____|_____|_____|_____|" << endl;
	cout << "\t\t\t\t\t\t\t  |     |     |     |     |     |     |     |     |" << endl;
	cout << "\t\t\t\t\t\t\t 7|  " << val[1][0] << "  |  " << val[1][1] << "  |  " << val[1][2] << "  |  " << val[1][3] << "  |  " << val[1][4] << "  |  " << val[1][5] << "  |  " << val[1][6] << "  |  " << val[1][7] << "  |" << endl;
	cout << "\t\t\t\t\t\t\t  |_____|_____|_____|_____|_____|_____|_____|_____|" << endl;
	cout << "\t\t\t\t\t\t\t  |     |     |     |     |     |     |     |     |" << endl;
	cout << "\t\t\t\t\t\t\t 6|  " << val[2][0] << "  |  " << val[2][1] << "  |  " << val[2][2] << "  |  " << val[2][3] << "  |  " << val[2][4] << "  |  " << val[2][5] << "  |  " << val[2][6] << "  |  " << val[2][7] << "  |" << endl;
	cout << "\t\t\t\t\t\t\t  |_____|_____|_____|_____|_____|_____|_____|_____|" << endl;
	cout << "\t\t\t\t\t\t\t  |     |     |     |     |     |     |     |     |" << endl;
	cout << "\t\t\t\t\t\t\t 5|  " << val[3][0] << "  |  " << val[3][1] << "  |  " << val[3][2] << "  |  " << val[3][3] << "  |  " << val[3][4] << "  |  " << val[3][5] << "  |  " << val[3][6] << "  |  " << val[3][7] << "  |" << endl;
	cout << "\t\t\t\t\t\t\t  |_____|_____|_____|_____|_____|_____|_____|_____|" << endl;
	cout << "\t\t\t\t\t\t\t  |     |     |     |     |     |     |     |     |" << endl;
	cout << "\t\t\t\t\t\t\t 4|  " << val[4][0] << "  |  " << val[4][1] << "  |  " << val[4][2] << "  |  " << val[4][3] << "  |  " << val[4][4] << "  |  " << val[4][5] << "  |  " << val[4][6] << "  |  " << val[4][7] << "  |" << endl;
	cout << "\t\t\t\t\t\t\t  |_____|_____|_____|_____|_____|_____|_____|_____|" << endl;
	cout << "\t\t\t\t\t\t\t  |     |     |     |     |     |     |     |     |" << endl;
	cout << "\t\t\t\t\t\t\t 3|  " << val[5][0] << "  |  " << val[5][1] << "  |  " << val[5][2] << "  |  " << val[5][3] << "  |  " << val[5][4] << "  |  " << val[5][5] << "  |  " << val[5][6] << "  |  " << val[5][7] << "  |" << endl;
	cout << "\t\t\t\t\t\t\t  |_____|_____|_____|_____|_____|_____|_____|_____|" << endl;
	cout << "\t\t\t\t\t\t\t  |     |     |     |     |     |     |     |     |" << endl;
	cout << "\t\t\t\t\t\t\t 2|  " << val[6][0] << "  |  " << val[6][1] << "  |  " << val[6][2] << "  |  " << val[6][3] << "  |  " << val[6][4] << "  |  " << val[6][5] << "  |  " << val[6][6] << "  |  " << val[6][7] << "  |" << endl;
	cout << "\t\t\t\t\t\t\t  |_____|_____|_____|_____|_____|_____|_____|_____|" << endl;
	cout << "\t\t\t\t\t\t\t  |     |     |     |     |     |     |     |     |" << endl;
	cout << "\t\t\t\t\t\t\t 1|  " << val[7][0] << "  |  " << val[7][1] << "  |  " << val[7][2] << "  |  " << val[7][3] << "  |  " << val[7][4] << "  |  " << val[7][5] << "  |  " << val[7][6] << "  |  " << val[7][7] << "  |" << endl;
	cout << "\t\t\t\t\t\t\t  |-----------------------------------------------|" << endl;
	
}//End interface function

//to display replay
void replaymove(char chess[][8], int currentrow, int currentcol, int rowtogo, int coltogo) 
{
	checkDestination(opposite);
	if (chess[currentrow][currentcol] != 'P' || chess[currentrow][currentcol] != 'p') {
		chess[rowtogo][coltogo] = chess[currentrow][currentcol];
		chess[currentrow][currentcol] = ' ';
	}
	else
	{
		Movements(chess, currentrow, currentcol, rowtogo, coltogo, opposite);
	}
}

//store all valid turns(FOR REPLAY)
void replayStore(int row, char col, int desRow, char desCol) {
	char srcRow = 8 - row + 48;
	char destinationRow = 8 - desRow + 48;

	saveMoves[indOfSaveMoves] = col;
	indOfSaveMoves++;
	saveMoves[indOfSaveMoves] = srcRow;
	indOfSaveMoves++;
	saveMoves[indOfSaveMoves] = desCol;
	indOfSaveMoves++;
	saveMoves[indOfSaveMoves] = destinationRow;
	indOfSaveMoves++;
	saveMoves[indOfSaveMoves] = '\n';
	indOfSaveMoves++;

}


//contains opponent pieces
void checkDestination(char opposite[]) {
	if (turn == 'A') {
		opposite[0] = 'p', opposite[1] = 'r', opposite[2] = 'n', opposite[3] = 'b', opposite[4] = 'q', opposite[5] = 'k';
	}
	else if (turn == 'B') {
		opposite[0] = 'P', opposite[1] = 'R', opposite[2] = 'N', opposite[3] = 'B', opposite[4] = 'Q', opposite[5] = 'K';
	}
}

//find king position
void kingPosition(char val[8][8], int& indi, int& indj) {
	if (turn == 'A') {
		//find king present coordinates
		bool cond = true;
		indi = 9, indj = 9;
		for (int i = 0; i < 8 && cond == true; i++) {
			for (int j = 0; j < 8; j++) {
				if (val[i][j] == 'K') {
					indi = i;
					indj = j;
					cond = false;
					break;
				}
			}
		}
	}

	else if (turn == 'B')
	{
		//find king present coordinates
		bool cond = true;
		indi = 9, indj = 9;
		for (int i = 0; i < 8 && cond == true; i++) {
			for (int j = 0; j < 8; j++) {
				if (val[i][j] == 'k') {
					indi = i;
					indj = j;
					cond = false;
					break;
				}
			}
		}
	}
}

//Valid MOVES
bool pawnForWhite(char val[8][8], int row, int column, int desRow, int desColumn, char opposite[])
{
	char ch = 32;
	//for empassent condition
	if (val[desRow][desColumn] == ch && prevCoordinate[4] == 1 && prevCoordinate[6] == row && desRow == row - 1 && (desColumn == column + 1 || desColumn == column - 1)) {
		val[desRow][desColumn] = val[row][column];
		val[row][column] = ch;
		val[prevCoordinate[6]][prevCoordinate[7]] = ch;
		return true;
	}
	//killing the opponent piece
	else if (val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])
	{
		if (desRow == row - 1 && desColumn == column + 1)
		{
			val[desRow][desColumn] = val[row][column];
			val[row][column] = ch;

			//pawn promotion
			if (desRow == 0)
			{
				cout << "R\tN\tB\tQ\n";
				cin >> val[row][column];
				while (val[row][column] != 'R' && val[row][column] != 'N' && val[row][column] != 'B' && val[row][column] != 'Q') {
					cout << "R\tN\tB\tQ\n";
					cin >> val[row][column];
				}
				val[desRow][desColumn] = val[row][column];
				return true;
			}
			return true;
		}
		else if (desRow == row - 1 && desColumn == column - 1)
		{
			val[desRow][desColumn] = val[row][column];
			val[row][column] = ch;

			//pawn promotion
			if (desRow == 0)
			{
				cout << "R\tN\tB\tQ\n";
				cin >> val[row][column];
				while (val[row][column] != 'R' && val[row][column] != 'N' && val[row][column] != 'B' && val[row][column] != 'Q') {
					cout << "R\tN\tB\tQ\n";
					cin >> val[row][column];
				}
				val[desRow][desColumn] = val[row][column];
				return true;
			}
			return true;
		}
	}

	//first move of pawn i.e it can move one step or two steps
	else if (row == 6)
	{
		if ((desRow == row - 1 || desRow == row - 2) && column == desColumn && val[desRow][desColumn] == ch) {
			if ((desRow == row - 2 && val[row - 1][desColumn] == ch) || desRow == row - 1) {
				val[desRow][desColumn] = val[row][column];
				val[row][column] = ch;
				return true;
			}
		}
	}

	else if (desRow == row - 1 && column == desColumn)
	{
		if (desRow == 0)
		{

			//pawn promotion
			cout << "R\tN\tB\tQ\n";
			cin >> val[row][column];
			while (val[row][column] != 'R' && val[row][column] != 'N' && val[row][column] != 'B' && val[row][column] != 'Q') {
				cout << "R\tN\tB\tQ\n";
				cin >> val[row][column];
			}
			val[desRow][desColumn] = val[row][column];
			return true;
		}
		else if (val[desRow][desColumn] == ch)
		{
			val[desRow][desColumn] = val[row][column];
			val[row][column] = ch;
			return true;
		}
	}
	return false;
}//PAWN FOR PLAYER WHITE END

bool rookForWhite(char val[8][8], int row, int column, int desRow, int desColumn, char opposite[])
{
	checkDestination(opposite);
	char ch = ' ';
	int count = 0;
	if (desColumn == column)
	{
		count = row;
		if (desRow > row)
		{																		//if destination row is greater than source row
			for (int i = row; i <= desRow; i++)
			{
				if (val[i][desColumn] == ch)
					count++;
			}
			if (val[desRow][desColumn] != ch && (val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5]))
				count++;
		}
		else if (desRow < row)													//if destination row is less than source row
		{
			for (int i = row; i >= desRow; i--)
			{
				if (val[i][column] == ch) {
					count--;
				}
			}
			if (val[desRow][desColumn] != ch && (val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5]))
				count--;
		}

		if (count == desRow)
		{
			return true;
		}
	}//FOR SAME COLUMN END

	else if (desRow == row)
	{
		count = column;
		if (desColumn > column)
		{
			for (int i = column; i <= desColumn; i++)
			{
				if (val[row][i] == ch)
					count++;
			}
			if (val[desRow][desColumn] != ch && (val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5]))
				count++;
		}
		else if (column > desColumn)
		{
			for (int i = column; i >= desColumn; i--)
			{
				if (val[row][i] == ch)
					count--;
			}
			if (val[desRow][desColumn] != ch && (val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5]))
				count--;
		}

		if (count == desColumn)
		{
			return true;
		}
	}//FOR SAME ROW END
	return false;
}//end of rookForWhite 

bool knightForWhite(char val[8][8], int row, int column, int desRow, int desColumn, char opposite[]) {
	char ch = ' ';
	if (desRow == row + 1) {
		if (desColumn == column + 2 && (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])) {
			return true;
		}
		else if (desColumn == column - 2 && (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])) {
			return true;
		}
	}
	else if (desRow == row - 1) {
		if (desColumn == column + 2 && (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])) {
			return true;
		}
		else if (desColumn == column - 2 && (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])) {
			val[desRow][desColumn] = val[row][column];
			val[row][column] = ch;
			return true;
		}
	}
	else if (desColumn == column + 1) {
		if (desRow == row + 2 && (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])) {
			return true;
		}
		else if (desRow == row - 2 && (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])) {
			return true;
		}
	}
	else if (desColumn == column - 1) {
		if (desRow == row + 2 && (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])) {
			return true;
		}
		else if (desRow == row - 2 && (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])) {
			return true;
		}
	}
	return false;
}  // end of knightForWhite

bool bishopForWhite(char val[8][8], int row, int column, int desRow, int desColumn, char opposite[]) {
	char ch = ' ';
	int count = 0;
	bool found = true;
	if (desRow > row && desColumn > column) {								//Forward down diagonal
		for (int i = row + 1, j = column + 1; i < 8 && j < 8; i++, j++) {
			if (desRow == i && desColumn == j) {
				found = false;
				break;
			}
			else if (val[i][j] == ch) {
				count++;
			}
		}
	}
	else if (desRow > row&& desColumn < column) {							//Backward down diagonal
		for (int i = row, j = column; i < 8 && j >= 0; i++, j--) {
			if (desRow == i && desColumn == j) {
				found = false;
				break;
			}
			else if (val[i][j] == ch) {
				count++;
			}
		}
	}
	else if (desRow < row && desColumn > column) {							//Forward up diagonal
		for (int i = row, j = column; i >= 0 && j < 8; i--, j++) {
			if (desRow == i && desColumn == j) {
				found = false;
				break;
			}
			else if (val[i][j] == ch) {
				count++;
			}
		}
	}
	else if (desRow < row && desColumn < column) {							//Backward up diagonal
		for (int i = row, j = column; i >= 0 && j >= 0; i--, j--) {
			if (desRow == i && desColumn == j) {
				found = false;
				break;
			}
			else if (val[i][j] == ch) {
				count++;
			}
		}
	}
	if (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5]) {
		count++;
	}
	int dif = desRow - row;
	if (dif < 0)
		dif = -1 * dif;
	if (found == false && count == dif) {
		return true;
	}
	return false;
}

bool queenForWhite(char val[8][8], int row, int column, int desRow, int desColumn, char opposite[]) {
	bool rook = rookForWhite(val, row, column, desRow, desColumn, opposite);
	if (rook == false) {
		bool bishop = bishopForWhite(val, row, column, desRow, desColumn, opposite);
		if (bishop == false) {
			return false;
		}
		else {
			return true;
		}
	}
		return rook;
}

bool kingForWhite(char val[8][8], int row, int column, int desRow, int desColumn, char opposite[]) {
	char ch = ' ';
	if (desColumn == column && (desRow == row + 1 || desRow == row - 1)) {
		if (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5]) {
			return true;
		}
	}
	else if (desRow == row && (desColumn == column + 1 || desColumn == column - 1)) {
		if (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5]) {
			return true;
		}
	}
	else if (desRow == row + 1 && (desColumn == column + 1 || desColumn == column - 1)) {
		if (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5]) {
			return true;
		}
	}
	else if (desRow == row - 1 && (desColumn == column + 1 || desColumn == column - 1)) {
		if (val[desRow][desColumn] == ch || val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5]) {
			return true;
		}
	}
	return false;
}

bool pawnForBlack(char val[8][8], int row, int column, int desRow, int desColumn, char opposite[]) {
	char ch = 32;
	if (val[desRow][desColumn] == ch && prevCoordinate[0] == 6 && prevCoordinate[2] == row && desRow == row + 1 && (desColumn == column + 1 || desColumn == column - 1)) {
		val[desRow][desColumn] = val[row][column];
		val[row][column] = ch;
		val[prevCoordinate[2]][prevCoordinate[3]] = ch;
		return true;
	}
	else if (val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])
	{
		if (desRow == row + 1 && desColumn == column + 1)
		{
			val[desRow][desColumn] = val[row][column];
			val[row][column] = ch;
			if (desRow == 7)
			{
				cout << "r\tn\tb\tq\n";
				cin >> val[row][column];
				while (val[row][column] != 'r' && val[row][column] != 'n' && val[row][column] != 'b' && val[row][column] != 'q') {
					cout << "r\tn\tb\tq\n";
					cin >> val[row][column];
				}
				val[desRow][desColumn] = val[row][column];
				return true;
			}
			return true;
		}
		else if (desRow == row + 1 && desColumn == column - 1)
		{
			val[desRow][desColumn] = val[row][column];
			val[row][column] = ch;
			if (desRow == 7)
			{
				cout << "r\tn\tb\tq\n";
				cin >> val[row][column];
				while (val[row][column] != 'r' && val[row][column] != 'n' && val[row][column] != 'b' && val[row][column] != 'q') {
					cout << "r\tn\tb\tq\n";
					cin >> val[row][column];
				}
				val[desRow][desColumn] = val[row][column];
				return true;
			}
			return true;
		}
	}

	else if (row == 1)
	{
		if ((desRow == row + 1 || desRow == row + 2) && column == desColumn && val[desRow][desColumn] == ch) {
			if (desRow == row + 1 || (desRow == row + 2 && val[row + 1][desColumn] == ch)) {
				val[desRow][desColumn] = val[row][column];
				val[row][column] = ch;
				return true;
			}
		}
	}

	else if (desRow == row + 1 && column == desColumn)
	{
		if (desRow == 7)
		{
			cout << "r\tn\tb\tq\n";
			cin >> val[row][column];
			while (val[row][column] != 'r' && val[row][column] != 'n' && val[row][column] != 'b' && val[row][column] != 'q') {
				cout << "r\tn\tb\tq\n";
				cin >> val[row][column];
			}
			val[desRow][desColumn] = val[row][column];
			return true;
		}
		else if (val[desRow][desColumn] == ch)
		{
			val[desRow][desColumn] = val[row][column];
			val[row][column] = ch;
			return true;
		}
	}
	return false;
}//pawn for black end

void Movements(char board[8][8], int row, int column, int desRow, int desColumn, char opposite[])
{
	checkDestination(opposite);
	if (board[row][column] == 'P')
		pawnForWhite(board, row, column, desRow, desColumn, opposite);
	else if (board[row][column] == 'p')
		pawnForBlack(board, row, column, desRow, desColumn, opposite);
	else if (board[row][column] == 'R' || board[row][column] == 'r') {
		checkDestination(opposite);
		bool found = rookForWhite(board, row, column, desRow, desColumn, opposite);
		if (found == true) {
			board[desRow][desColumn] = board[row][column];
			board[row][column] = ' ';
		}
	}
	else if (board[row][column] == 'N' || board[row][column] == 'n') {
		bool found = knightForWhite(board, row, column, desRow, desColumn, opposite);
		if (found == true) {
			board[desRow][desColumn] = board[row][column];
			board[row][column] = ' ';
		}
	}
	else if (board[row][column] == 'B' || board[row][column] == 'b') {
		bool found = bishopForWhite(board, row, column, desRow, desColumn, opposite);
		if (found == true) {
			board[desRow][desColumn] = board[row][column];
			board[row][column] = ' ';
		}
	}
	else if (board[row][column] == 'Q' || board[row][column] == 'q') {
		bool found = queenForWhite(board, row, column, desRow, desColumn, opposite);
		if (found == true) {
			board[desRow][desColumn] = board[row][column];
			board[row][column] = ' ';
		}
	}
	else if (board[row][column] == 'K' || board[row][column] == 'k') {
		bool found = kingForWhite(board, row, column, desRow, desColumn, opposite);
		if (found == true) {
			board[desRow][desColumn] = board[row][column];
			board[row][column] = ' ';
		}
	}
}


//CHECKS 
bool pawnCheckforWhite(char val[8][8], int row, int column, char opposite[]) {
	if (val[row - 1][column + 1] == opposite[0] || val[row - 1][column - 1] == opposite[0]) {
		storeCheckPieceInd[0] = row - 1;
		if (val[row - 1][column + 1] == opposite[0])
			storeCheckPieceInd[1] = column + 1;
		else if (val[row - 1][column - 1] == opposite[0])
			storeCheckPieceInd[1] = column - 1;
		return true;
	}
	return false;
}

bool pawnCheckforBlack(char val[8][8], int row, int column, char opposite[]) {
	if (val[row + 1][column + 1] == opposite[0] || val[row + 1][column - 1] == opposite[0]) {
		storeCheckPieceInd[0] = row + 1;
		if (val[row + 1][column + 1] == opposite[0])
			storeCheckPieceInd[1] = column + 1;
		else if (val[row + 1][column - 1] == opposite[0])
			storeCheckPieceInd[1] = column - 1;
		return true;
	}
	return false;
}

bool rookCheck(char val[8][8], int row, int column, char opposite[]) {
	int count = 1, i = 0;
	for (i = row; i < 7 && val[i][column] != opposite[1]; i++) {						//same column upward
		if (val[i][column] == ' ')
			count++;
	}
	int dif = i - row;
	if (count == dif && val[i][column] == opposite[1]) {
		return true;
	}
	count = 1, i = 0;
	for (i = row; i > 0 && val[i][column] != opposite[1]; i--) {						//same column downward
		if (val[i][column] == ' ')
			count++;
	}
	dif = row - i;
	if (count == dif && val[i][column] == opposite[1]) {
		return true;
	}

	count = 1, i = 0;
	for (i = column; i < 7 && val[row][i] != opposite[1]; i++) {						//same row rightward
		if (val[row][i] == ' ')
			count++;
	}
	dif = i - column;
	if (count == dif && val[row][i] == opposite[1]) {
		return true;
	}
	count = 1, i = 0;
	for (i = column; i > 0 && val[row][i] != opposite[1]; i--) {						//same row leftward
		if (val[row][i] == ' ')
			count++;
	}
	dif = column - i;
	if (count == dif && val[row][i] == opposite[1]) {
		return true;
	}
	return false;
}

bool knightCheck(char val[8][8], int row, int column, char opposite[]) {
	if (val[row + 1][column + 2] == opposite[2] || val[row + 1][column - 2] == opposite[2] || val[row - 1][column + 2] == opposite[2] || val[row - 1][column - 2] == opposite[2] || val[row + 2][column + 1] == opposite[2] || val[row + 2][column - 1] == opposite[2] || val[row - 2][column + 1] == opposite[2] || val[row - 2][column - 1] == opposite[2])
	{
		if (val[row + 1][column + 2] == opposite[2]) {
			storeCheckPieceInd[0] = row + 1;
			storeCheckPieceInd[1] = column + 2;
		}
		else if (val[row + 1][column - 2] == opposite[2]) {
			storeCheckPieceInd[0] = row + 1;
			storeCheckPieceInd[1] = column - 2;
		}
		else if (val[row - 1][column + 2] == opposite[2]) {
			storeCheckPieceInd[0] = row - 1;
			storeCheckPieceInd[1] = column + 2;
		}
		else if (val[row - 1][column - 2] == opposite[2]) {
			storeCheckPieceInd[0] = row - 1;
			storeCheckPieceInd[1] = column - 2;
		}
		else if (val[row + 2][column + 1] == opposite[2]) {
			storeCheckPieceInd[0] = row + 2;
			storeCheckPieceInd[1] = column + 1;
		}
		else if (val[row + 2][column - 1] == opposite[2]) {
			storeCheckPieceInd[0] = row + 2;
			storeCheckPieceInd[1] = column - 1;
		}
		else if (val[row - 2][column - 1] == opposite[2]) {
			storeCheckPieceInd[0] = row - 2;
			storeCheckPieceInd[1] = column - 1;
		}
		else if (val[row - 2][column + 1] == opposite[2]) {
			storeCheckPieceInd[0] = row - 2;
			storeCheckPieceInd[1] = column + 1;
		}
		return true;
	}
	return false;
}

bool bishopCheck(char val[8][8], int row, int column, char opposite[]) {
	int count = 1, i = 0, j = 0;
	for (i = row, j = column; val[i][j] != opposite[3] && i > 0 && j < 7; i--, j++) {			//forward upward diagonal
		if (val[i][j] == ' ')
			count++;
	}
	int dif = row - i;
	if (count == dif && val[i][j] == opposite[3]) {
		return true;
	}//end up forward diagonal

	count = 1, i = 0, j = 0;
	for (i = row, j = column; val[i][j] != opposite[3] && i < 7 && j < 7; i++, j++) {			//forward downward diagonal
		if (val[i][j] == ' ')
			count++;
	}
	dif = i - row;
	if (count == dif && val[i][j] == opposite[3]) {
		return true;
	}//end down forward diagonal 

	count = 1, i = 0, j = 0;
	for (i = row, j = column; val[i][j] != opposite[3] && i > 0 && j > 0; i--, j--) {				//backward up diagonal
		if (val[i][j] == ' ')
			count++;
	}
	dif = row - i;
	if (count == dif && val[i][j] == opposite[3]) {
		return true;
	}//end up backward diagonal

	count = 1, i = 0, j = 0;
	for (i = row, j = column; val[i][j] != opposite[3] && i < 7 && j > 0; i++, j--) {				//downward backward diagonal
		if (val[i][j] == ' ')
			count++;
	}
	dif = i - row;
	if (count == dif && val[i][j] == opposite[3]) {
		return true;
	}//end down backward diagonal
	return false;
}

bool queenCheck(char val[8][8], int row, int column, char opposite[]) {
	int count = 1, i = 0, j = 0;
	for (i = row; i < 7 && val[i][column] != opposite[4]; i++) {						//same column upward
		if (val[i][column] == ' ')
			count++;
	}
	int dif = i - row;
	if (count == dif && val[i][column] == opposite[4]) {
		return true;
	}
	count = 1, i = 0;
	for (i = row; i > 0 && val[i][column] != opposite[4]; i--) {						//same column downward
		if (val[i][column] == ' ')
			count++;
	}
	dif = row - i;
	if (count == dif && val[i][column] == opposite[4]) {
		return true;
	}

	count = 1, i = 0;
	for (i = column; i < 7 && val[row][i] != opposite[4]; i++) {						//same row rightward
		if (val[row][i] == ' ')
			count++;
	}
	dif = i - column;
	if (count == dif && val[row][i] == opposite[4]) {
		return true;
	}
	count = 1, i = 0;
	for (i = column; i > 0 && val[row][i] != opposite[4]; i--) {						//same row leftward
		if (val[row][i] == ' ')
			count++;
	}
	dif = column - i;
	if (count == dif && val[row][i] == opposite[4]) {
		return true;
	}
	count = 1, i = 0, j = 0;
	for (i = row, j = column; val[i][j] != opposite[4] && i > 0 && j < 7; i--, j++) {			//forward upward diagonal
		if (val[i][j] == ' ')
			count++;
	}
	dif = row - i;
	if (count == dif && val[i][j] == opposite[4]) {
		return true;
	}//end up forward diagonal

	count = 1, i = 0, j = 0;
	for (i = row, j = column; val[i][j] != opposite[4] && i < 7 && j < 7; i++, j++) {			//forward downward diagonal
		if (val[i][j] == ' ')
			count++;
	}
	dif = i - row;
	if (count == dif && val[i][j] == opposite[4]) {
		return true;
	}//end down forward diagonal 

	count = 1, i = 0, j = 0;
	for (i = row, j = column; val[i][j] != opposite[4] && i > 0 && j > 0; i--, j--) {				//backward up diagonal
		if (val[i][j] == ' ')
			count++;
	}
	dif = row - i;
	if (count == dif && val[i][j] == opposite[4]) {
		return true;
	}//end up backward diagonal

	count = 1, i = 0, j = 0;
	for (i = row, j = column; val[i][j] != opposite[4] && i < 7 && j > 0; i++, j--) {				//downward backward diagonal
		if (val[i][j] == ' ')
			count++;
	}
	dif = i - row;
	if (count == dif && val[i][j] == opposite[4]) {
		return true;
	}//end down backward diagonal
	return false;

}

bool kingCheck(char val[8][8], int row, int column, char opposite[]) {
	if (val[row + 1][column] == opposite[5] || val[row - 1][column] == opposite[5] || val[row + 1][column + 1] == opposite[5] || val[row + 1][column - 1] == opposite[5] || val[row - 1][column + 1] == opposite[5] || val[row - 1][column - 1] == opposite[5] || val[row][column + 1] == opposite[5] || val[row][column - 1] == opposite[5]) {
		return true;
	}
	return false;
}

bool check(char val[8][8], int row, int column, char opposite[]) {
	bool found = true;
	if (turn == 'A')
	{
		found = pawnCheckforWhite(val, row, column, opposite);
	}
	else
	{
		found = pawnCheckforBlack(val, row, column, opposite);
	}

	if (found == true) {
		return true;
	}
	else
		found = rookCheck(val, row, column, opposite);
	if (found == true)
		return true;
	else
		found = knightCheck(val, row, column, opposite);
	if (found == true)
		return true;
	else
		found = bishopCheck(val, row, column, opposite);
	if (found == true)
		return true;
	else
		found = queenCheck(val, row, column, opposite);
	if (found == true)
		return true;
	else
		found = kingCheck(val, row, column, opposite);
	if (found == true)
		return true;
	else
		return false;
}

bool afterCheck(char val[8][8], int row, int column, int desRow, int desColumn, char temp, char opposite[]) {
	bool found = true;
	int indi = 0, indj = 0;
	kingPosition(val, indi, indj);
	if (check(val, indi, indj, opposite) == true)
		found = false;
	if (found == false) {
		val[row][column] = val[desRow][desColumn];
		val[desRow][desColumn] = temp;  //space
		return false;
	}
	return true;
}

//Castling
bool castling(char val[8][8], int row, int column, int desRow, int desColumn, char opposite[], int& countK, int& countr_1, int& countr_2)
{
	if (turn == 'A') {
		opposite[0] = 'P', opposite[1] = 'R', opposite[2] = 'N', opposite[3] = 'B', opposite[4] = 'Q', opposite[5] = 'K';
	}
	else if (turn == 'B') {
		opposite[0] = 'p', opposite[1] = 'r', opposite[2] = 'n', opposite[3] = 'b', opposite[4] = 'q', opposite[5] = 'k';
	}
	char ch = 32;
	int count = 1, i = 0;
	bool found = false, checker = true, option = false;

	if (row == 0 || row == 7) {

		//queen Side castling 
		if (val[row][column] == opposite[5] && desColumn == column - 2 && column == 4 && val[desRow][desColumn] == ch) {
			if (row == desRow && val[row][column - 4] == opposite[1]) {
				if (countK == 1 && countr_1 == 1) {
					count = 1;
					for (i = column; i > 0; i--) {
						if (val[row][i] == ' ')
							count++;
					}
					int dif = column - i;
					checkDestination(opposite);
					if (count == dif) {
						if (check(val, row, column, opposite) == true)
							return true;
						else
							checker = check(val, row, column - 1, opposite);
						if (checker == true)
							return true;
						else
							checker = check(val, row, column - 2, opposite);
						if (checker == true)
							return true;
						else
						{
							val[desRow][desColumn] = val[row][column];
							val[row][column] = ' ';
							val[row][column - 1] = val[row][column - 4];
							val[row][column - 4] = ' ';
							return false;
						}
					}
				}
			}
		}

		//king Side castling
		if (val[row][column] == opposite[5] && desColumn == column + 2 && column == 4 && val[desRow][desColumn] == ch) {
			if (row == desRow && val[row][column + 3] == opposite[1]) {
				if (countK == 1 && countr_2 == 1) {
					count = 1;
					for (i = column; i < 7; i++) {
						if (val[row][i] == ' ')
							count++;
					}
					int dif = i - column;
					checkDestination(opposite);
					if (count == dif) {
						if (check(val, row, column, opposite) == true)
							return true;
						else
							checker = check(val, row, column - 1, opposite);
						if (checker == true)
							return true;
						else
							checker = check(val, row, column - 2, opposite);
						if (checker == true)
							return true;
						else
						{
							val[desRow][desColumn] = val[row][column];
							val[row][column] = ' ';
							val[row][column + 1] = val[row][column + 3];
							val[row][column + 3] = ' ';
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}

//checkMate for PAWN

bool pawnCheckMate(char val[8][8], int row, int column, int desRow, int desColumn, char opposite[]) {
	if (turn == 'A') {
		char ch = 32;
		//for empassent condition
		if (val[desRow][desColumn] == ch && prevCoordinate[4] == 1 && prevCoordinate[6] == row && desRow == row - 1 && (desColumn == column + 1 || desColumn == column - 1))
		{
			val[prevCoordinate[6]][prevCoordinate[7]] = ch;
			return true;
		}

		else if (val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])
		{
			if ((desRow == row - 1 && desColumn == column + 1) || (desRow == row - 1 && desColumn == column - 1))
			{
				return true;
			}
		}
		else if (row == 6)
		{
			if ((desRow == row - 1 || desRow == row - 2) && column == desColumn && val[desRow][desColumn] == ch) {
				if (desRow == row - 2 && val[row - 1][desColumn] == ch)
				return true;
				if (desRow == row - 1)
					return true;
			}
		}
		else if (desRow == row - 1 && column == desColumn)
		{
			if (val[desRow][desColumn] == ch) {
				return true;
			}
		}
	}
	else
	{
		char ch = 32;
		if (val[desRow][desColumn] == ch && prevCoordinate[0] == 6 && prevCoordinate[2] == row && desRow == row + 1 && (desColumn == column + 1 || desColumn == column - 1)) {
			val[prevCoordinate[2]][prevCoordinate[3]] = ch;
			return true;
		}
		else if (val[desRow][desColumn] == opposite[0] || val[desRow][desColumn] == opposite[1] || val[desRow][desColumn] == opposite[2] || val[desRow][desColumn] == opposite[3] || val[desRow][desColumn] == opposite[4] || val[desRow][desColumn] == opposite[5])
		{
			if (desRow == row + 1 && desColumn == column + 1 || desRow == row + 1 && desColumn == column - 1)
			{
				return true;
			}
		}
		else if (row == 1)
		{
			if ((desRow == row + 1 || desRow == row + 2) && column == desColumn && val[desRow][desColumn] == ch) {
				if (desRow == row + 2 && val[row + 1][desColumn] == ch)
					return true;
				if(desRow == row + 1)
				return true;
			}
		}
		else if (desRow == row + 1 && column == desColumn)
		{
			if (val[desRow][desColumn] == ch)
				return true;
		}
	}
	return false;
}

//checkmate
bool CheckForCheckmate(char val[8][8], char opposite[])
{
	int indi = 0, indj = 0;
	char savePiece = '\0';
	checkDestination(opposite);
	char ch = ' ';
	char piece = '\0'; //declare var
	for (int yi = 0; yi < 8; yi++)
	{
		for (int xi = 0; xi < 8; xi++)
		{
			if (turn == 'A') //if white...
			{
				piece = '\0';
				if (val[yi][xi] == 'P') piece = 'P'; //check for pawn
				else if (val[yi][xi] == 'N') piece = 'N'; //check for knight
				else if (val[yi][xi] == 'B') piece = 'B'; //check for bishop
				else if (val[yi][xi] == 'R') piece = 'R'; //check for rook
				else if (val[yi][xi] == 'Q') piece = 'Q'; //check for queen
				else if (val[yi][xi] == 'K') piece = 'K'; //check for king
				for (int yf = 0; yf < 8; yf++)
				{
					for (int xf = 0; xf < 8; xf++)
					{
						if (piece == 'P')
						{
							if (pawnCheckMate(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'P'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'P'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
						else if (piece == 'N')
						{
							if (knightForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'N'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'N'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
						else if (piece == 'B')
						{
							if (bishopForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'B'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'B'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;

							}
						}
						else if (piece == 'R')
						{
							if (rookForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'R'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'R'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
						else if (piece == 'Q')
						{
							if (queenForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'Q'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'Q'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
						else if (piece == 'K')
						{
							if (kingForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'K'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'K'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
					}
				}
			}
			else if (turn == 'B') //if BLACK...
			{
				piece = ch;
				if (val[yi][xi] == 'p') piece = 'p'; //check for pawn
				else if (val[yi][xi] == 'n') piece = 'n'; //check for knight
				else if (val[yi][xi] == 'b') piece = 'b'; //check for bishop
				else if (val[yi][xi] == 'r') piece = 'r'; //check for rook
				else if (val[yi][xi] == 'q') piece = 'q'; //check for queen
				else if (val[yi][xi] == 'k') piece = 'k'; //check for king
				for (int yf = 0; yf < 8; yf++)
				{
					for (int xf = 0; xf < 8; xf++)
					{
						savePiece = '\0';
						if (piece == 'p')
						{
							if (pawnCheckMate(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'p'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'p'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
						else if (piece == 'n')
						{
							if (knightForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'n'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'n'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
						else if (piece == 'b')
						{
							if (bishopForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'b'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'b'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
						else if (piece == 'r')
						{
							if (rookForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'r'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'r'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
						else if (piece == 'q')
						{
							if (queenForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'q'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'q'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
						else if (piece == 'k')
						{
							checkDestination(opposite);
							if (kingForWhite(val, yi, xi, yf, xf, opposite) == true)
							{
								savePiece = val[yf][xf];
								val[yf][xf] = 'k'; //set val
								val[yi][xi] = ch; //set val
								kingPosition(val, indi, indj);
								bool checker = check(val, indi, indj, opposite);  //check if still in check
								val[yf][xf] = savePiece; //set val
								val[yi][xi] = 'k'; //set val
								if (checker == false) return false; //check if white is in check
								kingPosition(val, indi, indj);
								if (check(val, indi, indj, opposite) == false)
									return false;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

//stalemate
bool stalemate(char val[8][8], char opposite[]) {
	int indi = 0, indj = 0;
		char savePiece = '\0';
		checkDestination(opposite);
		char ch = ' ';
		char piece = '\0'; //declare var
		for (int yi = 0; yi < 8; yi++)
		{
			for (int xi = 0; xi < 8; xi++)
			{
				if (turn == 'A') //if white...
				{
					piece = '\0';
					if (val[yi][xi] == 'P') piece = 'P'; //check for pawn
					else if (val[yi][xi] == 'N') piece = 'N'; //check for knight
					else if (val[yi][xi] == 'B') piece = 'B'; //check for bishop
					else if (val[yi][xi] == 'R') piece = 'R'; //check for rook
					else if (val[yi][xi] == 'Q') piece = 'Q'; //check for queen
					else if (val[yi][xi] == 'K') piece = 'K'; //check for king
					for (int yf = 0; yf < 8; yf++)
					{
						for (int xf = 0; xf < 8; xf++)
						{
							if (piece == 'P')
							{
								if (xi != xf || yi != yf) {
									if (pawnCheckMate(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'P'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'P'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
							else if (piece == 'N')
							{
								if (xi != xf || yi != yf) {
									if (knightForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'N'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'N'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
							else if (piece == 'B')
							{
								if (xi != xf || yi != yf) {
									if (bishopForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'B'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'B'; //set val
										if (checker == false) return false; //check if white is in check

									}
								}
							}
							else if (piece == 'R')
							{
								if (xi != xf || yi != yf) {
									if (rookForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'R'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'R'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
							else if (piece == 'Q')
							{
								if (xi != xf || yi != yf) {
									if (queenForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'Q'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'Q'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
							else if (piece == 'K')
							{
								if (xi != xf || yi != yf) {
									if (kingForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'K'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'K'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
						}
					}
				}

				//if turn is of BLACK
				else if (turn == 'B') //if BLACK...
				{
					piece = ch;
					if (val[yi][xi] == 'p') piece = 'p'; //check for pawn
					else if (val[yi][xi] == 'n') piece = 'n'; //check for knight
					else if (val[yi][xi] == 'b') piece = 'b'; //check for bishop
					else if (val[yi][xi] == 'r') piece = 'r'; //check for rook
					else if (val[yi][xi] == 'q') piece = 'q'; //check for queen
					else if (val[yi][xi] == 'k') piece = 'k'; //check for king
					for (int yf = 0; yf < 8; yf++)
					{
						for (int xf = 0; xf < 8; xf++)
						{
							savePiece = '\0';
							if (piece == 'p')
							{
								if (xi != xf || yi != yf) {
									if (pawnCheckMate(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'p'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'p'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
							else if (piece == 'n')
							{
								if (xi != xf || yi != yf) {
									if (knightForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'n'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'n'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
							else if (piece == 'b')
							{
								if (xi != xf || yi != yf) {
									if (bishopForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'b'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'b'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
							else if (piece == 'r')
							{
								if (xi != xf || yi != yf) {
									if (rookForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'r'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'r'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
							else if (piece == 'q')
							{
								if (xi != xf || yi != yf) {
									if (queenForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'q'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'q'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
							else if (piece == 'k')
							{
								if (xi != xf || yi != yf) {
									checkDestination(opposite);
									if (kingForWhite(val, yi, xi, yf, xf, opposite) == true)
									{
										savePiece = val[yf][xf];
										val[yf][xf] = 'k'; //set val
										val[yi][xi] = ch; //set val
										kingPosition(val, indi, indj);
										bool checker = check(val, indi, indj, opposite);  //check if still in check
										val[yf][xf] = savePiece; //set val
										val[yi][xi] = 'k'; //set val
										if (checker == false) return false; //check if white is in check
									}
								}
							}
						}
					}
				}
			}
		}
	return true;
}



//turns of player 
int playerTurn(char val[8][8], bool& checkA, bool& checkB, int& countAK, int& countAr_1, int& countAr_2, int& countBk, int& countBr_1, int& countBr_2)
{
	//initialize variables
	int rowA=9, columnA=9, desColumnA=9, desRowA=9, rowB=9, columnB=9, desColumnB=9, desRowB=9;
	int indi = 0, indj = 0;
	char col, desCol, ch = 32, temp = '\0';
	bool found2 = true;


	//50 consecutive moves(no pawn move & no piece capture)
	int presentPieces = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (val[i][j] != ' ') {			
				presentPieces++;
			}
		}
	}
	if (presentPieces < countPreviousPieces) {				//compare present pieces with previous pieces
		countPreviousPieces = presentPieces;
		consecutiveMoves = 0;
	}
	if (consecutiveMoves == 50) {
		cout << "::GAME DRAW::\n::CONSECUTIVE 50 MOVES NEITHER ANY PEICE CAPTURE NOR PAWN MOVE::\n";
		return 1;
	}



	//draw condition (insufficient pieces left)
	int pieceCount = 0, bishopCountA = 0, bishopCountB = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((val[i][j] != 'K' && val[i][j] != 'k' && val[i][j] != ' ' && (val[i][j] != 'B' && val[i][j] != 'b'))) {
				pieceCount++;
			}
			if (val[i][j] == 'B')
				bishopCountA++;
			if (val[i][j] == 'b')
				bishopCountB++;
		}
	}

	//if one king+bishop && an opponent king remain, then draw
	if (pieceCount == 0 && ((bishopCountA == 1 && bishopCountB == 0) || (bishopCountA == 0 && bishopCountB == 1))) {
		cout << "::GAME DARW DUE TO INSUFFICIENT PIECES::\n\n";
		return 1;
	}



	//TURN OF PLAYER WHITE(A)
	if (turn == 'A')				//turn of white player
	{
		cout << "PLAYER WHITE TURN(UPPER CASE)\n\n";

		//put opponent pieces in opposite[] array 
		checkDestination(opposite);
		
		//indexes where king is placed
		bool cond = true;
		indi = 0, indj = 0;

		//finding index where king is placed
		for (int i = 0; i < 8 && cond == true; i++) {
			for (int j = 0; j < 8; j++) {
				if (val[i][j] == 'K') {
					indi = i;
					indj = j;
					cond = false;
					break;
				}
			}
		}



		//checkmate
		bool checkMate = CheckForCheckmate(val, opposite);
		if (checkMate == true) {                     
			if (foolmateA == 2) {
				cout << "::FOOLS MATE::\n\n";				//if foolmate
			}
			else
			{
				cout << "::CHECKMATE::\n\n";			//if checkmate
			}
			cout << "::PLAYER BLACK WINS THE GAME::\n\n";
				return 1;					
		}
		checkA = check(val, indi, indj, opposite);
		
		//check on player WHITE
		if (checkA == true) 
		{														//if player WHITE(A) is in check
			cout << "::PLAYER WHITE IS IN CHECK::\n\n";
		}

		else {														//stalemate
			bool staMate = stalemate(val, opposite);
			if (staMate == true) {
				cout << "::STALEMATE::\n\n";
				
				//if mode is classic then compare time
				if (mode == 'C' || mode == 'c') 
				{
					if (classicTimerA > classicTimerB)
					{
						cout << "::PLAYER BLACK WINS THE GAME DUE TO TIME::\n";
						return 1;
					}
					else if (classicTimerA < classicTimerB) 
					{
						cout << "::PLAYER WHITE WINS THE GAME DUE TO TIME::\n";
						return 1;
					}
				}

				cout << "::GAME DRAW::\n\n";
				return 1;  
			}
		}

		char userChoice = '\0';
		if (mode != 'L' && mode != 'l') {
		//conditions for draw game (user choice)
		cout << "::PRESS H FOR HAND SHAKE::\n";
		cout << "::PRESS R FOR RESIGN::\n";
		cin>>userChoice;

		//handshake
			if (userChoice == 'H') {
				char handshakeB = '\0';
				cout << "::PLAYER WHITE WANT TO HANDSHAKE::\n::PRESS H TO HAND SHAKE::\n";
				cin >> handshakeB;
				if (userChoice == 'H' && handshakeB == 'H') {

					cout << "::HAND SHAKE::\n\n";

					//if mode is classic, then compare time
					if (mode == 'C' || mode == 'c')
					{
						if (classicTimerA > classicTimerB)
						{
							cout << "::PLAYER BLACK WINS THE GAME DUE TO TIME::\n";
							return 1;
						}
						else if (classicTimerA < classicTimerB)
						{
							cout << "::PLAYER WHITE WINS THE GAME DUE TO TIME::\n";
							return 1;
						}
					}
					cout << "::GAME DRAW BY HAND SHAKE::\n\n";
					return 1;
				}
			}
		}


		//check for resign (resign not include in lightening mode)
		if (mode != 'L' && mode != 'l') {
			if (userChoice == 'R' || userChoice == 'r') {
				cout << "::PLAYER WHITE RESIGN::\n\n";
				cout << "::PLAYER BLACK WINS THE GAME::\n\n";
				return 1;
			}
		}
		

		//games mode
		if (mode == 'L' || mode == 'l') {
			lightningTime();
		}
		else if (mode == 'R' || mode == 'r') {
				rapidTime(rapidTimerA);
		}
		else if (mode == 'B' || mode == 'b') {
			blitzTime(blitzTimerA);
		}
		else if (mode == 'C' || mode == 'c') {
			classicTime(classicTimerA, hourA,minA,secA);
		}

		//input source position
		cout << "MOVE FROM: ";
		cin >> col >> rowA;
		
		//convert virtual index of row into actual index of row
		rowA = 8 - rowA;

		//convert alphabets column into integer column using switch
		switch (col) {
		case 'a': columnA = 0; break;
		case 'b': columnA = 1; break;
		case 'c': columnA = 2; break;
		case 'd': columnA = 3; break;
		case 'e': columnA = 4; break;
		case 'f': columnA = 5; break;
		case 'g': columnA = 6; break;
		case 'h': columnA = 7; break;
		default:
		{
			cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
			playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			break;
		}
		}

		//input destination position
		cout << "TO: ";
		cin >> desCol >> desRowA;

		//convert virtual index of destination row into actual index of destination row
		desRowA = 8 - desRowA;
		
		// convert alphabets destination column into integer destination column using switch
		switch (desCol) {
		case 'a': desColumnA = 0; break;
		case 'b': desColumnA = 1; break;
		case 'c': desColumnA = 2; break;
		case 'd': desColumnA = 3; break;
		case 'e': desColumnA = 4; break;
		case 'f': desColumnA = 5; break;
		case 'g': desColumnA = 6; break;
		case 'h': desColumnA = 7; break;
		default:
		{
			cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
			playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			break;
		}
		}

		//store position of last moves
		prevCoordinate[0] = rowA;
		prevCoordinate[1] = columnA;
		prevCoordinate[2] = desRowA;
		prevCoordinate[3] = desColumnA;
	}

	//turn of PLAYER black
	else if (turn == 'B')
	{
		cout << "PLAYER BLACK TURN(SMALL CASE)\n\n";
		//put opponent pieces in opposite[] array 
		checkDestination(opposite);

		//finding index where king is placed
		bool cond = true;
		indi = 0, indj = 0;
		for (int i = 0; i < 8 && cond == true; i++) {
			for (int j = 0; j < 8; j++) {
				if (val[i][j] == 'k') {
					indi = i;
					indj = j;
					cond = false;
					break;
				}
			}
		}

		//checkmate
		bool checkMate = CheckForCheckmate(val, opposite);
		if (checkMate == true) {
			if (foolmateB == 2) {
				cout << "::FOOLS MATE::\n\n";				//if foolmate
			}
			else
			{
				cout << "::CHECKMATE::\n\n";			//if checkmate
			}
			cout << "::PLAYER WHITE WINS THE GAME::\n\n";
			return 1;
		}
		checkB = check(val, indi, indj, opposite);
		if (checkB == true)
		{						//if player B is in check
			cout << "::PLAYER BLACK IS IN CHECK::\n\n";
		}

		else
		{									//if stalemate
			bool staMate = stalemate(val, opposite);
			if (staMate == true) {
				cout << "::STALEMATE::\n\n";

				//if mode is classic, then compare time
				if (mode == 'C' || mode == 'c')
				{
					if (classicTimerA > classicTimerB)
					{
						cout << "::PLAYER BLACK WINS THE GAME DUE TO TIME::\n";
						return 1;
					}
					else if (classicTimerA < classicTimerB)
					{
						cout << "::PLAYER WHITE WINS THE GAME DUE TO TIME::\n";
						return 1;
					}
				}
				cout << "::GAME DRAW::\n\n";
				return 1;
			}
		}


		char userChoice = '\0';
		if (mode != 'L' && mode != 'l')
		{
		//conditions for draw game
		cout << "::ENTER H FOR HAND SHAKE::\n";
		cout << "::PRESS R FOR RESIGN::\n";
		cin >> userChoice;
		
		//handshake
			char handshakeA = '\0';
			if (userChoice == 'H') {
				cout << "::PLAYER BLACK WANTS TO HANDSHAKE::\n::PRESS H TO HAND SHAKE::\n\n";
				cin >> handshakeA;
			}
				if (handshakeA == 'H' && userChoice == 'H')
				{
					cout << "::HAND SHAKE DONE::\n\n";
					//if mode is classic, then compare time
					if (mode == 'C' || mode == 'c')
					{
						if (classicTimerA > classicTimerB)
						{
							cout << "::PLAYER BLACK WINS THE GAME DUE TO TIME::\n";
							return 1;
						}
						else if (classicTimerA < classicTimerB)
						{
							cout << "::PLAYER WHITE WINS THE GAME DUE TO TIME::\n";
							return 1;
						}
					}
					cout << "::GAME DRAW BY HAND SHAKE::\n\n";
					return 1;
				}
			}



		//check for resign (resign not include in lightening mode)
		if (mode != 'L' && mode != 'l') {
			if (userChoice == 'R' || userChoice == 'r') {
				cout << "::PLAYER BLACK RESIGN::\n\n";
				cout << "::PLAYER WHITE WINS THE GAME::\n\n";
				return 1;
			}
		}


		//games mode
		if (mode == 'L' || mode == 'l') {
			lightningTime();
		}
		else if (mode == 'R' || mode == 'r') {
			rapidTime(rapidTimerB);
		}
		else if (mode == 'B' || mode == 'b') {
			blitzTime(blitzTimerB);
		}
		else if (mode == 'C' || mode == 'c') {
			classicTime(classicTimerB, hourB, minB, secB);
		}

		
		//input source position
		cout << "MOVE FROM: ";
		cin >> col >> rowB;
		
		//convert virtual index of row into actual index of row
		rowB = 8 - rowB;

		//convert alphabets column into integer column using switch
		switch (col) {
		case 'a': columnB = 0; break;
		case 'b': columnB = 1; break;
		case 'c': columnB = 2; break;
		case 'd': columnB = 3; break;
		case 'e': columnB = 4; break;
		case 'f': columnB = 5; break;
		case 'g': columnB = 6; break;
		case 'h': columnB = 7; break;
		default:
		{
			cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
			playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			break;
		}
		}

		cout << "TO: ";
		cin >> desCol >> desRowB;

		//convert virtual index of destination row into actual index of destination row
		desRowB = 8 - desRowB;

		// convert alphabets destination column into integer destination column using switch
		switch (desCol) {
		case 'a': desColumnB = 0; break;
		case 'b': desColumnB = 1; break;
		case 'c': desColumnB = 2; break;
		case 'd': desColumnB = 3; break;
		case 'e': desColumnB = 4; break;
		case 'f': desColumnB = 5; break;
		case 'g': desColumnB = 6; break;
		case 'h': desColumnB = 7; break;
		default:
		{
			cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
			playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			break;
		}
		}

		//store position of last moves
		prevCoordinate[4] = rowB;
		prevCoordinate[5] = columnB;
		prevCoordinate[6] = desRowB;
		prevCoordinate[7] = desColumnB;
	}
	//PLAYER A TURN
	if (turn == 'A')
	{
		checkDestination(opposite);
		if (val[rowA][columnA] == 'P')
		{
			temp = val[desRowA][desColumnA];
			bool found = pawnForWhite(val, rowA, columnA, desRowA, desColumnA, opposite);
			found2 = afterCheck(val, rowA, columnA, desRowA, desColumnA, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerA = rapidTimerA + 10;
				}
				consecutiveMoves = 0;
				foolmateA++;
				replayStore(rowA, col, desRowA, desCol);
				turn = 'B';
			}
		}
		else if (val[rowA][columnA] == 'R')
		{
			checkDestination(opposite);
			bool found = rookForWhite(val, rowA, columnA, desRowA, desColumnA, opposite);
			if (found == true) {
				temp = val[desRowA][desColumnA];
				val[desRowA][desColumnA] = val[rowA][columnA];
				val[rowA][columnA] = ch;
			}
			found2 = afterCheck(val, rowA, columnA, desRowA, desColumnA, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else
			{
				if (mode == 'R' || mode == 'r') {
					rapidTimerA = rapidTimerA + 10;
				}
				consecutiveMoves++;
				foolmateA++;
				replayStore(rowA, col, desRowA, desCol);
				turn = 'B';
			}
		}
		else if (val[rowA][columnA] == 'N')
		{
			bool found = knightForWhite(val, rowA, columnA, desRowA, desColumnA, opposite);
			if (found == true) {
				temp = val[desRowA][desColumnA];
				val[desRowA][desColumnA] = val[rowA][columnA];
				val[rowA][columnA] = ch;
			}
			found2 = afterCheck(val, rowA, columnA, desRowA, desColumnA, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerA = rapidTimerA + 10;
				}
				consecutiveMoves++;
				foolmateA++;
				replayStore(rowA, col, desRowA, desCol);
				turn = 'B';
			}
		}
		else if (val[rowA][columnA] == 'B')
		{
			bool found = bishopForWhite(val, rowA, columnA, desRowA, desColumnA, opposite);
			if (found == true) {
				temp = val[desRowA][desColumnA];
				val[desRowA][desColumnA] = val[rowA][columnA];
				val[rowA][columnA] = ch;
			}
			found2 = afterCheck(val, rowA, columnA, desRowA, desColumnA, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerA = rapidTimerA + 10;
				}
				consecutiveMoves++;
				foolmateA++;
				replayStore(rowA, col, desRowA, desCol);
				turn = 'B';
			}
		}
		else if (val[rowA][columnA] == 'Q')
		{
			bool found = queenForWhite(val, rowA, columnA, desRowA, desColumnA, opposite);
			if (found == true) {
				temp = val[desRowA][desColumnA];
				val[desRowA][desColumnA] = val[rowA][columnA];
				val[rowA][columnA] = ch;
			}
			found2 = afterCheck(val, rowA, columnA, desRowA, desColumnA, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerA = rapidTimerA + 10;
				}
				consecutiveMoves++;
				foolmateA++;
				replayStore(rowA, col, desRowA, desCol);
				turn = 'B';
			}
		}
		else if (val[rowA][columnA] == 'K') {
			bool found = castling(val, rowA, columnA, desRowA, desColumnA, opposite, countAK, countAr_1, countAr_2);
			if (found == true)
			{
				checkDestination(opposite);
				found = kingForWhite(val, rowA, columnA, desRowA, desColumnA, opposite);
				if (found == true) {
					temp = val[desRowA][desColumnA];
					val[desRowA][desColumnA] = val[rowA][columnA];
					val[rowA][columnA] = ch;
				}
				found2 = afterCheck(val, rowA, columnA, desRowA, desColumnA, temp, opposite);
				if (found == false || found2 == false)
				{
					cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
					playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
				}
				else {
					if (mode == 'R' || mode == 'r') {
						rapidTimerA = rapidTimerA + 10;
					}
					consecutiveMoves++;
					foolmateA++;
					replayStore(rowA, col, desRowA, desCol);
					countAK++;
					if (columnA == 0)
						countAr_1++;
					else if (columnA == 7)
						countAr_2++;
					turn = 'B';
				}
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerA = rapidTimerA + 10;
				}
				consecutiveMoves++;
				foolmateA++;
				replayStore(rowA, col, desRowA, desCol);
				countAK++;
				if (columnA == 0)
					countAr_1++;
				else if (columnA == 7)
					countAr_2++;
				turn = 'B';
			}
		}
		else {
			cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
			playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
		}
	}
	else if (turn == 'B')
	{
		checkDestination(opposite);
		if (val[rowB][columnB] == 'p')
		{
			temp = val[desRowB][desColumnB];
			bool found = pawnForBlack(val, rowB, columnB, desRowB, desColumnB, opposite);
			found2 = afterCheck(val, rowB, columnB, desRowB, desColumnB, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerB = rapidTimerB + 10;
				}
				consecutiveMoves = 0;
				foolmateB++;
				replayStore(rowB, col, desRowB, desCol);
				turn = 'A';
			}
		}
		else if (val[rowB][columnB] == 'r')
		{
			checkDestination(opposite);
			bool found = rookForWhite(val, rowB, columnB, desRowB, desColumnB, opposite);
			if (found == true) {
				temp = val[desRowB][desColumnB];
				val[desRowB][desColumnB] = val[rowB][columnB];
				val[rowB][columnB] = ch;
			}
			found2 = afterCheck(val, rowB, columnB, desRowB, desColumnB, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerB = rapidTimerB + 10;
				}
				consecutiveMoves++;
				foolmateB++;
				replayStore(rowB, col, desRowB, desCol);
				turn = 'A';
			}
		}
		else if (val[rowB][columnB] == 'n')
		{
			bool found = knightForWhite(val, rowB, columnB, desRowB, desColumnB, opposite);
			if (found == true) {
				temp = val[desRowB][desColumnB];
				val[desRowB][desColumnB] = val[rowB][columnB];
				val[rowB][columnB] = ch;
			}
			found2 = afterCheck(val, rowB, columnB, desRowB, desColumnB, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerB = rapidTimerB + 10;
				}
				consecutiveMoves++;
				foolmateB++;
				replayStore(rowB, col, desRowB, desCol);
				turn = 'A';
			}
		}
		else if (val[rowB][columnB] == 'b')
		{
			bool found = bishopForWhite(val, rowB, columnB, desRowB, desColumnB, opposite);
			if (found == true) {
				temp = val[desRowB][desColumnB];
				val[desRowB][desColumnB] = val[rowB][columnB];
				val[rowB][columnB] = ch;
			}
			found2 = afterCheck(val, rowB, columnB, desRowB, desColumnB, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerB = rapidTimerB + 10;
				}
				consecutiveMoves++;
				foolmateB++;
				replayStore(rowB, col, desRowB, desCol);
				turn = 'A';
			}
		}
		else if (val[rowB][columnB] == 'q')
		{
			bool found = queenForWhite(val, rowB, columnB, desRowB, desColumnB, opposite);
			if (found == true) {
				temp = val[desRowB][desColumnB];
				val[desRowB][desColumnB] = val[rowB][columnB];
				val[rowB][columnB] = ch;
			}
			found2 = afterCheck(val, rowB, columnB, desRowB, desColumnB, temp, opposite);
			if (found == false || found2 == false)
			{
				cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
				playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerB = rapidTimerB + 10;
				}
				consecutiveMoves++;
				foolmateB++;
				replayStore(rowB, col, desRowB, desCol);
				turn = 'A';
			}
		}
		else if (val[rowB][columnB] == 'k')
		{
			bool found = castling(val, rowB, columnB, desRowB, desColumnB, opposite, countBk, countBr_1, countBr_2);
			if (found == true) {
				checkDestination(opposite);
				found = kingForWhite(val, rowB, columnB, desRowB, desColumnB, opposite);
				if (found == true) {
					temp = val[desRowB][desColumnB];
					val[desRowB][desColumnB] = val[rowB][columnB];
					val[rowB][columnB] = ch;
				}
				found2 = afterCheck(val, rowB, columnB, desRowB, desColumnB, temp, opposite);
				if (found == false || found2 == false)
				{
					cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
					playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
				}
				else {
					if (mode == 'R' || mode == 'r') {
						rapidTimerB = rapidTimerB + 10;
					}
					consecutiveMoves++;
					foolmateB++;
					replayStore(rowB, col, desRowB, desCol);
					countBk++;
					if (columnB == 0)
						countBr_1++;
					else if (columnB == 7)
						countBr_2++;
					turn = 'A';
				}
			}
			else {
				if (mode == 'R' || mode == 'r') {
					rapidTimerB = rapidTimerB + 10;
				}
				consecutiveMoves++;
				foolmateB++;
				replayStore(rowB, col, desRowB, desCol);
				countBk++;
				if (columnB == 0)
					countBr_1++;
				else if (columnB == 7)
					countBr_2++;
				turn = 'A';
			}
		}
		else {
			cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
			playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
		}
	}
	else
	{
		cout << "\t\t\t\t\t ****************| Invalid Choice |*****************\n";
		playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
	}

	return 0;   //if there is no mate
}  // end of player Turn

int main() {
up:
	system("cls");
	char val[8][8] = {
					   {'r','n','b','q','k','b','n','r'},
					   {'p','p','p','p','p','p','p','p'},
					   {' ',' ',' ',' ',' ',' ',' ',' '},
					   {' ',' ',' ',' ',' ',' ',' ',' '},
					   {' ',' ',' ',' ',' ',' ',' ',' '},
					   {' ',' ',' ',' ',' ',' ',' ',' '},
					   {'P','P','P','P','P','P','P','P'},
					   {'R','N','B','Q','K','B','N','R'}
	};
	int n = 1;
	int mate = 0;
	
	//counts for castling
	int countAK = 1, countAr_1 = 1, countAr_2 = 1, countBk = 1, countBr_1 = 1, countBr_2 = 1;


	//mode input
	cout << "::ENTER C FOR CLASSICAL MODE::\n";
	cout << "::ENTER R FOR RAPID MODE::\n";
	cout << "::ENTER B FOR BLITZ MODE::\n";
	cout << "::ENTER L FOR LIGHTENING MODE::\n";
	cin >> mode;
	
	
	if (mode == 'L' || mode == 'l' || mode == 'R' || mode == 'r' || mode == 'B' || mode == 'b' || mode == 'C' || mode == 'c')
	{

		//print board
		//display(val);


		//for classical Mode
		while (mate == 0) {


			//print board
			display(val);

			char srcRow, srcCol, desRow, desCol;
			int row=9, column=9, destinationRow=9, desColumn=9;
			char board[8][8] = {
				   {'r','n','b','q','k','b','n','r'},
				   {'p','p','p','p','p','p','p','p'},
				   {' ',' ',' ',' ',' ',' ',' ',' '},
				   {' ',' ',' ',' ',' ',' ',' ',' '},
				   {' ',' ',' ',' ',' ',' ',' ',' '},
				   {' ',' ',' ',' ',' ',' ',' ',' '},
				   {'P','P','P','P','P','P','P','P'},
				   {'R','N','B','Q','K','B','N','R'}
			};


			//input what user want
			char savetype, dor;
			cout << "::WHICH FILE DO YOU WANT TO OPEN::" << endl << endl;
			cout << "::PRESS 0 FOR NEW GAME::" << endl;
			cout << "::PRESS 1 FOR LOADING FILE 1::" << endl;
			cout << "::PRESS 2 FOR LOADING FILE 2::" << endl;
			cout << "::PRESS 3 LOADING FILE 3::" << endl;
			cout << "::PRESS 4 TO CONTINUE::" << endl;
			cout << "::PRESS S TO SAVE GAME::\n";
			cin >> savetype;

			//if user wants to load the saved games
			if (savetype == '1' || savetype == '2' || savetype == '3') {
				//counts for castling
				cout << "::TO OPEN DIRECTLY PRESS 1::" << endl;
				cout << "::TO SHOW REPLAY PRESS 2::" << endl;
				cin >> dor;
				Sleep(2000);
				if (savetype == '1') {
					display(board);
					ifstream MyFile("save1.txt");
					countAK = 1, countAr_1 = 1, countAr_2 = 1, countBk = 1, countBr_1 = 1, countBr_2 = 1;
					while (!MyFile.eof()) {
						MyFile >> srcCol >> srcRow >> desCol >> desRow;
						int  column = SWITCH(srcCol);
						int row = rowSwitch(srcRow);

						int  desColumn = SWITCH(desCol);
						int destinationRow = rowSwitch(desRow);
						
						if (MyFile.eof())
							break;
						
						replaymove(board, row, column, destinationRow, desColumn);
						system("cls");
						
						//if replay
						if (dor == '2' && !_kbhit()) {
							display(board);
							Sleep(2000);
						}
						else
							display(board);
						if (turn == 'A')
							turn = 'B';
						else
							turn = 'A';
					}
				}
				else if (savetype == '2') {
					display(board);
					ifstream MyFile("save2.txt");
					countAK = 1, countAr_1 = 1, countAr_2 = 1, countBk = 1, countBr_1 = 1, countBr_2 = 1;
					while (!MyFile.eof()) {
						MyFile >> srcCol >> srcRow >> desCol >> desRow;
						int  column = SWITCH(srcCol);
						int row = rowSwitch(srcRow);

						int  desColumn = SWITCH(desCol);
						int destinationRow = rowSwitch(desRow);

						if (MyFile.eof())
							break;

						replaymove(board, row, column, destinationRow, desColumn);
						system("cls");

						//if replay
						if (dor == '2' && !_kbhit()) {
							display(board);
							Sleep(2000);
						}

						else
							display(board);
						if (turn == 'A')
							turn = 'B';
						else
							turn = 'A';
					}
				}
				else if (savetype == '3') {
					display(board);
					ifstream MyFile("save3.txt");
					countAK = 1, countAr_1 = 1, countAr_2 = 1, countBk = 1, countBr_1 = 1, countBr_2 = 1;
					while (!MyFile.eof()) {
						MyFile >> srcCol >> srcRow >> desCol >> desRow;
						int  column = SWITCH(srcCol);
						int row = rowSwitch(srcRow);

						int  desColumn = SWITCH(desCol);
						int destinationRow = rowSwitch(desRow);

						if (MyFile.eof())
							break;

						replaymove(board, row, column, destinationRow, desColumn);
						system("cls");

						//if replay
						if (dor == '2' && !_kbhit()) {
							display(board);
							Sleep(2000);
						}
						else
							display(board);
						if (turn == 'A')
							turn = 'B';
						else
							turn = 'A';
					}
				}
			}
				else if (savetype == '0') {
					goto up;
			}
				else if (savetype == 'S') {
				cout << "::PRESS 1 TO SAVE IN FILE 1::\n";
				cout << "::PRESS 2 TO SAVE IN FILE 2::\n";
				cout << "::PRESS 3 TO SAVE IN FILE 3::\n";
				cin >> fileChoice;
				if(fileChoice=='1'){
					ofstream saveFile1;
					saveFile1.open("save1.txt");
					int size = strlen(saveMoves);
					for (int i = 0; i < size; i++) {
						saveFile1 << saveMoves[i];
					}
				}
				else if (fileChoice == '2') {
					ofstream saveFile2;
					saveFile2.open("save2.txt");
					int size = strlen(saveMoves);
					for (int i = 0; i < size; i++) {
						saveFile2 << saveMoves[i];
					}
				}
				if (fileChoice == '3') {
					ofstream saveFile3;
					saveFile3.open("save3.txt");
					int size = strlen(saveMoves);
					for (int i = 0; i < size; i++) {
						saveFile3 << saveMoves[i];
					}
				}
				}
			if (savetype == '1' || savetype == '2' || savetype == '3') {
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						val[i][j] = board[i][j];
					}
				}
			}

			system("cls");
			

			//print board
			display(val);


			mate = playerTurn(val, checkA, checkB, countAK, countAr_1, countAr_2, countBk, countBr_1, countBr_2);
			if (mate != 0)
				break;
			system("cls");

		}//loop end

	}// if end
	
	else
		cout << "::INVALID INPUT::\n";
	return 0;
}

