#include <iostream>
#include "vector"
#include <cstdlib>
#include <ctime>
#include "thread"
#include "chrono"
#include "math.h"
using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using namespace std;


void printMatrix(int size, int** matrix){
    cout << "Matrix: " << endl;
    for(int i = 0; i < size; i++){
        cout << "[ ";
        for(int j = 0; j < size; j++){
            cout << matrix[i][j] << " ";
        }
        cout << "]" << endl;
    }
}

void calculate(int start, int actions, int** matrixA, int** matrixB, int** matrixRes, int size){
    //this_thread::sleep_for(chrono::milliseconds(1000));
    if(start + actions >= size*size){
        actions = size*size - start;
    }
    int startI, startJ;
    if(start < size){
        startI = 0;
        startJ = start;
    }
    else{
        int row = start/size;
        startI = row;
        startJ = start - row*size;
    }
    while(actions > 0){
        matrixRes[startI][startJ] = matrixA[startI][startJ] - matrixB[startI][startJ];
        if(startJ + 1 < size){
            startJ++;
        }
        else{
            startI++;
            startJ = 0;
        }
        actions--;
    }

}

int main() {
    cout << "Enter n(for matrix nxn):";
    float matrixSize;
    cin >> matrixSize;
    srand(time(0));
    int** matrixA = new int*[matrixSize];
    int** matrixB = new int*[matrixSize];
    int** matrixRes = new int*[matrixSize];
    for(int i = 0; i < matrixSize; i++)
    {
        matrixA[i] = new int[matrixSize];
        matrixB[i] = new int[matrixSize];
        matrixRes[i] = new int[matrixSize];
        for(int j = 0; j < matrixSize; j++){
            matrixA[i][j] = rand() % 15;
            matrixB[i][j] = rand() % 15;
        }
    }
    string finish = "no";
    float threadNum;
    cout << "Enter amount of threads:";
    cin >> threadNum;
    int actionsPerThread = ceil(matrixSize * matrixSize / threadNum);
    vector<thread> threads;
    auto payload_begin = high_resolution_clock::now();
    for (int i = 0; i < matrixSize * matrixSize; i += actionsPerThread) {
        threads.emplace_back(calculate, i, actionsPerThread, matrixA, matrixB, matrixRes, matrixSize);
    }
    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
    //printMatrix(matrixSize, matrixRes);
    auto payload_end = high_resolution_clock::now();
    auto elapsed = duration_cast<nanoseconds>(payload_end - payload_begin);
    std::printf("Payload Time: %.3f seconds.\n", elapsed.count() * 1e-9);
}

