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
        // execute and update
        //判斷alu內是否有正在做的instruction，如果有且未完成，則繼續做(cd--)，如果完成，則wb
        if (alu[0].cd != emp && alu[0].cd > 0) //+-
        {
            alu[0].cd--;
        }
        else if (alu[0].cd == 0) //做完ㄌ且不為空
        {
            //更新rf
            int wb = 0;
            // cout << "alu[0] done and wb" << endl;
            if (rs[alu[0].rsn].Operator == "ADDI")
            {
                // rf[stoi(rs[alu[0].rsn].rd.substr(1))]
                wb = rs[alu[0].rsn].o1v + rs[alu[0].rsn].o2v;
            }
            else if (rs[alu[0].rsn].Operator == "ADD")
            {
                // rf[stoi(rs[alu[0].rsn].rd.substr(1))]
                wb = rs[alu[0].rsn].o1v + rs[alu[0].rsn].o2v;
            }
            else if (rs[alu[0].rsn].Operator == "SUB")
            {
                // rf[stoi(rs[alu[0].rsn].rd.substr(1))]
                wb = rs[alu[0].rsn].o1v - rs[alu[0].rsn].o2v;
            }

            //處理rat
            //(如果這個rs)跟(這個rs的rd對應到的rf對應到的rat)相同->清掉rat
            //如果不是->不用動
            //
            if (alu[0].rsn == rat[stoi(rs[alu[0].rsn].rd.substr(1))])
            {
                rf[stoi(rs[alu[0].rsn].rd.substr(1))] = wb;
                rat[stoi(rs[alu[0].rsn].rd.substr(1))] = emp;
            }

            for (int i = 0; i < 5; i++)
            {
                if (rs[i].o1 == rs[alu[0].rsn].rd)
                {
                    rs[i].o1v = wb;
                }
                if (rs[i].o2 == rs[alu[0].rsn].rd)
                {
                    rs[i].o2v = wb;
                }
            }
            //清掉rs
            rs[alu[0].rsn].o1 = "";
            rs[alu[0].rsn].o2 = "";
            rs[alu[0].rsn].rd = "";
            rs[alu[0].rsn].Operator = "";
            rs[alu[0].rsn].o1v = emp;
            rs[alu[0].rsn].o2v = emp;
            //清掉alu
            alu[0].cd = emp;
            alu[0].rsn = emp;

            rsemp--;
            modified = true;
        }
        //
        if (alu[1].cd != emp && alu[1].cd > 0) //*/
        {
            alu[1].cd--;
        }
        else if (alu[1].cd == 0) //做完ㄌ且不為空
        {
            int wb = 0;
            // cout << "alu[1] done and wb" << endl;
            if (rs[alu[1].rsn].Operator == "MUL")
            {
                // rf[stoi(rs[alu[1].rsn].rd.substr(1))]
                wb = rs[alu[1].rsn].o1v * rs[alu[1].rsn].o2v;
            }
            else if (rs[alu[1].rsn].Operator == "DIV")
            {
                // rf[stoi(rs[alu[1].rsn].rd.substr(1))]
                wb = rs[alu[1].rsn].o1v / rs[alu[1].rsn].o2v;
            }

            //處理rat
            //(如果這個rs)跟(這個rs的rd對應到的rf對應到的rat)相同->清掉rat
            //如果不是->不用動
            //
            if (alu[1].rsn == rat[stoi(rs[alu[1].rsn].rd.substr(1))])
            {
                rf[stoi(rs[alu[1].rsn].rd.substr(1))] = wb;
                rat[stoi(rs[alu[1].rsn].rd.substr(1))] = emp;
            }

            for (int i = 0; i < 5; i++)
            {
                if (rs[i].o1 == rs[alu[1].rsn].rd)
                {
                    rs[i].o1v = wb;
                }
                if (rs[i].o2 == rs[alu[1].rsn].rd)
                {
                    rs[i].o2v = wb;
                }
            }
            //清掉rs
            rs[alu[1].rsn].o1 = "";
            rs[alu[1].rsn].o2 = "";
            rs[alu[1].rsn].rd = "";
            rs[alu[1].rsn].Operator = "";
            rs[alu[1].rsn].o1v = emp;
            rs[alu[1].rsn].o2v = emp;
            //清掉alu
            alu[1].cd = emp;
            alu[1].rsn = emp;

            rsemp--;
            modified = true;
        }
        // execute done

        // capture
        for (int i = 0; i < 3; i++)
        {
            if (rs[i].Operator == "")
            {
                continue;
            }
            if (rs[i].Operator == "ADDI")
            {
                if (rat[stoi(rs[i].o1.substr(1))] == emp)
                    rs[i].o1v = rf[stoi(rs[i].o1.substr(1))];
            }
            else if (rs[i].Operator == "ADD" || rs[i].Operator == "SUB")
            {
                if (rat[stoi(rs[i].o1.substr(1))] == emp)
                    rs[i].o1v = rf[stoi(rs[i].o1.substr(1))];
                if (rat[stoi(rs[i].o2.substr(1))] == emp)
                    rs[i].o2v = rf[stoi(rs[i].o2.substr(1))];
            }
        }
        for (int i = 3; i < 5; i++)
        {
            if (rs[i].Operator == "")
            {
                continue;
            }

            if (rs[i].Operator == "MUL" || rs[i].Operator == "DIV")
            {
                if (rat[stoi(rs[i].o1.substr(1))] == emp)
                    rs[i].o1v = rf[stoi(rs[i].o1.substr(1))];
                if (rat[stoi(rs[i].o2.substr(1))] == emp)
                    rs[i].o2v = rf[stoi(rs[i].o2.substr(1))];
            }
        }
        // capture done

        // dispatch pre process
        for (int i = 0; i < 5; i++)
        {
            if (rs[i].o1 == "F0")
                rs[i].o1v = 0;
            if (rs[i].o2 == "F0")
                rs[i].o2v = 0;
        }
        // dispatch  pre process done

        // dispatch
        if (alu[0].rsn == emp) // if alu for +- is free
        {
            for (int i = 0; i < 3; i++)
            {
                // cout << "i =" << i;
                if (rs[i].o1v != emp && rs[i].o2v != emp) //+-, if ready(2 value exist), mark down that this rs enter it
                {
                    alu[0].rsn = i;
                    // cout << "alu[0] is free and rs[" << i << "] is ready" << endl;
                    alu[0].cd = 2; // both + and - do 2 cycle
                    break;
                }
            }
        }

        if (alu[1].rsn == emp) // if alu for */ is free
        {
            for (int i = 3; i < 5; i++)
            {
                if (rs[i].o1v != emp && rs[i].o2v != emp) //*, if ready(2 value exist), mark down that this rs enter it
                {
                    alu[1].rsn = i;
                    if (rs[i].Operator == "MUL")
                        alu[1].cd = 4; // mul do 4 cycle
                    else if (rs[i].Operator == "DIV")
                        alu[1].cd = 8; // div do 8 cycle
                    break;
                }
            }
        }
        // dispatch done

        // issue
        // if there exist space, put in an inst. and update rat of rd(put in rs number)
        if (!IQ.empty())
        {
            vector<string> tmp = IQ.front();
            if (tmp[0] == "ADDI" || tmp[0] == "ADD" || tmp[0] == "SUB")
            {
                if (tmp[0] == "ADDI")
                {
                    for (int i = 0; i < 3; i++)
                    {
                        if (rs[i].Operator == "") //這格rs(0~2其中一格)是空的
                        {
                            rs[i].rd = tmp[1]; //目的地
                            // cout << "----rs" << i << "'s rd= " << rs[i].rd << endl;
                            rs[i].o1 = tmp[2]; // rs1
                            rs[i].o2 = "";     // rs2(not reg)
                            rs[i].o1v = emp;
                            if (rat[stoi(rs[i].o1.substr(1))] == emp)
                                rs[i].o1v = rf[stoi(rs[i].o1.substr(1))];
                            rs[i].o2v = stoi(tmp[3]);          // rs2 val(only val)
                            rs[i].Operator = tmp[0];           // operator
                            rat[stoi(rs[i].rd.substr(1))] = i; //把rs的數字存進rat
                            rsemp++;
                            IQ.pop();
                            modified = true;
                            break;
                        }
                    }
                }
                else // add sub
                {
                    for (int i = 0; i < 3; i++)
                    {
                        if (rs[i].Operator == "") //這格rs(0~2其中一格)是空的
                        {
                            rs[i].rd = tmp[1]; //目的地
                            // cout << "----rs" << i << "'s rd= " << rs[i].rd << endl;
                            rs[i].o1 = tmp[2]; // rs1
                            rs[i].o2 = tmp[3]; // rs2
                            rs[i].o1v = emp;
                            rs[i].o2v = emp;
                            if (rat[stoi(rs[i].o1.substr(1))] == emp)
                                rs[i].o1v = rf[stoi(rs[i].o1.substr(1))];
                            if (rat[stoi(rs[i].o2.substr(1))] == emp)
                                rs[i].o2v = rf[stoi(rs[i].o2.substr(1))];
                            rs[i].Operator = tmp[0];           // operator
                            rat[stoi(rs[i].rd.substr(1))] = i; //把rs的數字存進rat
                            rsemp++;
                            IQ.pop();
                            modified = true;
                            break;
                        }
                    }
                }
            }
            else if (tmp[0] == "MUL" || tmp[0] == "DIV")
            {
                for (int i = 3; i < 5; i++)
                {
                    if (rs[i].Operator == "") //這格rs(0~2其中一格)是空的
                    {
                        rs[i].rd = tmp[1]; //目的地
                        // cout << "----rs" << i << "'s rd= " << rs[i].rd << endl;
                        rs[i].o1 = tmp[2]; // rs1
                        rs[i].o2 = tmp[3]; // rs2
                        rs[i].o1v = emp;
                        rs[i].o2v = emp;
                        if (rat[stoi(rs[i].o1.substr(1))] == emp)
                            rs[i].o1v = rf[stoi(rs[i].o1.substr(1))];
                        if (rat[stoi(rs[i].o2.substr(1))] == emp)
                            rs[i].o2v = rf[stoi(rs[i].o2.substr(1))];
                        rs[i].Operator = tmp[0];           // operator
                        rat[stoi(rs[i].rd.substr(1))] = i; //把rs的數字存進rat
                        rsemp++;
                        IQ.pop();
                        modified = true;
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < 5; i++)
        {
            // cout << "rs" << i << "=" << rs[i].o1 << endl;
            //  cout << "rs" << i << "=" << rs[i].o2 << endl;
            if (rs[i].o1 == "F0")
            {
                rs[i].o1v = 0;
                // cout << "rs[1].o1v = " << rs[i].o1v << endl;
            }
            if (rs[i].o2 == "F0")
            {
                rs[i].o2v = 0;
                // cout << "rs[1].o2v = " << rs[i].o2v << endl;
            }
        }
        // issue done
        if (modified) // print
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
                if (rs[i].Operator != "")
                    cout << " " << po(rs[i].Operator) << " ";
                cout << rs[i].o1 << " " << (rs[i].o2 == "" ? (rs[i].o2v == emp ? "null" : to_string(rs[i].o2v)) : rs[i].o2);
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
