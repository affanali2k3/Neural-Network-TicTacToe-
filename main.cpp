#include <iostream>
#include "NeuralNetwork.h"
#include <vector>
#include <fstream>
#include<string>
using namespace std;
// This function finds the winning combination in the data file for one line
void checkWinningMove(vector <double> array, int &index1, int&index2, int&index3, bool &finalMove);


class tictactoe{
    // Input Values store the values to feed to the neural network
    vector<double>inputValues;
    vector<double>targetValues;

    public:
    double board[9];

    // This function checks who won the game
    int check(double array[9])
    {
        int row1 = array[0] + array[1] + array[2];
        int row2 = array[3] + array[4] + array[5];
        int row3 = array[6] + array[7] + array[8];

        int col1 = array[0] + array[3] + array[6];
        int col2 = array[1] + array[4] + array[7];
        int col3 = array[2] + array[5] + array[8];

        int diag1 = array[0] + array[4] + array[8]; 
        int diag2 = array[2] + array[4] + array[6];
        if(row1 == -3 || row2 == -3 || row3 == -3 || col1 == -3 || col2 == -3 || col3 == -3 || diag1 == -3 || diag2 == -3)
            return -1;   
        else if(row1 == 3 || row2 == 3 || row3 == 3 || col1 == 3 || col2 == 3 || col3 == 3 || diag1 == 3 || diag2 == 3)
            return 1;
        else
            for(int i = 0; i < 9; i++)
            {
                if(array[i] == 0.01)
                    return 0;
            }
    return 2;
    }   

// Variables used for random play
int computerWin = 0;
int randomWin = 0;
int tie = 0;

// Function to play computer against the rand function
void playGame(double resultArray[9], NeuralNetwork NET)
{


for(int games = 0; games < 500; games++)
{
    for(int i = 0; i < 9; i++)
    {
        resultArray[i] = -2;
        board[i] = 0.01;
    }
    while(1)
    {
        inputValues.clear();
        targetValues.clear();
        int index;
        for(int i = 0; i < 9; i++)
        {
            inputValues.push_back(board[i]);
        }
        double greatest = 0;
        int j = 0;
        for(int i = 0; i < 9; i++)
        {
            if(board[i] == 0.01)
            {
                inputValues[i] = -1;
                NET.feedForward(inputValues);
                resultArray[i] = NET.getResult();
                inputValues[i] = 0.01;
            }
            else
            {
                resultArray[i] = -2;
            }
            }

            for(int i = 0; i < 9; i++)
            {
                if(resultArray[i] > greatest)
                {
                    greatest = resultArray[i];
                }
            }

            for(int i = 0; i < 9; i++)
            {
            if(greatest == resultArray[i])
            {
                j = i;
                break;
            }
            }

            board[j] = -1;

            int randomPlayer;
            do
            {
                randomPlayer = rand() % 9;     
            }
            while(board[randomPlayer] == -1);
            board[randomPlayer] = 1;

            if(check(board) == -1)
            {
                computerWin++;
                break;
            }
            else if(check(board) == 1)
            {
                randomWin++;
                break;
            }
            else if(check(board) == 2)
            {
                tie++;
                break;
            }
        }
    }
}


void humanMove()
{
    int index;
    cout << "Enter index\n";
    cin >> index;
    
    board[index-1] = 1;

        if(check(board) == -1)
        {
            cout << "Computer win\n";
            
        }
        else if(check(board) == 1)
        {
            cout << "You win\n";
            
        }
        else if(check(board) == 2)
        {
            cout << "Its a tie\n";
            
        }
}

void getCount()
{
    cout << "Computer wins = " << computerWin << endl;
    cout << "Random wins = " << randomWin << endl;
    cout << "Ties = " << tie << endl;
}


void printBoard()
{
 for(int i = 0; i < 9; i++)
        {
            if(i%3!=0)
                cout << "|   ";
            if(i%3==0 && i != 0)
            {
                cout << endl;
                cout << "_______________________\n";
            }
            if(board[i] == 1)
                cout << 'X' << "\t";
            else if(board[i] == -1)
                cout << 'O' << '\t';
            else
                cout << ' ' << '\t';

        }


}
void clear()
{
    for(int i=0;i<9;i++)
    {
        board[i]=0.01;
    }
}

};


