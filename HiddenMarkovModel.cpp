#include "Header.h"
using namespace std;

static std::unordered_map<std::string,int> keywordMap;
static std::unordered_map<std::string,int>::iterator it;

int calculateIndex(int n)
{
	int nIndex = n;
	std::string str(observationWords[nIndex], strlen(observationWords[nIndex]) + 1);
	it = keywordMap.find(str.c_str());
	if (it != keywordMap.end())
		return it->second;
	
	return 0;
}

void alphaPass(double** alpha, double* c)
{
	//alpha[0][i]
	c[0] = 0;
	for (int i = 0; i < N; i++)
	{
		int nIndex = calculateIndex(0);
		if (nIndex >= 0)
		{
			alpha[0][i] = pi[i] * B[i][nIndex];
			c[0] += alpha[0][i];
		}
	}

	//scale alpha [0][i]
	c[0] = 1 / c[0];
	for (int i = 0 ; i < N; i++)
		alpha[0][i] = c[0] * alpha[0][i];

	//alpha [t][i
	for (int t = 1; t < T; t++)
	{
		c[t] = 0;
		for (int i = 0; i < N; i++)
		{
			alpha[t][i] = 0;
			for (int j = 0; j < N; j++)
			{	
				alpha[t][i] += alpha[t-1][j] * A[j][i];
			}

			int nIndex = calculateIndex(t);
			if (nIndex >= 0)
			{
				alpha[t][i] = alpha[t][i] * B[i][nIndex];
				c[t] = c[t] + alpha[t][i];
			}
			else if (nIndex == -1)
			{
				alpha[t][i] = alpha[t][i];
				c[t] = c[t];
			}
		}

		c[t] = 1/c[t];
		for (int i = 0; i < N; i++)
			alpha[t][i] *= c[t];
	}
}

void betaPass(double **beta, double* c)
{
	for (int i = 0; i < N; i++)
	{
		beta[T-1][i] = 1 * c[T-1];
	}

	for (int t = T- 2; t >= 0; t--)
	{
		for (int i = 0; i < N; i++)
		{
			beta[t][i] = 0;
			for (int j = 0; j < N; j++)
			{
				int nIndex = calculateIndex(t);
				if (nIndex >= 0)
				{
					beta[t][i] += A[i][j] * B[j][nIndex] * beta[t+1][j];
				}
				else if (nIndex == -1)
				{
					beta[t][i] = beta[t][i];
				}
			}

			//scale beta[t][i]
			beta[t][i] *= c[t];
		}
	}
}

void gammaComputation(double ***digamma, double** gamma, double** alpha, double** beta)
{
	double denom = 0.0;
	for (int t = 0; t <= T-2; t++)
	{
		denom = 0;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				int nIndex = calculateIndex(t);
				if (nIndex >= 0)
				{
					denom += alpha[t][i] * A[i][j] * B[j][t] * beta[t+1][j];
				}
				else if (nIndex == -1)
				{
					denom = denom;
				}
			}
		}

		for (int i = 0; i < N; i++)
		{
			gamma[t][i] = 0;
			for (int j = 0; j < N; j++)
			{
				digamma[t][i][j] = (alpha[t][i] * A[i][j] * B[j][t] * beta[t+1][j]) / denom;
				gamma[t][i] = gamma[t][i] + digamma[t][i][j];
			}
		}
	}

	denom = 0;
	for (int i = 0; i < N; i++)
		denom += alpha[T-1][i];

	for (int i = 0; i < N; i++)
		gamma[T-1][i] = alpha[T-1][i] / denom ; 
}

void ReEstimateTheModel(double***digamma, double** gamma)
{
	double numer = 0.0;
	double denom = 0.0;

	int i,j, t;

	//PI
	for (i = 0; i < N; i++)
	{
		pi[i] = gamma[0][i];
	}

	//A
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			numer = 0;
			denom = 0;

			for (t = 0; t < T-1; t++)
			{
				numer += digamma[t][i][j];
				denom += gamma[t][i];
			}

			A[i][j] = numer / denom;
		}
	}

	//B
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			numer = 0;
			denom = 0;

			for (t = 0; t < T; t++)
			{
				if (t == j)
					numer += gamma[t][i];
				denom += gamma[t][i];
			}

			B[i][j] = numer / denom;
		}
	}
}

void ComputeLog(double *c)
{
	logProb = 0;
	for (int i = 0; i < T; i++)
	{
		logProb += log10l(c[i]);
	}
	logProb = -1 * logProb;
}

void PrintMatrices()
{
	int i;
        
    double ftemp;

	cout << " PI MATRIX" << endl;
    ftemp = 0.0;
    for(i = 0; i < N; ++i)
    {
        printf("%8.5f ", pi[i]);
        ftemp += pi[i];
    }
	cout << "Sum = " << ftemp << endl;
	
	int  j;
        
    cout << " A MATRIX" << endl;
	ftemp = 0;
    for(i = 0; i < N; ++i)
    {
        ftemp = 0.0;
        for(j = 0; j < N; ++j)
        {
            printf("%8.5f ", A[i][j]);
            ftemp += A[i][j];
        }
        cout << "Sum = " << ftemp << endl;
        
    }// next i
        
	cout << " B MATRIX" << endl;
    ftemp = 0;
    for(i = 0; i < N; ++i)
    {
		ftemp = 0.0;
        for (j = 0; j < M; ++j)
        {
            printf("%8.5f ", B[i][j]);
            ftemp += B[i][j];
        }
        cout << "Sum = " << ftemp << endl;
        
    }// next i
}

