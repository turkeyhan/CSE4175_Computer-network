#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <utility>
#include <queue>
#define MAX_NUM 100
#define INF 1e9
using namespace std;
int nodesize;
ofstream fout;
int dis[MAX_NUM + 1][MAX_NUM + 1];
int pre[MAX_NUM + 1][MAX_NUM + 1];
vector <vector<pair<int, int> > > graph(MAX_NUM + 1);
typedef struct items{
    int from;
    int to;
    string mes;
}item;
struct cmp{
    bool operator()(pair <int, int> A, pair<int, int> B){
        if(A.first == B.first){
            return A.second > B.second;
        }
        else return A.first > B.first;
    }
};
void Dijkstra(int start){
    dis[start][start] = 0;
    pre[start][start] = start;
    
    priority_queue<pair<int, int>, vector<pair<int, int> >, cmp> pq;
    pair<int, int> tmp;
    tmp.first = 0;
    tmp.second = start;
    pq.push(tmp);
    while(!pq.empty()){
        int wei = pq.top().first;
        int cur = pq.top().second;
        pq.pop();
        if(wei > dis[start][cur]) continue;
        int gcs = graph[cur].size();
        for(int i = 0; i < gcs; i++){
            int nex = graph[cur][i].first;
            int nw = graph[cur][i].second;
            if(dis[start][nex] == nw + wei){
                if(pre[start][nex] > cur) pre[start][nex] = cur;
            }
            else if(dis[start][nex] > nw + wei){
                dis[start][nex] = nw + wei;
                pre[start][nex] = cur;
                tmp.first = dis[start][nex];
                tmp.second = nex;
                pq.push(tmp);
            }
        }
    }
       
    return;
}
void init_dis(){
    for(int i = 0; i <= MAX_NUM; i++){
        for(int j= 0 ; j <= MAX_NUM; j++){
            dis[i][j] = INF;
        }
    }
    return;
}
void init_pre(){
    for(int i = 0; i <= MAX_NUM; i++){
        for(int j= 0 ; j <= MAX_NUM; j++){
            pre[i][j] = INF;
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
        cout << "usage: linkstate topologyfile messagesfile changesfile\n";
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

    init_dis();
    init_pre();
    while(fin1 >> n1 >> n2 >> w){
        pair<int, int> tmp;
        tmp.first = n2;
        tmp.second = w;
        graph[n1].push_back(tmp);
        tmp.first = n1;
        graph[n2].push_back(tmp);
    }
    for(int i = 0; i < nodesize; i++){
        Dijkstra(i);
    }
    fin1.close();

    //print initial state
    fout.open("output_ls.txt");
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
        vector <pair<int, int> >::iterator iter;
        if(w == -999){
            for(iter = graph[n1].begin(); iter != graph[n1].end(); iter++){
                if((*iter).first == n2) {
                    graph[n1].erase(iter);
                    break;
                }
            }
            for(iter = graph[n2].begin(); iter != graph[n2].end(); iter++){
                if((*iter).first == n1) {
                    graph[n2].erase(iter);
                    break;
                }
            }
        }
        else{
            bool flag = 1;
            for(iter = graph[n1].begin(); iter != graph[n1].end(); iter++){
                if((*iter).first == n2) {
                    (*iter).second = w;
                    flag = 0;
                    break;
                }
            }
            for(iter = graph[n2].begin(); iter != graph[n2].end(); iter++){
                if((*iter).first == n1) {
                    (*iter).second = w;
                    flag = 0;
                    break;
                }
            }
            if(flag){
                pair<int, int> tmp;
                tmp.first = n2;
                tmp.second = w;
                graph[n1].push_back(tmp);
                tmp.first = n1;
                graph[n2].push_back(tmp);
            }
        }
        init_dis();
        init_pre();
        for(int i = 0; i < nodesize; i++){
            Dijkstra(i);
        }
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

    cout << "Complete. Output file written to output_ls.txt.\n";
    fout.close();
    return 0;
}