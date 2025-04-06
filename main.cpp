#include <iostream>
#include <stdexcept>
#include <fstream>

using namespace std;

struct List{
    string value;
    List* before;
    List* after;
};

class Stack {
    private:
        List* top;
        int capacity;
        int size;


    public:
        //konstruktor
        Stack(int initialCapacity = 10) : capacity(initialCapacity), size(0) {
            top = new List;
        }

        //destruktor
        ~Stack() {
        }

        //Kopiowanie
        Stack(const Stack &other) : capacity(other.capacity), size(other.size) {
        }

        bool empty() const {
            return size == 0;
        }

        int size() const {
            return size;
        }

        void push(const string& value){
            if(size == capacity){
                resize(capacity * 2);
            }
            top->after = new List;
            top->after->value = value;
            top->after->after = nullptr;
            top->after->before = top;
            size++;
        }

        string& pop() {
            if(empty()) {
                throw std::runtime_error("Stack underflow");
            }
            --size;
            List a = *top;
            List *b = top->before;
            top->before->after = nullptr;
            top = b;
            return a.value;
        }
        
        string& Data(int position) {
            if(empty()) {
                throw std::runtime_error("Stack is empty");
            }
            List *a = top;
            for(int i=1; i<position; i++){
                if(a->before == nullptr){
                    throw std::runtime_error("Wrong position");
                }
                a = a->before;
            }
            return a->value;   
        }

        friend std::ostream& operator<<(std::ostream& os, const Stack stack){
            List *a = top;
            for(int i=0; i<stack.size; i++){
                cout << i << ": " << a->value << endl;
                a = a->before;
            }
            return os;
        }
    private:
        void resize(int newCapacity) {
            T* newData = new T[newCapacity];
            for (int i = 0; i <= topIndex; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
};

string reverse(string a, string b, int pos){
    b += a[pos];
    if(pos+1 >= 0)
        return reverse(a, b, pos-1);
    else return b;
}

int string_to_int(string a, int x, int pos){
    char c = a[pos] - 48;
    a = a + c;
    if(pos+1 <= a.size()) return string_to_int(a, x, pos+1);
    else return x;
}

int toInt(string a){
    a = reverse(a, "", a.size()-1);
    
}



int main(){
    ifstream wejscie("wejscie.txt");
    char instructions[20000];
    int pos = 0;
    while(wejscie >> instructions[pos]) pos++;
    
    Stack stosik;

    for(int i=0; i<pos; i++){
        switch(instructions[i]){
            case '\'':
                stosik.push("");
                break;
            case '&':
                cout << stosik;
                break;
            case '+':{
                string A = stosik.pop();
                string B = stosik.pop();
                int IntA = toInt(A);
                int IntB = toInt(B);
                int res = IntA + IntB;
                stosik.push(reverse(int_to_string(res), "", int_to_string(res).size()));
                break;
            }
            case '$':{
                string a = stosik.pop();
                string b = a.substr(0,1);
                a = a.substr(1);
                stosik.push(a);
                stosik.push(b);
                break;
            }
            case ',':
                stosik.pop();
                break;
            case ':':
                stosik.push(stosik.top());
                break;
            case ';':{
                string a = stosik.pop();
                string b = stosik.pop();
                stosik.push(b);
                stosik.push(a);
                break;
            }
            case '@':{
                string a = stosik.pop();
                int intA = toInt(a);
                stosik.push(stosik.Data(intA));
                break;
            }
            case '.':{
                char znak;
                cin >> znak;
                stosik.push(znak+stosik.pop());
                break;
            }
            case '>':{
                string a = stosik.pop();
                cout << a[0] << endl;
                break;
            }
            case '!':{
                string a = stosik.pop();
                if(a == "0" || a.empty()){
                    stosik.push("1");
                }
                else stosik.push("0");
                break;
            }
            case '<':{
                string a = stosik.pop();
                string b = stosik.pop();
                int A = toInt(a);
                int B = toInt(b);
                if(B < A) stosik.push("1");
                else stosik.push("0");
                break;
            }
            case '=':{
                string a = stosik.pop();
                string b = stosik.pop();
                int A = toInt(a);
                int B = toInt(b);
                if(B == A) stosik.push("1");
                else stosik.push("0");
                break;
            }
            case '~':
                stosik.push(int_to_string(i));
                break;
            case '?':
            case '-':{
                string a = stosik.pop();
                if(a.size()>0 && a[a.size()-1] == '-'){
                    a.pop_back();
                }
                else{
                    a = a + "-";
                }
                stosik.push(a);
                break;
            }
            case '^':{
                string a = stosik.pop();
                if(a.size()>0 && a[a.size()-1] == '-'){
                    a.pop_back();
                }
                stosik.push(a);
            }
            case '#':{
                string a = stosik.pop();
                stosik.push(stosik.pop() + a);
                break;
            }
            case ']':{
                string a = stosik.pop();
                char A = toInt(a);
                stosik.push(string(1, A));
                break;
            }
            case '[':{
                string a = stosik.pop();
                int A = a[0];
                stosik.push(int_to_string(A));
                break;
            }
            default:{
                string a = stosik.pop();
                a = instructions[i] + a;
                stosik.push(a);
                break;
            }
        }
    } 
}