#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#define emp -1 //拿這個用來代表空
using namespace std;

int cycle = 0; // add,sub,mul,div
string pm(int n)
{
    if (n == emp)
        return "null";
    return to_string(n);
}
string po(string s)
{
    if (s == "ADD" || s == "ADDI")
        return "+";
    if (s == "SUB")
        return "-";
    if (s == "MUL")
        return "*";
    if (s == "DIV")
        return "/";
    return "null";
}
// containers for all the data
struct RS
{
    string o1;
    string o2;
    string rd;
    string Operator;
    int o1v; // operand1 value
    int o2v; // operan2 value
    bool empty()
    {
        return o1 == "" && o2 == "";
    }
};
struct ALU
{
    int cd;  // execute waiting time
    int rsn; // which rs
};

RS rs[5]; // 0~2(+-),3~4(*/)
ALU alu[2];
int rat[6] = {emp, emp, emp, emp, emp, emp}; //先把rat弄成沒放東西//之後會放rs數字
int rf[6] = {0, 0, 2, 4, 6, 8};              //先把rf initialize//但F0=0
queue<vector<string>> IQ;
//
int main()
{
    int cycle = 0;
    for (int i = 0; i < 5; i++) // initialize
    {
        rs[i].o1 = "";
        rs[i].o2 = "";
        rs[i].rd = "";
        rs[i].Operator = "";
        rs[i].o1v = emp;
        rs[i].o2v = emp;
    }
    for (int i = 0; i < 2; i++)
    {
        alu[i].cd = emp;
        alu[i].rsn = emp;
    }
    vector<string> in;
    streambuf *psbuf, *backup;
    ofstream file;
    file.open("output.txt");
    backup = cout.rdbuf();
    psbuf = file.rdbuf();
    cout.rdbuf(psbuf); // cout定向到文件
    fstream f = fstream("input.txt");
    string s;
    while (getline(f, s))
    {
        in.push_back(s);
    }
    string tmp = "";
    vector<string> temp_vec;
    for (int i = 0; i < in.size(); i++) //把instruction分割成vector of string
    {
        temp_vec.clear();
        for (int j = 0; j < in[i].size(); j++)
        {
            if ((!isalnum(in[i][j]) && in[i][j] != '-'))
                continue;
            else
            {
                tmp += in[i][j];
                if (!isalnum(in[i][j + 1]))
                {
                    temp_vec.push_back(tmp);
                    tmp = "";
                    continue;
                }
            }
        }
        IQ.push(temp_vec); //把vector of string存入IQ
    }
    cout << "start\n";
    // complete read in datas
    // start running
    int rsemp = 0;
    bool complete = false;
    while (!complete)
    {
        // not done yet
        cycle++;
        bool modified = false;
        // execute wb & capture
        if (alu[0].cd != emp && alu[0].cd > 0) //+-
        {
            alu[0].cd--;
        }
        else if (alu[0].cd == 0)
        {
            int wb = 0;
            if (rs[alu[0].rsn].Operator == "ADD" || rs[alu[0].rsn].Operator == "ADDI")
                wb = rs[alu[0].rsn].o1v + rs[alu[0].rsn].o2v;
            else if (rs[alu[0].rsn].Operator == "SUB")
                wb = rs[alu[0].rsn].o1v - rs[alu[0].rsn].o2v;

            if (rat[stoi(rs[alu[0].rsn].rd.substr(1))] == alu[0].rsn)
            {
                rf[stoi(rs[alu[0].rsn].rd.substr(1))] = wb;
                rat[stoi(rs[alu[0].rsn].rd.substr(1))] = emp;
            }
            //
            //
            for (int i = 0; i < 5; i++)
            {
                if (rs[i].o1 == "rs" + to_string(alu[0].rsn))
                {
                    rs[i].o1v = wb;
                    cout << "update rs" << i << " o1v to " << wb << endl;
                }
                if (rs[i].o2 == "rs" + to_string(alu[0].rsn))
                {
                    rs[i].o2v = wb;
                    cout << "update rs" << i << " o2v to " << wb << endl;
                }
            }
            rs[alu[0].rsn].o1 = "";
            rs[alu[0].rsn].o2 = "";
            rs[alu[0].rsn].rd = "";
            rs[alu[0].rsn].Operator = "";
            rs[alu[0].rsn].o1v = emp;
            rs[alu[0].rsn].o2v = emp;
            alu[0].cd = emp;
            alu[0].rsn = emp;
            modified = true;
            rsemp--;
        }

        if (alu[1].cd != emp && alu[1].cd > 0) //*/
        {
            alu[1].cd--;
        }
        else if (alu[1].cd == 0)
        {
            int wb = 0;
            if (rs[alu[1].rsn].Operator == "MUL")
                wb = rs[alu[1].rsn].o1v * rs[alu[1].rsn].o2v;
            else if (rs[alu[1].rsn].Operator == "DIV")
                wb = rs[alu[1].rsn].o1v / rs[alu[1].rsn].o2v;
            if (rat[stoi(rs[alu[1].rsn].rd.substr(1))] == alu[1].rsn)
            {
                rf[stoi(rs[alu[1].rsn].rd.substr(1))] = wb;
                rat[stoi(rs[alu[1].rsn].rd.substr(1))] = emp;
            }
            //
            //
            for (int i = 0; i < 5; i++)
            {
                if (rs[i].o1 == "rs" + to_string(alu[1].rsn))
                {
                    rs[i].o1v = wb;
                    cout << "update rs" << i << " o1v to " << wb << endl;
                }
                if (rs[i].o2 == "rs" + to_string(alu[1].rsn))
                {
                    rs[i].o2v = wb;
                    cout << "update rs" << i << " o2v to " << wb << endl;
                }
            }
            rs[alu[1].rsn].o1 = "";
            rs[alu[1].rsn].o2 = "";
            rs[alu[1].rsn].rd = "";
            rs[alu[1].rsn].Operator = "";
            rs[alu[1].rsn].o1v = emp;
            rs[alu[1].rsn].o2v = emp;
            alu[1].cd = emp;
            alu[1].rsn = emp;
            modified = true;
            rsemp--;
        }
        // execute wb & capture done

        // dispatch
        if (alu[0].rsn == emp) //+-的dispatch
        {
            for (int i = 0; i < 3; i++)
            {
                if (rs[i].o1v != emp && rs[i].o2v != emp)
                {
                    alu[0].rsn = i;
                    alu[0].cd = 2;
                    modified = true;
                    break;
                }
            }
        }
        if (alu[1].rsn == emp) //*/的dispatch
        {
            for (int i = 3; i < 5; i++)
            {
                if (rs[i].o1v != emp && rs[i].o2v != emp)
                {
                    alu[1].rsn = i;
                    if (rs[i].Operator == "MUL")
                        alu[1].cd = 4;
                    else if (rs[i].Operator == "DIV")
                        alu[1].cd = 8;
                    modified = true;
                    break;
                }
            }
        }
        // dispatch done

        // issue
        if (!IQ.empty())
        {
            vector<string> tmp = IQ.front();
            if (tmp[0] == "ADDI" || tmp[0] == "ADD" || tmp[0] == "SUB")
            {
                for (int i = 0; i < 3; i++)
                {
                    if (rs[i].empty()) //如果有空間
                    {
                        if (tmp[0] == "ADDI") // addi的o2另外處理(直接是value)
                        {
                            rs[i].o2 = "";            //沒
                            rs[i].o2v = stoi(tmp[3]); //直接存值
                        }
                        else // add or sub
                        {
                            if (rat[stoi(tmp[3].substr(1))] != emp) // RATX有放東西
                            {
                                //存rs
                                rs[i].o2 = "rs" + to_string(rat[stoi(tmp[3].substr(1))]);
                                //還沒有值
                                rs[i].o2v = emp;
                            }
                            else
                            {
                                //存FX
                                rs[i].o2 = tmp[3];
                                //直接拿FX的值
                                rs[i].o2v = rf[stoi(tmp[3].substr(1))];
                            }
                        }
                        //其他都一樣
                        if (rat[stoi(tmp[2].substr(1))] != emp)
                        {
                            rs[i].o1 = "rs" + to_string(rat[stoi(tmp[2].substr(1))]);
                            rs[i].o1v = emp;
                        }
                        else
                        {
                            rs[i].o1 = tmp[2];
                            rs[i].o1v = rf[stoi(tmp[2].substr(1))];
                        }
                        rs[i].rd = tmp[1];
                        rat[stoi(rs[i].rd.substr(1))] = i; // rd的F幾對應到rat的幾存入rs i
                        rs[i].Operator = tmp[0];
                        rsemp++;
                        IQ.pop();
                        modified = true;
                        break;
                    }
                }
            }
            else if (tmp[0] == "MUL" || tmp[0] == "DIV")
            {
                for (int i = 3; i < 5; i++)
                {
                    if (rs[i].empty())
                    {
                        rs[i].rd = tmp[1];
                        rs[i].Operator = tmp[0];
                        if (rat[stoi(tmp[2].substr(1))] != emp)
                        {
                            rs[i].o1 = "rs" + to_string(rat[stoi(tmp[2].substr(1))]);
                            rs[i].o1v = emp;
                        }
                        else
                        {
                            rs[i].o1 = tmp[2];
                            rs[i].o1v = rf[stoi(tmp[2].substr(1))];
                        }
                        if (rat[stoi(tmp[3].substr(1))] != emp)
                        {
                            rs[i].o2 = "rs" + to_string(rat[stoi(tmp[3].substr(1))]);
                            rs[i].o2v = emp;
                        }
                        else
                        {
                            rs[i].o2 = tmp[3];
                            rs[i].o2v = rf[stoi(tmp[3].substr(1))];
                        }
                        rat[stoi(rs[i].rd.substr(1))] = i;
                        rsemp++;
                        IQ.pop();
                        modified = true;
                        break;
                    }
                }
            }
        }
        // issue done
        if (modified) // print//modified
        {
            cout << "cycle " << cycle << endl;
            cout << endl;
            for (int i = 0; i < 6; i++)
            {
                cout << "rf" << i << ": " << fixed << setw(4) << pm(rf[i]) << " | rat" << i << ": " << fixed << setw(4) << (rat[i] == emp ? "" : "rs") + pm(rat[i]) << "\n";
            }
            cout << endl;
            for (int i = 0; i < 5; i++)
            {
                cout << "rs" << i << ": ";
                if (rs[i].empty())
                    cout << "empty";
                else
                {
                    cout << rs[i].Operator << " ";
                    if (rs[i].o1v == emp)
                        cout << rs[i].o1 << " "; //應該是rs甚麼
                    else
                        cout << rs[i].o1v << " "; //或是直接有值
                    if (rs[i].o2v == emp)
                        cout << rs[i].o2 << " ";
                    else
                        cout << rs[i].o2v << " ";
                }
                cout << endl;
            }
            cout << endl;
            for (int i = 0; i < 2; i++)
            {
                cout << "alu" << i << ": ";
                if (alu[i].rsn != emp)
                    cout << rs[alu[i].rsn].Operator << " " << rs[alu[i].rsn].o1 << " " << (rs[alu[i].rsn].o2 == "" ? to_string(rs[alu[i].rsn].o2v) : rs[alu[i].rsn].o2) << "\n";
                else
                    cout << "null\n";
                cout << "cd: " << fixed << setw(4) << pm(alu[i].cd) << ", rsn: " << pm(alu[i].rsn) << "\n";
            }
            // cout << endl;
            cout << "======================\n";
        }
        // check rs is empty or not
        if (IQ.empty() && rsemp == 0)
            complete = true;
    }
    f.close();
    cout.rdbuf(backup); //恢复cout输出重定向到终端
    file.close();
    return 0;
}
