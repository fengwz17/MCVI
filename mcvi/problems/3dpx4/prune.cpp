#include <iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

// dot -Tpng -o policy.png policyPrune

int a[10000][20] = { 0 };
int edgeLength[10000] = {0};

int flag[10000] = {0};

void p(int num)
{
      for(int i = 3; i < 2 * a[num][2] + 3; i=i+2)
      {
          int x = a[num][i + 1];
           //cout << x << " ";
           flag[x] = 1;
           if (x == 0)
            continue;
           p(a[num][i+1]);
      }
}

string transObs(int s)
{
    // ObsNothing, Crash, Left, Right
    if (s == 0)
        return "ObsNothing";
    else if (s == 1)
        return "Left";
    else if (s == 2)
        return "Right";
    // else if (s == 1)
    //     return "UpperLeft";
    // else if (s == 2)
    //     return "LowerLeft";
    // else if (s == 3)
    //     return "UpperRight";
    // else if (s == 4)
    //     return "LowerRight";
    else if (s == -1)
        return "TermObs";
    else
    {
        cout << s << endl;
        cout << "Wrong edge" << endl;
    }
        
    return 0;
}


int main()
{
    // string filename;
    // cin >> filename;
    ifstream infile;
    infile.open("policy");
    ofstream fout;
    fout.open("policyPrune");
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
    while(getline(infile, s), len <= tempSecond)
    {
        // // cout << s << endl;
        // // Leveloff, Ascend, Descend
        // if(s[2] == '0')
        //     action = "GoAhead";
        // else if(s[2] == '1')
        //     action = "TurnLeft";
        // else if(s[2] == '2')
        //     action = "TurnRight";
        // //else
        //   //  cout << "wrong: " << s[2] << endl;

        // result += "action" + to_string(len) + "[label = \"" + action + to_string(len) + "\" ] \n";
        // edgeLength[len] = s[4] - '0';
    
        istringstream iss(s);
        int ss;
        for (int i = 0; i < s.length() / 2; ++i) 
        {
            iss >> ss;
            a[len][i]  = ss;
            //cout << a[len][i] << " ";
        }
        //cout << endl;
        len++;
    }

    int num = tempSecond;
    flag[tempSecond] = 1;
    p(num);

    num = 0;

    while(num <= tempSecond)
    {
        // cout << s << endl;
        // Leveloff, Ascend, Descend
        if (flag[num] == 1)
        {
            if(a[num][1] == 0)
                action = "GoAhead";
            else if(a[num][1] == 1)
                action = "TurnLeft";
            else if(a[num][1] == 2)
                action = "TurnRight";
            // else if (a[num][1] == 3)
            //     action = "Climb";
            // else
            //     action = "Decline";
        //else
          //  cout << "wrong: " << s[2] << endl;

            result += "action" + to_string(num) + "[label = \"" + action + to_string(num) + "\" ] \n";
            edgeLength[len] = a[num][2];

        }
    
        num++;
    }

    len = 0;
    // // for(int j = 0; j < 5; j++)
    // //     for(int k  = 0; k < 2 * a[j][2] + 3; k++)
    // //         cout << j << " " << k << " " << a[j][k] << endl;
    result += "edge [ color = \"red\", decorate = false] \n";
    while(len <= tempSecond)
    {
        // cout << a[len][2] << endl;
        if(flag[len] == 1)
        {
            for(int i = 3; i < 2 * a[len][2] + 3; i=i+2)
        {
            // cout << "len: " << len << " i: " << i << " " << a[len][i+1] << endl;
            result += "action" + to_string(len) + " -> " + "action" + to_string(a[len][i+1]) + " [style = \"solid\", color = \"red\", label = \"" +  transObs(a[len][i])+ "\"] \n";
        }

        }
        
        len++;
    }
   


    result += "}\n";
    fout << result;
    infile.close();
    fout.close();
    return 0;
}