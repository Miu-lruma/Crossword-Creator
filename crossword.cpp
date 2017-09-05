//Randall Krueger
//Fundamentals of Computing Lab 9
//Crossword Anagram
//Program that uses functions to create a crossword puzzle out of words given by the user.  Clues are output in the form of an anagram.
//11/20/16

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <fstream>

using namespace std;

//Initializing Functions
bool compareSize(const string& a, const string& b);
void printBoard(char board[15][15]);
void printFile(char board[15][15], string);
void addWordH(string, int, int, char board[15][15]);
void addWordV(string, int, int, char board[15][15]);
int canPlace(string, int, int, char board[15][15], int);


int main(int argc, char* argv[]) {

	//Initializing Variables
	vector<string> words(400);
	vector<string> addedWords;
	vector<string> initWords;
	vector<string> clues;
	vector<string> inputwords;
	string tempword, dirclue, anaclue;
	int count = 0, tempvar = 0, found = 0, found2, distance=0, placeable=0, xclue, yclue, validword;
	char board[15][15];
	char blankBoard[15][15];
	char fileinput;

	//Checks to see if the user input too many arguments
	if (argc > 3) {
		cout << "Too many arguments." << endl;
		return 0;
	}

	//Working with command line arguments
	if (argc == 2 || argc == 3) {
		ifstream ifs(argv[1]);

		//Make sure the file is valid
		if (!ifs) {
			cout << "Error opening file " << argv[1] << endl;
			return 0;
		}

		//Placing words from file into a vector of strings
		while (fileinput != EOF) {
			getline(ifs, tempword, '\n');
			inputwords.push_back(tempword);
			fileinput = ifs.peek();
		}
		inputwords.push_back(".");
	}


	//Making blank board
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			board[i][j] = 178; //178 denotes an ascii character that is a full white block
		}
	}
	if (argc == 1) {
		cout << "Welcome to the crossword anagram program!" << endl << endl << "Please enter the words that you want to be in the puzzle (up to 20 words, each no more than 15 letters)" << endl <<"Enter ""."" when you are finished." << endl << endl;
	}
	//Entering Words
	while (count < 20) {
		validword = 1;
		if (argc == 1) {
			cout << "Your word: ";
		}

		//Allows user to input words if there are no arguments
		if (argc == 1) {
			cin >> tempword;
		}

		//Allows the program to get information from a file if passed
		if (argc == 2 || argc == 3) {
			tempword = inputwords[count];
		}

		//Will stop the loop if the user enters "."
		if (tempword.compare(".") == 0) {
			count = 20;
			validword = 0;
		}
		//Checks for alphabetical input and proper size, disallowing the word if it doesn't meet the requirements
		if (tempword.compare(".") != 0) {
			if (tempword.size() > 15) {
				cout << "Please only enter words of 15 letters or less." << endl;
				validword = 0;
			}
			for (auto i = tempword.begin(); i != tempword.end(); i++) {
				if (isalpha(*i) == 0) {
					if (validword == 1) {
						cout << "Please only enter letters." << endl;
					}	  
					validword = 0;
				}
				*i = toupper(*i);
			}
			//Places words into the changing words vector and the static initial words vectors
			if (validword == 1) {
				words.push_back(tempword);
				initWords.push_back(tempword);
			}      
		}
		count++;
	}
	cout << endl;

	//Sorts words by size
	sort(words.begin(), words.end(), compareSize);


	//Debugging show of all words
	for (auto i = words.begin(); i != words.end(); i++) {
	}

	//Printing the first word in the center and moving it to the addedWords vector
	tempword = words.front();
	tempvar = 15 - tempword.length();
	tempvar = tempvar / 2;
	addWordH(tempword, tempvar, 7,  board);
	addedWords.push_back(tempword);
	words.erase(words.begin());

	//Acquiring values for the clue
	xclue = tempvar + 1;
	yclue = 8;
	dirclue = "Across";
	anaclue = tempword;
	random_shuffle(anaclue.begin(), anaclue.end());

	//Making the string for the first entry in the clues vector
	ostringstream stream;
	stream << xclue << ", " << yclue << "\t" << dirclue << "\t" << anaclue;
	string clue = stream.str();
	clues.push_back(clue);

	//Loop that places words into the board
	for (auto i = words.begin(); i != words.end(); i++) {
		found = 0;
		tempword = *i;
		for (int j = 0; j < 15; j++) {
			for (int k = 0; k < 15; k++) {
				//Finding matching letters on the board
				for (auto it = tempword.begin(); it != tempword.end(); it++) {
					if (found == 0) {	       
						if (*it == board[j][k]) {
							//Finding the position of the first letter of the word and running the canPlace function to find out if the word can be placed in this position
							distance = it - tempword.begin();
							placeable =  canPlace(tempword, k, j, board, distance);
							if (placeable == 1) {
								//Adding the word vertically if the canPlace function returns 1
								addWordV(tempword, k, (j-distance), board);
								//Forming the clue for vertical words
								xclue = k + 1;
								yclue = j - distance + 1;
								dirclue = "Down";
								anaclue = tempword;
								random_shuffle(anaclue.begin(), anaclue.end());

								//Adding the clue to the clues string
								ostringstream stream;
								stream << xclue << ", " << yclue << "\t" << dirclue << "\t" << anaclue;
								string clue = stream.str();
								clues.push_back(clue);
								//Placing the word in the added words vector and telling the program that a position was found
								addedWords.push_back(tempword);
								found = 1;
							}
							//Follows the same steps as the if placeable == 1 code, but for horizontal words
							if (placeable == 2) {
								addWordH(tempword, (k-distance), j, board);

								xclue = k - distance + 1;
								yclue = j + 1;
								dirclue = "Across";
								anaclue = tempword;
								random_shuffle(anaclue.begin(), anaclue.end());

								ostringstream stream;
								stream << xclue << ", " << yclue << "\t" << dirclue << "\t" << anaclue;
								string clue = stream.str();
								clues.push_back(clue);

								addedWords.push_back(tempword);
								found = 1;
							}
						}
					}
				}
			}
		}

		//If a position for the word is not found
		if (found == 0) {
			found2 = 0;
			//Checks to see how many times the word has already been cheecked, as every time the word is not found it is re-added to the end of the words vector
			for (auto kk = words.begin(); kk != words.end(); kk++) {
				if (tempword.compare(*kk) == 0) {
					found2++;
				}
				//If the program has tried to place the word 20 times already, it gives up. otherwise, the word is placed at the end of the words vector, to check it again once more words have been placed
			}
			if (found2 < 20) {
				words.push_back(tempword);
			}
		}
	}

	//Creates a board with blanks instead of words
	for (int j = 0; j < 15; j++) {
		for (int k = 0; k < 15; k++) {
			if (board[j][k] == -78) {
				blankBoard[j][k] = 178;
			}
			if (board[j][k] != -78) {
				blankBoard[j][k] = '_';
			}
		}
	}

	//Compares the initial words to the added words to determine which words could not be added before displaying the words to the user
	for (auto kk = initWords.begin(); kk != initWords.end(); kk++) {
		found = 0;
		tempword = *kk;
		for (auto jj = addedWords.begin(); jj != addedWords.end(); jj++) {
			if (tempword.compare(*jj) == 0) {
				found = 1;
			}
		}
		if (found == 0) {
			//If an outfput file is passed, output is sent to the file rather than to the console
			if (argc == 1 || argc == 2) {
				cout << "Unable to add word: " << tempword << endl;
			}
			if (argc == 3) {
				ofstream ofs;
				ofs.open (argv[2], ios::app);
				ofs << "Unable to add word: " << tempword << endl;
				ofs.close();
			}
		}
	}

	if (argc == 1 || argc == 2) {
		//Prints the solution, blank puzzle, and clues
		cout << endl << "Solution:" << endl << endl;
		printBoard(board);
		cout << endl << "Crossword Puzzle: " << endl << endl;
		printBoard(blankBoard);
		cout << endl << "Clues:" << endl << endl;
		for (auto kk = clues.begin(); kk != clues.end(); kk++) {
			cout << *kk << endl;
		}
		cout << endl;
	}

	//This is, again, the program sending output to a file rather than to the console.Because a function is run, the file is opened and closed so that main and the function don't interfere with one another.
	if (argc == 3) {
		ofstream ofs;
		ofs.open (argv[2], ios::app);
		ofs << endl;
		ofs << "Solution:" << endl << endl;
		ofs.close();
		printFile(board, argv[2]);
		ofs.open (argv[2], ios::app);
		ofs << endl << "Crossword Puzzle: " << endl << endl;
		ofs.close();
		printFile(blankBoard, argv[2]);
		ofs.open(argv[2], ios::app);
		ofs << endl << "Clues:" << endl << endl;
		for (auto kk = clues.begin(); kk != clues.end(); kk++) {
			ofs << *kk << endl;
		}
		ofs.close();

	}
}

