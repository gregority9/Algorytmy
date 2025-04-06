#include <bits/stdc++.h>

using namespace std;

string dodaj(string a, string b, string c, int reszta, int sign){
    if(a.size() == 0 && b.size() == 0){
        if(reszta == 1){
            return "1" + c;
        }
        else return c;
    }

    if(a.size() > 0 && b.size() > 0){
        char l1 = a[a.size()-1] - '0';
        char l2 = b[b.size()-1] - '0';
        char res = 0;
        int sum = l1 + l2 + reszta;
        a.pop_back();
        b.pop_back();
        if(sum>=10){
            res = sum%10;
            reszta = 1;
        }
        else{
            res = sum;
            reszta = 0;
        }
        res += '0';
        c = res + c;
        return(dodaj(a, b, c, reszta, sign));
    }
    if(a.size() > 0){
        char l1 = a[a.size()-1];
        c = l1 + c;
        a.pop_back();
        return dodaj(a, b, c, 0, sign);
    }
    if(b.size() > 0){
        char l1 = b[b.size()-1];
        c = l1 + c;
        b.pop_back();
        return dodaj(a, b, c, 0, sign);
    }
}

string find_zero(string a, int pos){
    if(a[pos] == '0'){
        a[pos] = '9';
        return find_zero(a, pos-1);
    }
    else{
        a[pos] -= 1;
        return a;
    }
}

string del_zero(string a, int pos){
    if(a[pos] == '0'){
        a = a.substr(1);
        return(del_zero(a, pos));
    }
    return a;
}

string odejmij(string a, string b, string c, int reszta, int sign){
    if(b.size() == 0){
        if(a.size() == 0){
            del_zero(c, 0);
            if(sign == -1){
                c = '-' + c;
                return c;
            }
            else{
                return c;
            }
        }
        c = a[a.size()-1] + c;
        a.pop_back();
        return odejmij(a, b, c, reszta, sign);
    }
    if(a[a.size()-1] >= b[b.size()-1]){
        char res = a[a.size()-1] - b[b.size()-1] + '0';
        a.pop_back();
        b.pop_back();
        c = res + c;
        return odejmij(a, b, c, reszta, sign);
    }
    if(a[a.size()-2] > '0'){
        a[a.size()-1] += 10;
        a[a.size()-2] -= 1;
        odejmij(a, b, c, reszta, sign);
    }
    else{
        a[a.size()-1] += 10;
        a = find_zero(a, a.size()-2);
        odejmij(a, b, c, reszta, sign);
    }
}

string hard_math(string a, string b){
    //dodawanie liczb
    if(a[0] != '-' && b[0] != '-'){ //dodawanie liczb dodatnich
        return dodaj(a, b, "", 0, 1);
    }
    else if(a[0] == '-' && b[0] == '-'){ //dodawanie liczb ujemnych
        a = a.substr(1);
        b = b.substr(1);
        return dodaj(a, b, "", 0, -1);
    }
    //odejmowanie
    else if(a[0] == '-'){ //a jest na minusie
        a = a.substr(1);
        if(a.size() > b.size() || (a.size() == b.size() && a[0] > b[0])){ //jak |a| > |b| => res = (|a| - |b|) * -1;
            return odejmij(a, b, "", 0, -1);
        }
        else{ //jak |b| >= |a| => res = (|b| - |a|);
            return odejmij(b, a, "", 0, 1);
        }
    }
    else{ //b jest na minusie
        b = b.substr(1);
        if(b.size() > a.size() || (b.size() == a.size() && b[0] > a[0])){ //jak |b| > |a| => res = (|b| - |a|) * -1;
            return odejmij(b, a, "", 0, -1);
        }
        else{ //jak |a| >= |b| => res = (|a| - |b|);
            return odejmij(a, b, "", 0, 1);
        }
    }
}



int main(){
    string a = "1000000000000";
    string b = "-999999999999";
    cout << hard_math(a, b);
    
} 