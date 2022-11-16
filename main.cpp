#include<string>
#include<vector>
#include<cmath>
#include <algorithm>
#include<fstream>
#include<iostream>
#include<omp.h>
#include<time.h>
#include<string.h>
#include<iomanip>
#include"SW.h"

using namespace std;

int main(int argc, char* argv[])
{
    int mode = 0;
    if (argc == 1 || (argc == 2 && strcmp(argv[1],"--interactive") == 0)) mode = 0;
    else if (argc == 2 && strcmp(argv[1],"--file") == 0) mode = 1;
    else if (argc == 2 && strcmp(argv[1],"--help") == 0) mode = 2;
    else {
        cout << "The command is wrong. The correct usage is shown below:\n" << endl;
        mode = 2;
    }

    if (mode == 0) //interactive mode
    {
        int num_slave_nodes;
        cout << "Number of working cluster nodes: ";
        cin >> num_slave_nodes;
        clock_t start = clock();
        Master_Node(num_slave_nodes); //Master node divides the database file to chunks
        clock_t end = clock();
        cout << "\nMaster node divides the database file to chunks: " << (end-start) << "ms" << endl << endl;
        int sequences_in_chunk = floor(10000.0 / num_slave_nodes);
        result r;
        vector<result> records(num_slave_nodes,r); //Record the alignment results of all working cluster nodes, which are gathered by the master node
        for (int i = 1; i <= num_slave_nodes; i++)
        {
            start = clock();
            Working_Cluster_Node(i,&records[i-1]);
            records[i-1].index += sequences_in_chunk * (i-1); //change a relative index to an absolute index
            end = clock();
            cout << "Working cluster node " << to_string(i) << ": " << (end-start) << "ms";
            cout << setw(20) << "Maximum score: " << records[i-1].maxScore << setw(21) << "Absolute index: " << records[i-1].index << endl;
        }
        int maxS = records[0].maxScore;
        int ind = records[0].index;
        for (int i = 1; i < num_slave_nodes; i++)
        {
            if (records[i].maxScore > maxS)
            {
                maxS = records[i].maxScore;
                ind = records[i].index;
            }
        }
        cout << "\nMaster node gathers the results:" << endl;
        cout << setw(20) << "Maximum score: " << maxS << setw(21) << "Absolute index: " << ind << endl; 
        system("pause");
    }
    else if (mode == 1) //file mode
    {
        cout << "(In unit of ms):" << endl;
        cout << setw(31) << "Number of working cluster nodes" << setw(20) << "Master dividing" << setw(21) << "Slave Processing" << setw(21) << "Master gathering" << setw(19) << "Execution time" << endl;
        vector<int> num_slave_nodes = {1,2,4,6,8};
        clock_t master1, master2, slave, start, end;
        for (int i = 0; i < num_slave_nodes.size(); i++)
        {
            slave = INT_MIN;
            start = clock();
            Master_Node(num_slave_nodes[i]);
            end = clock();
            master1 = end - start;
            int sequences_in_chunk = floor(10000.0 / num_slave_nodes[i]);
            result r;
            vector<result> records(num_slave_nodes[i],r); //Record the alignment results of all working cluster nodes, which are gathered by the master node
            for (int j = 1; j <= num_slave_nodes[i]; j++)
            {
                start = clock();
                Working_Cluster_Node(j,&records[j-1]);
                records[j-1].index += sequences_in_chunk * (j-1); //change a relative index to an absolute index
                end = clock();
                if ((end - start) > slave) slave = end - start;
            }
            start = clock();
            int maxS = records[0].maxScore;
            int ind = records[0].index;
            for (int j = 1; j < num_slave_nodes[i]; j++)
            {
                if (records[j].maxScore > maxS)
                {
                    maxS = records[j].maxScore;
                    ind = records[j].index;
                }
            }
            end = clock();
            master2 = end - start;
            float executionTime = (float)(master1+slave+master2) / CLOCKS_PER_SEC; //in unit of second
            string path = "output/" + to_string(num_slave_nodes[i]) + ".txt";
            ofstream of(path.c_str());
            of << setprecision(3) << executionTime;
            of.close();
            cout << setw(31) << num_slave_nodes[i] << setw(20) << master1 << setw(21) << slave << setw(21) << master2 << setw(19) << (master1+slave+master2) << endl;
        }
    }
    else if (mode == 2) //help mode
    {
        cout << "Usage:" << endl;
        cout << setw(40) << setiosflags(ios::left) << "./parallel_SW.exe" << "Interactive mode by default" << endl;
        cout << setw(40) << setiosflags(ios::left) << "./parallel_SW.exe --interactive" << "Interactive mode by explicit command" << endl;
        cout << setw(40) << setiosflags(ios::left) << "./parallel_SW.exe --file" << "File mode" << endl;
        cout << setw(40) << setiosflags(ios::left) << "./parallel_SW.exe --help" << "Show the correct usage of the command" << endl;
    }
}