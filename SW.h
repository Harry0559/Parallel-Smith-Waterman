#pragma once

using namespace std;

typedef struct {
    int maxScore;
    int index;
}result;

void Master_Node(int num_chunks);
int SWAlgo(string data, string query);
void Working_Cluster_Node(int node_num, result* ans);