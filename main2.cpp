#include <iostream>
#include <stdexcept>
#include <string>
#include <climits>
#include <stdio.h>
#include <string.h>

using namespace std;


struct List {
    string value;
    List* prev;
    List* next;

    List(const string val, List* p = nullptr, List* n = nullptr)
        : value(val), prev(p), next(n) {}
};

struct String {
    char znak;
    String* next;
};

long long stringToInt(const string& str, size_t index = 0, long long result = 0, long long sign = 1) {
    if (index >= str.length()) {
        return result * sign;
    }
    if (index == 0 && str[0] == '-') {
        return stringToInt(str, index + 1, result, -1);
    }
    if (str[index] >= '0' && str[index] <= '9') {
        long long digit = str[index] - '0';
        long long newResult = result * 10 + digit;
        return stringToInt(str, index + 1, newResult, sign);
    }
    return result * sign;
}

string int_to_string(long long x, string result = "", long long sign = 1) {
    if(result=="" && x==0) return "0";
    if (x == 0) {
        return sign == 1 ? result : "-" + result;
   }
    if (x < 0) {
        x *= -1;
        return int_to_string(x, result, sign = -1);
    }
    char digit = x % 10 + '0';
    x /= 10;
    result += digit;
    return int_to_string(x, result, sign);
}

string dodaj(string a, string b, string c, int reszta){
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
        return(dodaj(a, b, c, reszta));
    }
    if(a.size() > 0){
        char l1 = a[a.size()-1];
        c += l1;
        a.pop_back();
        return dodaj(a, b, c, 0);
    }
    if(b.size() > 0){
        char l1 = b[b.size()-1];
        c += l1;
        b.pop_back();
        return dodaj(a, b, c, 0);
    }
}

List* push(List* top, string value) {
    top->next = new List(value,  top, nullptr);
    return top->next;
}

List* go_to_first(List* top) {
    if (top->prev->prev == nullptr) return top;
    else return go_to_first(top->prev);
}

List* go_to_last(List* top, long long size) {
    cout << size << ": " << top->value << endl;
    if (top->next == nullptr) return top;
    else return go_to_last(top->next, size - 1);
}

void printout(List* top, long long size) {
    if(size>0){
        top = go_to_first(top);
        top = go_to_last(top, size - 1);
    }
}

List* pop(List* top) {
    List* res = top->prev;
    (top->prev)->next = nullptr;
    return res;
}

string val(List* top) {
    return top->value;
}

string reverse(string a, string b, long long pos) {
    if (pos >= 0) {
        b += a[pos];
        return reverse(a, b, pos - 1);
    }
    else return b;
}

string data(List* top, long long pos) {
    if (pos > 0) return data(top->prev, pos - 1);
    else return top->value;
}

int main() {
    char instructions[20000];
    long long pos = 0;
    while (scanf("%c", &(instructions[pos]))){
        if(instructions[pos] == '\n') break;
        pos++;
    }

    List* blank = new List("");
    List* top = blank;
    long long size = 0;

    for (long long i = 0; i < pos; i++) {
        switch (instructions[i]) {
        case '\'':
            top = push(top, "");
            size++;
            break;
        case '&':
            printout(top, size);
            break;
        case '+': {
            string A = val(top);
            A = reverse(A, "", A.size() - 1);
            top = pop(top);
            string B = val(top);
            B = reverse(B, "", B.size() - 1);
            top = pop(top);
            long long IntA = stringToInt(A);
            long long IntB = stringToInt(B);
            long long res = IntA + IntB;
            string C = int_to_string(res);
            top = push(top, reverse(C, "", C.size()-1));
            size--;
            break;
        }
        case '$':{
            string a = val(top);
            top = pop(top);
            string b = a.substr(0,1);
            a = a.substr(1);
            top = push(top, a);
            top = push(top, b);
            size++;
            break;
        }
        case ',':
            top = pop(top);
            size--;
            break;
        case ':':
            top = push(top, top->value);
            size++;
            break;
        case ';':{
            string a = val(top);
            top = pop(top);
            string b = val(top);
            top = pop(top);
            top = push(top, a);
            top = push(top, b);
            break;
        }
        case '@':{
            string a = val(top);
            top = pop(top);
            a = reverse(a, "", a.size()-1);
            long long intA = stringToInt(a);
            top = push(top, data(top, intA));
            break;
        }
        case '.':{
            char znak;
            cin >> znak;
            string a = val(top);
            top = pop(top);
            a = znak + a;
            top = push(top, a);
            break;
        }
        case '>':{
            string a = val(top);
            top = pop(top);
            cout << a[0];
            size--;
            break;
        }
        case '!':{
            string a = val(top);
            top = pop(top);
            if(a == "0" || a.empty()){
                top = push(top, "1");
            }
            else top = push(top, "0");
            break;
        }
        case '<':{
            string a = val(top);
            top = pop(top);
            string b = val(top);
            top = pop(top);
            a = reverse(a, "", a.size()-1);
            b = reverse(b, "", b.size()-1);
            long long A = stringToInt(a);
            long long B = stringToInt(b);
            if (B < A) top = push(top, "1");
            else top = push(top, "0");
            size--;
            break;
        }
        case '=':{
            string a = val(top);
            a = reverse(a, "", a.size()-1);
            top = pop(top);
            string b = val(top);
            b = reverse(b, "", b.size()-1);
            top = pop(top);
            long long A = stringToInt(a);
            long long B = stringToInt(b);
            if (B == A) top = push(top, "1");
            else top = push(top, "0");
            size--;
            break;
        }
        case '~':
            top = push(top, int_to_string(i));
            size++;
            break;
        case '?':{
            string a = val(top);
            a = reverse(a, "", a.size()-1);
            top = pop(top);
            string b = val(top);
            b = reverse(b, "", b.size()-1);
            top = pop(top);
            long long A = stringToInt(a);
            if(b!="" && b != "0"){
                i = A;
            }
            break;
        }
        case '-':{
            string a = val(top);
            top = pop(top);
            if(a.size()>0 && a[a.size()-1] == '-'){
                a.pop_back();
            }
            else{
                a = a + "-";
            }
            top = push(top, a);
            break;
        }
        case '^':{
            string a = val(top);
            top = pop(top);
            if(a.size()>0 && a[a.size()-1] == '-'){
                a.pop_back();
            }
            top = push(top, a);
            break;
        }
        case '#':{
            string a = val(top);
            top = pop(top);
            string b = val(top);
            top = pop(top);
            b = b + a;
            top = push(top, b);
            size--;
            break;
        }
        case ']':{
            string a = val(top);
            top = pop(top);
            a = reverse(a, "", a.size()-1);
            char A = stringToInt(a);
            top = push(top, string(1, A));
            break;
        }
        case '[':{
            string a = val(top);
            top = pop(top);
            char B = a[0];
            long long A = B;
            top = push(top, int_to_string(A));
            break;
        }
        default: {
            string a = val(top);
            top = pop(top);
            a = instructions[i] + a;
            top = push(top, a);
            break;
        }
        }
    }



}