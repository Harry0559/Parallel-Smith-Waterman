#include<string>
#include<cmath>
#include<fstream>
#include"SW.h"

using namespace std;

void Master_Node(int num_chunks)
{
    string database = "data/database.fasta";
    int num_sequences = 10000;
    int sequences_in_chunk = floor((float)num_sequences / num_chunks);
    string temp;
    ofstream ofile;
    ifstream ifile(database.c_str());
    getline(ifile, temp);
    for (int i = 1; i <= num_chunks; i++)
    {
        string path = "chunks/chunk" + to_string(i) + ".fasta";
        ofile.open(path.c_str());
        ofile << endl;
        for (int j = 0; j < sequences_in_chunk; j++)
        {
            getline(ifile, temp);
            ofile << endl;
            getline(ifile, temp);
            ofile << temp << endl;
            getline(ifile, temp);
            ofile << temp << endl;
        }
        if (i == num_chunks)
        {
            while(!ifile.eof())
            {
                getline(ifile, temp);
                ofile << endl;
                getline(ifile, temp);
                ofile << temp << endl;
                getline(ifile, temp);
                ofile << temp << endl;                
            }
        }
        ofile.close();
    }
    ifile.close();
}