int main()
{
    tictactoe t;

    double resultArray[9];
    // The structure of Neural Network
    vector <int> topology;
    vector <double> inputValues;
    vector <double> targetValues;

    // 2D vector to store all lines of data from file
    vector <vector <double>> fileData;
    // Vector to store one line from the data file
    vector <double> lineData;

    topology.push_back(9);
    topology.push_back(5);
    topology.push_back(1);
    NeuralNetwork NET(topology);

    ifstream trainingData("ticktacktoe.txt");
    string line;
    int k = 0;

    while(!trainingData.eof())
    {
        lineData.clear();
        getline(trainingData, line);
        for(int i = 0; i < 9; i++)
        {
            if(line[i] == 'x')
                lineData.push_back(-1);
            else if(line[i] == 'o')
                lineData.push_back(1);
            else if(line[i] == 'b')
                lineData.push_back(0.01);
            else if(line[i] == 'p')
                lineData.push_back(1);
            else
                lineData.push_back(0);
        }

        fileData.push_back(lineData); 
    }

    for(int trainingCount= 0; trainingCount < 100000; trainingCount++)
    {
        inputValues.clear();

        for(int i = 0; i < 9; i++)
        {
            inputValues.push_back(0.01);
        }

        // Indexes to store the winning move from the file
        int index1=0;
        int index2=0;
        int index3=0;

        // Stores the previous human move from the file
        int previousIndex = 0;

        // Trains a random line
        int randomLine = (rand() % 957) + 1;
        bool temp = false;

        checkWinningMove(fileData[randomLine], index1, index2, index3, temp);

        // Places the first computer move from the file
        for(int i = 0; i < 9; i++)
        {
            if(fileData[randomLine][i] == -1)
            {
                inputValues[i] = -1;
                break;
            }
        }

        // Plays the whole game according to file and trains the neural network with each iteration
        for(int l = 0; l < 5; l++)
        {
            double tempArray[9];
            targetValues.clear();

            // Human move placed according to the data file
            for(int i = previousIndex+1; i < 9; i++)
            {
                // For negative cases
                if(randomLine > 626)
                {
                    if(inputValues[index1] == 0.01)
                        inputValues[index1] = 1;
                    else if(inputValues[index2] == 0.01)
                        inputValues[index2] = 1;
                    else if(inputValues[index3] == 0.01)
                        inputValues[index3] = 1;
                    break;
                }
                // For positive cases
                else
                {
                    if(fileData[randomLine][i] == 1)
                    {
                    inputValues[i] = 1;
                    previousIndex = i;
                    break;
                    }
                }
            }

            // temporary array used to pass to the check function as an array
            for(int i = 0; i < 9; i++)
            {
                tempArray[i] = inputValues[i];
            }

            // Stops the training for one line of data when a game is finished
            if(t.check(tempArray) == -1)
                break;
            if(t.check(tempArray) == 1)
                break;

            // Makes a computer move to a preferable position to keep the game going for maximum training
            int moveIndex = 0;
            for(int i = 0; i < 9; i++)
            {
                // tempIndex3 is the index for winning move
                int tempIndex1 = 0;
                int tempIndex2 = 0;
                int tempIndex3 = 0;
                bool finalMove = false;


                targetValues.clear();

                // Computer places a move at empty place and later checks if the move was good or bad
                if(inputValues[i] == 0.01)
                {
                    checkWinningMove(inputValues, tempIndex1, tempIndex2, tempIndex3, finalMove);
                    inputValues[i] = -1;

                    // Feedimg the new data
                    NET.feedForward(inputValues);

                    // If the move is good then backpropogation with target 1 else backporpogation with target 0
                    if(finalMove == true && i == tempIndex3)
                    {
                        moveIndex = i;
                        targetValues.push_back(1);
                        NET.backwardPropogation(targetValues);
                    }
                    else if(finalMove == true && i != tempIndex3)
                    {
                        if(i != index1 || i != index2)
                        {
                        targetValues.push_back(0);
                        NET.backwardPropogation(targetValues);
                        }
                        
                    }
                    else if((i == index1 || i == index2 || i == index3) & randomLine > 627)
                    {
                        moveIndex = i;
                        targetValues.push_back(1);
                        NET.backwardPropogation(targetValues);
                    }
                    else if((i != index1 || i != index2 || i!= index3) & randomLine > 627)
                    {
                        targetValues.push_back(0);
                        NET.backwardPropogation(targetValues);
                    }
                    inputValues[i] = 0.01;
                }
            }

            // Places a move for further training
            if(randomLine <= 626)
                inputValues[moveIndex] = -1;

        }
    }

 
    // Choice to continue or exit playing the game
    char choice;
    do
    {
        t.playGame(resultArray,NET);
        t.getCount();
        t.clear();

        for(int i = 0; i < 9; i++)
        {
            resultArray[i] = -2;
         
        }

        while(1)
        {
            inputValues.clear();
            targetValues.clear();

            // Index for computer move
            int index = 0;

            for(int i = 0; i < 9; i++)
            {
                inputValues.push_back(t.board[i]);
            }

            double greatest = 0;

            // Places a computer move at every empty index and checks for the highest probability
            for(int i = 0; i < 9; i++)
            {
                if(t.board[i] == 0.01)
                {
                    inputValues[i] = -1;
                    NET.feedForward(inputValues);
                    resultArray[i] = NET.getResult();
                    inputValues[i] = 0.01;
                }
                else
                {
                    resultArray[i] = -2;
                }
            }

            for(int i = 0; i < 9; i++)
            {
                cout << "Probability at index(" << i << ") = " << resultArray[i] << endl;
                if(resultArray[i] > greatest)
                {
                    greatest = resultArray[i];
                }
            }

            for(int i = 0; i < 9; i++)
            {
                if(greatest == resultArray[i])
                {
                    index = i;
                    break;
                }
            }

            // Computer move
            t.board[index] = -1;

            // Checks who won the game
            if(t.check(t.board)==1)
            {
                cout<<"you win \n";
                break;
            }
            if(t.check(t.board)==-1)
            {
                cout<<"computer win\n";
                break;
            }
            if(t.check(t.board)==2)
            {
                cout<<"its a tie\n";
                break;
            }


             t.printBoard();

            t.humanMove();


            if(t.check(t.board)==1)
            {
            cout<<"you win \n";
                break;
            }
            if(t.check(t.board)==-1)
            {
            cout<<"computer win\n";
                break;
            }
            if(t.check(t.board)==2)
            {
            cout<<"its a tie\n";
                break;
            }
        
            }
        
            cout << "Do you want to continue\n";
            cin >> choice;
    }
    while(choice != 'n' || choice != 'N');
    return 0;
}


