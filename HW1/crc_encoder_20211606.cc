#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <sstream>
using namespace std;
int gen_Size;
string generator;
vector <char> v;
string savebinary;

string getStringFromInt(int value, int count){
    string str ="";
    int binary = 1;
    for(int i = 0; i < count - 1; i++) binary *= 2;
    while(binary){
        if(value >= binary){
            value %= binary;
            str += "1";
         }
        else str += "0";
        binary /= 2;
    }
    return str;
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
string for_Save_Char(string bin){
    string ch;
    stringstream ss(bin);
    bitset<8> bs;
    ss >> bs;
    ch = char(bs.to_ulong());
    return ch;
}
int main(int argc, char *argv[]){
    
    int dws = atoi(argv[4]);
    if(argc != 5){
        cout << "usage: ./crc_encoder input_file output_file generator dataword_size\n";
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
    if(!(dws == 4 || dws == 8)){
        cout << "dataword size must be 4 or 8.\n";
        return 0;
    }   
    generator = argv[3];
    gen_Size = generator.size();

    char c = fin.get();
    while(!fin.fail()){
        v.push_back(c);
        c = fin.get();
    }
    fin.close();

    savebinary = "";
    for(int i = 0; i < v.size(); i++){
        int charnum = v[i];
        string one_byte = getStringFromInt(charnum, 8);
        savebinary += one_byte;
    }

    string gen_zero = "";
    for(int i = 0; i < gen_Size - 1; i++){
        gen_zero += "0";
    }

    vector<string> dwv;
    for(int i = 0; i < savebinary.size(); i+=dws){
        string s_str = savebinary.substr(i, dws);
        dwv.push_back(s_str);
    }   
    
    string cwd = "";
    for(int i = 0; i < dwv.size(); i++){
        string enc = dwv[i] + gen_zero;
        string rem = modulo2division(enc, generator);
        cwd += enc.substr(0, dws) + rem;
    }

    int cwdSize = cwd.size();
    int padnum = 8 - (cwdSize % 8);
    int padtmp = padnum;
    string codeword = "";
    while(padtmp){
        codeword += "0";
        padtmp--;
    }
    codeword += cwd;
    string padout = getStringFromInt(padnum, 8);
    fout << for_Save_Char(padout);
    vector<string> v_cwd;
    for(int i = 0; i < codeword.size(); i+=8){
        string ctmp = codeword.substr(i, 8);
        v_cwd.push_back(ctmp);
    }

    for(int i = 0; i < v_cwd.size(); i++){
        fout << for_Save_Char(v_cwd[i]);
    }   
    fout.close();
    return 0;
}