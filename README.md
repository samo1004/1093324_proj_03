# 1093324_proj_3

##影片連結 :
待補
## 執行方式 :
>先在和cpp檔同個資料夾下建立空白的input.txt、output.txt，將附件中的input.txt內容複製過去，執行過後將會把輸出印到output.txt中
(注意: 檔名就是input跟output，輸入格式、大小寫等和網站上相同)
## 輸入輸出 :
>附件input.txt、output.txt
## 限制 :
1. register file使用F0~F5，rat一樣
2. 等待cycle為 ADD: 2 SUB: 2 MUL: 4 DIV: 8
3. execute完的下個cycle write back
4. 可同時write back和capture(拿來用)
5. alu一做完事就檢查有沒有rs已經ready，可以進去execute
## 變數說明 :
>各種存放data的容器，說明如註解
```cpp
// containers for all the data
struct RS
{
    string o1;//operand 1
    string o2;//operand 2
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
    int rsn; // which rs is doing now
};

RS rs[5]; // 0~2(+-),3~4(*/)
ALU alu[2];
int rat[6] = {emp, emp, emp, emp, emp, emp}; //先把rat弄成沒放東西//之後會放rs數字
int rf[6] = {0, 0, 2, 4, 6, 8};              //先把rf initialize//但F0=0
queue<vector<string>> IQ;//instruction queue
```