void checkWinningMove(vector <double> array, int &index1, int&index2, int&index3, bool &finalMove)
{
    for(int i = 0; i <9; i++)
    {
        if(array[i] == 0.01)
            array[i] = 0;
    }
    int row1 = array[0] + array[1] + array[2];
    int row2 = array[3] + array[4] + array[5];
    int row3 = array[6] + array[7] + array[8];

    int col1 = array[0] + array[3] + array[6];
    int col2 = array[1] + array[4] + array[7];
    int col3 = array[2] + array[5] + array[8];

    int diag1 = array[0] + array[4] + array[8]; 
    int diag2 = array[2] + array[4] + array[6];


    if(row1 == -2 || row1 == 2)
    {
        finalMove = true;
        for(int i = 0; i < 3; i++)
        {
            if(array[i] == 0)
                index3 = i;
        }
    }

    else if(row2 == -2 || row2 == 2)
    {
        finalMove = true;
        for(int i = 0; i < 3; i++)
        {
            if(array[i+3] == 0)
                index3 = i+3;
        }
    }

    else if(row3 == -2 || row3 == 2)
    {
        finalMove = true;
        for(int i = 0; i < 3; i++)
        {
            if(array[i+6] == 0)
                index3 = i+6;
        }
    }
    
    else if(col1 == -2 || col1 == 2)
    {
        finalMove = true;
        for(int i = 0; i < 3; i++)
        {
            if(array[3*i] == 0)
                index3 = 3*i;
        }
    }
    else if(col2 == -2 || col2 == 2)
    {
        finalMove = true;
        for(int i = 0; i < 3; i++)
        {
            if(array[3*i + 1] == 0)
                index3 = (3*i)+1;
        }
    }
    else if(col3 == -2 || col3 == 2)
    {
        finalMove = true;
        for(int i = 0; i < 3; i++)
        {
            if(array[3*i + 2] == 0)
                index3 = (3*i)+2;
        }
    }
    else if(diag1 == -2 || diag1 == 2)
    {
        finalMove = true;
        for(int i = 0; i < 3; i++)
        {
            if(array[4*i] == 0)
                index3 = (4*i);
        }
    }
    else if(diag2 == -2 || diag2 == 2)
    {
        finalMove = true;
        for(int i = 0; i < 3; i++)
        {
            if(array[2*i + 2] == 0)
                index3 = (2*i)+2;
        }
    }
    
    if(row1 == -3 || row1 == 3)
    {
        index1 = 0;
        index2 = 1;
        index3 = 2;
    }
    else if(row2 == -3 || row2 == 3)
    {
        index1 = 3;
        index2 = 4;
        index3 = 5;
    }
    else if(row3 == -3 || row3 == 3)
    {
        index1 = 6;
        index2 = 7;
        index3 = 8;
    }
    else if(col1 == -3 || col1 == 3)
    {
        index1 = 0;
        index2 = 3;
        index3 = 6;
    }
    else if(col2 == -3 || col2 == 3)
    {
        index1 = 1;
        index2 = 4;
        index3 = 7;
    }
    else if(col3 == -3 || col3 == 3)
    {
        index1 = 2;
        index2 = 5;
        index3 = 8;
    }
    else if(diag1 == -3 || diag1 == 3)
    {
        index1 = 0;
        index2 = 4;
        index3 = 8;
    }
    else if(diag2 == -3 || diag2 == 3)
    {
        index1 = 2;
        index2 = 4;
        index3 = 6;
    }
}

