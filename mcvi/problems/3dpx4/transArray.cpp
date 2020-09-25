#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

int a[1000][100];

int main()
{
	ifstream infile;
    infile.open("policy");
    ofstream fout;
    fout.open("array");
    string result = "";
    string s;

    result += "{";
    int allNodeSize, obsPoliciesSize, numRoots, tempSecond;
	infile >> allNodeSize >> obsPoliciesSize >> numRoots >> tempSecond;
	int len = 0;
	getline(infile,s);
	string action;
	while(getline(infile, s), len <= tempSecond)
	{
		istringstream iss(s);
		int ss;
		for (int i = 0; i < s.length() / 2; ++i)
		{
			iss >> ss;
			a[len][i]  = ss;
		}
		len++;
	}

    for (int i = 0; i <= tempSecond; i++)
    {
        result+="{";
        for (int j = 0; j < a[i][2] * 2 + 2; j++)
        {
            result+=to_string(a[i][j])+",";
        }
        result+=to_string(a[i][a[i][2] * 2 + 2])+"},\n";
    }
       

    result += "}\n";
    fout << result;
    infile.close();
    fout.close();

    return 0;
}