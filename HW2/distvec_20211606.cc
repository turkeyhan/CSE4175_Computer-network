#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <utility>
#define MAX_NUM 100
#define INF 1e9
using namespace std;
int nodesize;
ofstream fout;
int dis[MAX_NUM + 1][MAX_NUM + 1];
int pre[MAX_NUM + 1][MAX_NUM + 1];
int weight[MAX_NUM + 1][MAX_NUM + 1];
typedef struct items{
    int from;
    int to;
    string mes;
}item;
void bellman_ford(){
    int update;
    do{
        update = 0;
        for(int i = 0; i < nodesize; i++){
            for(int j = 0; j < nodesize; j++){
                for(int k = 0; k < nodesize; k++){
                    if(dis[i][k] > (weight[j][k] + dis[i][j])){
                        dis[i][k] = weight[j][k] + dis[i][j];
                        pre[i][k] = j;
                        update++;
                    }
                    else if(dis[i][k] == (weight[j][k] + dis[i][j])){
                        if(j != k && pre[i][k] > j) {
                            pre[i][k] = j;
                        }
                    }
                }
            }
        }
    }while(update != 0);

    return;
}
void init_weight(){
    for(int i = 0; i <= MAX_NUM; i++){
        for(int j= 0 ; j <= MAX_NUM; j++){
            if(i == j) weight[i][j] = 0;
            else weight[i][j] = INF;
        }
    }
    return;
}
void init_dis(){
    for(int i = 0; i <= MAX_NUM; i++){
        for(int j= 0 ; j <= MAX_NUM; j++){
            if(i == j) dis[i][j] = 0;
            else dis[i][j] = INF;
        }
    }
    return;
}
void init_pre(){
    for(int i = 0; i <= MAX_NUM; i++){
        for(int j= 0 ; j <= MAX_NUM; j++){
            if(i == j) pre[i][j] = i;
            else pre[i][j] = INF;
        }
    }
    return;
}
void find_route(int start, int dest){
    vector<int> r;
    int cur = dest;
    int pres = pre[start][cur];
    while(pres != start){
        r.push_back(pres);
        cur = pres;
        pres = pre[start][cur];
    }
    r.push_back(pres);
    for(int i = r.size() - 1; i >= 0; i--){
        fout << r[i] << ' ';
    }
    return;
}
void print_table(){
    for(int i = 0; i < nodesize; i++){
        for(int j = 0; j < nodesize; j++){
            if(j == i){
                fout << i << ' ' << j << ' ' << 0 << '\n';
                continue;    
            }
            else if(dis[i][j] == INF){
                continue;
            }
            int cur = j;
            int pres = pre[i][cur];
            
            while(pres != i){
                cur = pres;
                pres = pre[i][cur];
            }
            fout << j << ' ' << cur << ' ' << dis[i][j] << '\n';
        }
        fout << '\n';
    }
    return;
}
int main(int argc, char *argv[]){

    
    if(argc != 4){
        cout << "usage: distvec topologyfile messagesfile changesfile\n";
        return 0;
    }
    
    //read file open
    ifstream fin1, fin2, fin3;
    fin1.open(argv[1]);
    fin2.open(argv[2]);
    fin3.open(argv[3]);
    if(fin1.is_open() == false || fin2.is_open() == false || fin3.is_open() == false){
        cout << "Error: open input file.\n";
        return 0;
    }
        
    //topology.txt
    int n1, n2, w;
    fin1 >> nodesize;
    init_weight();
    init_dis();
    init_pre();
    while(fin1 >> n1 >> n2 >> w){
        weight[n1][n2] = w;
        weight[n2][n1] = w;
    }

    bellman_ford();

    fin1.close();

    //print initial state
    fout.open("output_dv.txt");
    if(fout.is_open() == false){
        cout << "Error: open output file.\n";
        return 0;
    }

    print_table();

    //messages.txt
    int from, to;
    vector <item> s;
    string buf;
    while(fin2 >> from >> to){
        getline(fin2, buf);
        fout << "from " << from << " to " << to << " cost " << dis[from][to] << " hops ";
        find_route(from, to);
        fout << "message" << buf << '\n';
        item itm;
        itm.from = from;
        itm.to = to;
        itm.mes = buf;
        s.push_back(itm);
    }
    fout << '\n';
    fin2.close();
    
    //changes.txt
    while(fin3 >> n1 >> n2 >> w){
        if(w == -999){
            weight[n1][n2] = INF;
            weight[n2][n1] = INF;   
        }
        else{
            weight[n1][n2] = w;
            weight[n2][n1] = w;
        }
        init_dis();
        init_pre();
        
        bellman_ford();

        print_table();
        int Size = s.size();
        for(int i = 0; i < Size; i++){
            from = s[i].from;
            to = s[i].to;
            buf = s[i].mes;
            if(dis[from][to] == INF){
                fout << "from " << from << " to " << to << " cost infinite hops unreachable message" << buf << '\n';
                continue; 
            }
            fout << "from " << from << " to " << to << " cost " << dis[from][to] << " hops ";
            find_route(from, to);
            fout << "message" << buf << '\n';
        }
        fout << '\n';
    }
    fin3.close();

    cout << "Complete. Output file written to output_dv.txt.\n";
    fout.close();
    return 0;
}