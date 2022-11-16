#include<string>
#include<vector>
#include<algorithm>
#include"SW.h"

using namespace std;

int SWAlgo(string data, string query) //score: match=2,otherwise=-1
{
    int highestScore = INT_MIN;
    int dataLen = data.size();
    int queryLen = query.size();
    vector<vector<int>> matrix(2, vector<int>(queryLen+1, 0));
    for (int i = 0; i < dataLen; i++)
    {
        for (int j = 0; j < queryLen; j++)
        {
            int S = (data[i] == query[j]) ? 2 : -1;
            matrix[1][j+1] = max(max(0,matrix[1][j]-1), max(matrix[0][j+1]-1,matrix[0][j]+S));
            if (matrix[1][j+1] > highestScore) highestScore = matrix[1][j+1];
        }
        matrix[0] = matrix[1];
    }
    return highestScore;
}