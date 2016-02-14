#define N  2
#define M  38
#define T  38
#define maxIters 20

double pi[N] = {0}; 
double A[N][N] = {0}; 
double B[N][M] = {0}; 

int iters   = 0;
double logProb = 0.0;

double oldLogProb = -999;

#include <fstream>
#include <vector>
#include <iterator>
#include <iostream>
#include <string>
#include <unordered_map>

char * observation = NULL;
char * observationWords[T] = {NULL};

//typedef std::tr1::unordered_map< std::string, int > Keywords;
//Keywords  javaKeywords;
 

