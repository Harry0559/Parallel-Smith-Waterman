#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<omp.h>
#include"SW.h"

using namespace std;

void Working_Cluster_Node(int node_num, result* ans)
{
    string queryPath = "data/query_sequence.fasta";
    string chunkPath = "chunks/chunk" + to_string(node_num) + ".fasta";
    string querySeq;
    string temp;
    char space;
    vector<string> dataSeqs;
    ifstream ifile;
    ifile.open(queryPath.c_str());
    getline(ifile, querySeq);
    getline(ifile, querySeq);
    ifile.close();
    ifile.open(chunkPath.c_str());
    getline(ifile, temp);
    while (!ifile.eof())
    {
        getline(ifile, temp);
        getline(ifile, temp);
        ifile >> space;
        getline(ifile, temp);
        dataSeqs.push_back(temp);
    }
    ifile.close();
    vector<vector<int>> record(8, vector<int>(2, 0)); //(maxScore,index)
    omp_set_num_threads(8);
    #pragma omp parallel for
    for (int i = 0; i < dataSeqs.size(); i++)
    {
        int id = omp_get_thread_num();
        int score = SWAlgo(dataSeqs[i], querySeq);
        if (score > record[id][0])
        {
            record[id][0] = score;
            record[id][1] = i;
        }
    }
    ans->maxScore = record[0][0];
    ans->index = record[0][1];
    for (int i = 1; i < 8; i++)
    {
        if (record[i][0] > ans->maxScore)
        {
            ans->maxScore = record[i][0];
            ans->index = record[i][1];
        }
    }
}