#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <sstream>
using namespace std;
vector <char> v;
string dataword_size;
int cwcnt, ercnt;
string savebinary;
int getIntFromString(string str){
    int value = 0;
    int binary = 1;
    for(int i = str.size() - 1; i >= 0; i--){
        
        if(str[i] == '1') value += binary; 
        binary *= 2;
    }
    return value;
}
string for_Save_Char(string bin){
    string ch;
    stringstream ss(bin);
    bitset<8> bs;
    ss >> bs;
    ch = char(bs.to_ulong());
    return ch;
}

string XOR(string a, string b) {
    string res = "";
    for (int i = 1; i < b.size(); i++) {
        if (a[i] == b[i]) res += "0";
        else res += "1";
    }
    return res;
}
string modulo2division(string dividend, string generator) {
    int next = generator.size();
    string rem = dividend.substr(0, next);
    int divSize = dividend.size();
    while (next < divSize) {
        if (rem[0] == '1') {
            rem = XOR(generator, rem) + dividend[next];
        }
        else {
            rem = rem.substr(1, generator.size() - 1) + dividend[next];
        }
        next++;
    }
    if (rem[0] == '1') {
        rem = XOR(generator, rem);
    }
    else {
        rem = rem.substr(1, generator.size() - 1);
    }
    return rem;
}

int main(int argc, char *argv[]){
    
    int dws = atoi(argv[5]);
    if(argc != 6){
        cout << "usage: ./crc_encoder input_file output_file result_file generator dataword_size\n";
        return 0;
    }
    ifstream fin;
    fin.open(argv[1], ios::binary);
    if(fin.is_open() == false){
        cout << "input file open error.\n";
        return 0;
    }
    ofstream fout;
    fout.open(argv[2], ios::binary);
    if(fout.is_open() == false){
        cout << "output file open error.\n";
        return 0;
    }
    ofstream fres;
    fres.open(argv[3]);
    if(fres.is_open() == false){
        cout << "result file open error.\n";
        return 0;
    }
    if(!(dws == 4 || dws == 8)){
        cout << "dataword size must be 4 or 8.\n";
        return 0;
    }   
    string generator = argv[4];
    int gen_Size = generator.size();

    char c = fin.get();
    while(!fin.fail()){
        v.push_back(c);
        c = fin.get();
    }
    fin.close();

    savebinary = "";
    for(int i = 0; i < v.size(); i++){
        int charnum = v[i];
        string one_byte = bitset<8>(charnum).to_string();
        savebinary += one_byte;
    }
    int paddnum = getIntFromString(savebinary.substr(0, 8));
    savebinary = savebinary.substr(8 + paddnum);;

    string decoded_Data = "";
    for(int i = 0; i  < savebinary.size(); i += (dws + gen_Size - 1)){
        string rem = modulo2division(savebinary.substr(i, dws + gen_Size -1), generator);
        if(getIntFromString(rem)) ercnt++;
        cwcnt++;
        decoded_Data += savebinary.substr(i, dws);
    }
    fres << cwcnt << ' ' << ercnt << endl;
    fres.close();

    for(int i = 0; i < decoded_Data.size(); i+=8){
        fout << for_Save_Char(decoded_Data.substr(i, 8));
    }
    fout.close();
    
    return 0;
}