void initMatrices(double pi[], 
                  double A[][N],     //N 
                  double B[][M],
                  int seed)
{
    int i,
        j;
        
    double prob,
           ftemp,
           ftemp2;
    
    // initialize pseudo-random number generator
    srand(seed);

    // initialize pi
    prob = 1.0 / (double)N;
    ftemp = prob / 10.0;
    ftemp2 = 0.0;
    for(i = 0; i < N; ++i)
    {
        if((rand() & 0x1) == 0)
        {
            pi[i] = prob + (double)(rand() & 0x7) / 8.0 * ftemp;
        }
        else
        {
            pi[i] = prob - (double)(rand() & 0x7) / 8.0 * ftemp;
        }
        ftemp2 += pi[i];
        
    }// next i
    
    for(i = 0; i < N; ++i)
    {
        pi[i] /= ftemp2;
    }

    // initialize A[][]
    prob = 1.0 / (double)N;
    ftemp = prob / 10.0;
    for(i = 0; i < N; ++i)
    {
        ftemp2 = 0.0;
        for(j = 0; j < N; ++j)
        {
            if((rand() & 0x1) == 0)
            {
                A[i][j] = prob + (double)(rand() & 0x7) / 8.0 * ftemp;
            }
            else
            {
                A[i][j] = prob - (double)(rand() & 0x7) / 8.0 * ftemp;
            }
            ftemp2 += A[i][j];
            
        }// next j
        
        for(j = 0; j < N; ++j)
        {
            A[i][j] /= ftemp2;
        }
        
    }// next i
    
    // initialize B[][]
    prob = 1.0 / (double)M;
    ftemp = prob / 10.0;
    for(i = 0; i < N; ++i)
    {
        ftemp2 = 0.0;
        for(j = 0; j < M; ++j)
        {
            if((rand() & 0x1) == 0)
            {
                B[i][j] = prob + (double)(rand() & 0x7) / 8.0 * ftemp;
            }
            else
            {
                B[i][j] = prob - (double)(rand() & 0x7) / 8.0 * ftemp;
            }
            ftemp2 += B[i][j];
            
        }// next j
        
        for(j = 0; j < M; ++j)
        {
            B[i][j] /= ftemp2;
        }
        
    }// next i
    
}// end initMatrices
void InitializeKeywordMap()
{
	keywordMap["import"] = 0;
	keywordMap["abstract"] = 1;
	keywordMap["public"] = 2;
	keywordMap["static"] = 3;
	keywordMap["void"] = 4;
	keywordMap["main"] = 5;
	keywordMap["("] = 6;
	keywordMap[")"] = 7;
	keywordMap["{"] = 8;
	keywordMap["}"] = 9;
	keywordMap["return"] = 10;
	keywordMap["System.out.println"] = 11;
	keywordMap["BufferedReader"] = 12;
	keywordMap["for"] = 13;
	keywordMap["while"] = 14;
	keywordMap["package"] = 15;
	keywordMap["java.lang.Object;"] = 16;
	keywordMap["java.util.ArrayList;"] = 17;
	keywordMap["java.util.HashMap;"] = 18;
	keywordMap["int"] = 19;
	keywordMap["double"] = 20;
	keywordMap["float"] = 21;
	keywordMap["long"] = 22;
	keywordMap["char"] = 23;
	keywordMap["String"] = 24;
	keywordMap["interface"] = 25;
	keywordMap["extends"] = 26;
	keywordMap["implements"] = 27;
	keywordMap["private"] = 29;
	keywordMap["default"] = 30;
	keywordMap["protected"] = 31;
	keywordMap["try"] = 32;
	keywordMap["catch"] = 33;
	keywordMap["Exception"] = 34;
	keywordMap["finally"] = 35;
	keywordMap["switch"] = 36;
	keywordMap["break"] = 37;
	keywordMap["class"] = 38;
	keywordMap["String[]"] = 38;
}

int main()
{
	double **alpha = new double * [T];
	for (int i = 0; i < T; i++)
		alpha[i] = new double[N];

	double **beta = new double * [T];
	for (int i = 0; i < T; i++)
		beta[i] = new double[N];

	double **gamma = new double * [T];
	for (int i = 0; i < T; i++)
		gamma[i] = new double[N];

	double ***digamma = new double ** [T];
	for (int i = 0; i < T; i++)
	{
		*(digamma + i) = new double*[N];
		for (int j = 0; j < N; j++)
			*(*(digamma + i) + j) = new double[N];
	}

	double *c = new double[T];

	observation = new char [T];
	
	char temp[100];
	int nWord = 0;
	FILE *in;
	in = fopen("KMeansAlgorithm.java", "r");
	char space[1] = {""};
	int len  = 0;
	long int nTotalChar =  0;
	long int nTotalWords = 0;
	while (fgets(temp, 100, in))
	{
		char * pch;
		pch = strtok (temp ," ;\n*\t");

		while (pch != NULL)
		{
			observationWords[nWord] = new char[strlen(pch)];
			strcpy (observationWords[nWord++], pch);
			pch = strtok (NULL ," \n*\t");
		}

	}

	initMatrices(pi, A, B, 1241);
	InitializeKeywordMap();
	
	while((iters < maxIters))
    {
		cout << " \n Iteration = " << iters << endl;

		cout << "Alpha Pass " << endl;	
		alphaPass(alpha, c);
		cout << "Completed" << endl;

		cout << "Beta Pass " << endl;
		betaPass(beta, c);
		cout << "Completed " << endl;
	
		cout << "Compute Gamma and Digamma " << endl;
		gammaComputation(digamma, gamma, alpha, beta);
		cout << "Completed " << endl;

		ReEstimateTheModel(digamma, gamma);

		PrintMatrices();

		iters++;

		getchar();
	}

	delete []alpha;
	alpha = NULL;

	delete []beta;
	beta = NULL;

	delete []c;
	c = NULL;

	delete []gamma;
	gamma = NULL;

	delete []digamma;
	digamma = NULL;


}