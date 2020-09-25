#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

// dot -Tpng -o policy.png policydot.txt

int a[10000][20] = { 0 };
int edgeLength[10000] = {0};

string transObs(int s)
{
    // ObsNothing, Crash, Left, Right
    if (s == 0)
        return "ObsNothing";
    else if (s == 1)
        return "Crash";
    else if (s == 2)
        return "Left";
    else if(s == 3)
        return "Right";
    else
        cout << "Wrong edge" << endl;
    return 0;
}

int main()
{
    // string filename;
    // cin >> filename;
    ifstream infile;
    infile.open("policy");
    ofstream fout;
    fout.open("policydot.txt");
    string result = "";
    result += "digraph G {\n";
    result += "node [ shape = \"ellipse\", sides = 4, color = \"blue\"] \n";
    string s;
    int allNodeSize, obsPoliciesSize, numRoots, tempSecond;
    infile >> allNodeSize >> obsPoliciesSize >> numRoots >> tempSecond;
    // cout << allNodeSize << endl;
    // cout << tempSecond << endl;
    int len = 0;
    getline(infile,s);
    string action;
    while(getline(infile, s), len < allNodeSize)
    {
        // cout << s << endl;
        // Leveloff, Ascend, Descend
        if(s[2] == '0')
            action = "GoAhead";
        else if(s[2] == '1')
            action = "TurnLeft";
        else if(s[2] == '2')
            action = "TurnRight";
        //else
          //  cout << "wrong: " << s[2] << endl;

        result += "action" + to_string(len) + "[label = \"" + action + to_string(len) + "\" ] \n";
        edgeLength[len] = s[4] - '0';
    
        istringstream iss(s);
        int ss;
        for (int i = 0; i < s.length() / 2; ++i) 
        {
            iss >> ss;
            a[len][i]  = ss;
            // cout << a[len][i] << " ";
        }
        // cout << endl;
        len++;
    }

    len = 0;
    // for(int j = 0; j < 5; j++)
    //     for(int k  = 0; k < 2 * a[j][2] + 3; k++)
    //         cout << j << " " << k << " " << a[j][k] << endl;
    result += "edge [ color = \"red\", decorate = false] \n";
    while(len < allNodeSize)
    {
        // cout << a[len][2] << endl;
        for(int i = 3; i < 2 * a[len][2] + 3; i=i+2)
        {
            // cout << "len: " << len << " i: " << i << " " << a[len][i+1] << endl;
            result += "action" + to_string(len) + " -> " + "action" + to_string(a[len][i+1]) + " [style = \"solid\", color = \"red\", label = \"" +  transObs(a[len][i])+ "\"] \n";
        }
        len++;
    }
    result += "}\n";
    fout << result;
    infile.close();
    fout.close();
    return 0;
}