//Function for comparing size of strings
bool compareSize(const string& a, const string& b) {
	return (a.size() > b.size());
}

//Function that outputs the board passed into it
void printBoard(char board[15][15]) {
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			cout << board[i][j];
		}
		cout << endl;
	}
}

//Function that prints the output to a file.
void printFile(char board[15][15], string filename) {
	string blank = "▒";
	ofstream ofs;
	ofs.open(filename, ios::app);
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {

			if (board[i][j] < 65 || board[i][j] > 90) {
				ofs << blank;
			}         
			if (board[i][j] > 64 && board[i][j] < 91) {
				ofs << board[i][j];
			}
		}
		ofs << endl;
	}
	ofs.close();
}

//Function that adds horizontal word
void addWordH(string word, int x, int y, char board[15][15]) {
	for (auto i = word.begin(); i != word.end(); i++) {
		board[y][x] = *i;
		x++;
	}
}

//Function that adds vertical word
void addWordV(string word, int x, int y, char board[15][15]) {
	for (auto i = word.begin(); i != word.end(); i++) {
		board[y][x] = *i;
		y++;
	}
}

//Function that checks to see if the word can be placed in the position
int canPlace(string word, int x, int y, char board[15][15], int distance) {
	int asc = 0, asc2 = 0;
	int newx = x;
	int newy = y - distance;

	//Vertical position check
	//Checking to see if the word will go off of the board
	if (newy > -1 && (word.length() - distance - 1 + y) < 15) {
		if ((board[(newy - 1)][newx]) == -78 || (newy - 1) == -1) { //"-78" is the ascii reference for the blank character in the board.  It is used multiple times throughout this function
			if ((board[(word.length() - distance + y)][newx]) == -78 || (word.length() - distance + y) == 15) { 
				for ( int i = 0; i < word.length(); i++) {

					//Checks to see if the space to the left and right of the word is blank, save when the word crosses another
					if ((newx + 1) < 15) {
						if (i != distance){
							asc = (int)board[(newy + i)][(newx + 1)];
							asc2 = (int)board[(newy + i)][newx];
							if (asc2 == -78) {
								if (asc != -78) {
									return 0;
								}
							}
						}
					}

					if ((newx - 1) > -1) {
						if (i != distance) {
							asc = (int)board[(newy + i)][(newx - 1)];
							asc2 = (int)board[(newy + i)][newx];
							if (asc2 == -78) {
								if (asc  != -78) {
									return 0;
								}
							}
						}
					}

					//Checks to see if when the word crosses another word, they share a letter
					asc = (int)board[(newy+i)][(newx)];
					if (asc != word.at(i) && asc != -78) {
						return 0;
					}
				}	   
				//If the word can be placed vertically, a 1 is returned 
				return 1;
			}
		}
	}

	//Horizontal position check, follows the same structure as the vertical check
	asc = 0;
	newx = x - distance;
	newy = y;

	if (newx > -1 && (word.length() - distance - 1 + x) < 15) {
		if ((board[(newy)][newx - 1]) == -78 || (newx - 1) == -1) {
			if ((board[newy][(word.length() - distance + x)]) == -78 || (word.length() - distance + x) == 15) {
				for ( int i = 0; i < word.length(); i++) {

					if ((newy + 1) < 15) {
						if (i != distance){
							asc2 = (int)board[newy][(newx + i)];
							asc = (int)board[(newy + 1)][(newx + i)];
							if (asc2 == -78) {
								if (asc != -78) {
									return 0;
								}
							}		  
						}
					}
					if ((newy -1) > -1) {
						if (i != distance) {
							asc = (int)board[(newy - 1)][(newx + i)];
							asc2 = (int)board[newy][(newx + i)];
							if (asc2 == -78) {
								if (asc  != -78) {
									return 0;
								}		  
							}
						}
					}

					asc = (int)board[(newy)][(newx+i)];
					if (asc != word.at(i) && asc != -78) {
						return 0;
					}
				}
				//If the word fits horizontally and there are no obstructions, a 2 is returned
				return 2;
			}
		}
	}
	return 0;
}
