#include <iostream>
#include <climits>
#include <stdio.h>
#include <fstream>

using namespace std;

struct String { //Listy na stosie
    char znak;
    String* next;
    String(char zn, String* n = nullptr) : znak(zn), next(n) {}
};

struct Stack { //Stos
    String* value;
    Stack* prev;
    Stack(String* val, Stack* p = nullptr) : value(val), prev(p) {}
};

Stack* push(Stack* top, Stack* New) {
    New->prev = top;
    return New;
}

void deleteString(String* s){
    while(s){
        String* tmp = s;
        s = s->next;
        delete tmp;
    }
}

void deleteStack(Stack* s){
    while(s){
        Stack* tmp = s;
        s = s->prev;
        deleteString(tmp->value);
        delete tmp;
    }
}

void go_thru_String(String* value) {
    cout << value->znak;
    if (value->next != nullptr) go_thru_String(value->next);
}

void print(String* value, long long size) {
    cout << size << ": ";
    go_thru_String(value);
}

void go_to_first(Stack* top, long long size, long long it) {
    if (top->prev != nullptr && size>0) go_to_first(top->prev, size - 1, it + 1);
    print(top->value, it);
    cout << endl;
}

void printout(Stack* top, long long size) {
    Stack* first = top;
    if (size > 0) {
        go_to_first(first, size - 1, 0);
    }
}

Stack* pop(Stack* top) {
    Stack* pom = top->prev;
    top->prev = nullptr;
    return pom;
}

int findMinus(String* A) {
    if (A->next->znak == 0) return A->znak == '-' ? 1 : 0;
	else return findMinus(A->next);
}

int findSize(String* A, int* size, int* lDigit) {
	if (A->next != nullptr) {
		(*size)++;
		A->znak == '-' ? *lDigit : (*lDigit) = A->znak - '0';
		return findSize(A->next, size, lDigit);
	}
    return *size;;
}

int findWiekszy(String* A, String* B) {
    if (A->next != nullptr && B->next != nullptr) {
		int s = findWiekszy(A->next, B->next);
        if (s == 0) {
			if (A->znak > B->znak) return 1; //A>B
			else if (A->znak < B->znak) return -1; //A<B
			else return 0; //A=B
        }
        else return s;
    }
    return 0;
}

String* add(String* A, String* B, int carry = 0) {
    if (!A && !B && !carry) return nullptr;

	int digitA = A ? A->znak - '0' : 0;
	int digitB = B ? B->znak - '0' : 0;

	int sum = digitA + digitB + carry;
	carry = sum / 10;
	char newDigit = (sum % 10) + '0';

    String* nextA = (A->next == nullptr || A->next->znak == '-') ? A->next : nullptr;
	String* nextB = (B->next == nullptr || B->next->znak == '-') ? B->next : nullptr;

    return new String(newDigit, add(nextA, nextB, sum / 10));
}

String* addMinus(String* A) { //dodawanie z minusem
	String* pom = new String('-', A);
	return pom;
}

String* subtract(String* A, String* B, int borrow = 0) {
    if (!(A->next)) return nullptr;

    int digitA = A->znak - '0' - borrow;
	int digitB = B ? B->znak - '0' : 0;

    if (digitA < digitB) {
        digitA += 10;
        borrow = 1;
	}
	else {
		borrow = 0;
	}

	int diff = digitA - digitB;
	char newDigit = diff + '0';

    String* nextA = A->next;
	String* nextB = B ? B->next : nullptr;

	String* result = new String(newDigit, subtract(nextA, nextB, borrow));

    if (!result->next && result->znak == '0') {
        delete result;
		return nullptr; // Remove leading zero
    }
    return result;
}

String* hard_math(String* A, String* B) { //dodawanie
    int minusA = findMinus(A);
    int minusB = findMinus(B);
    
	int sizeA = 0, sizeB = 0, lDigitA = 0, lDigitB = 0;
	findSize(A, &sizeA, &lDigitA);
	findSize(B, &sizeB, &lDigitB);

    minusA == 1 ? sizeA-- : sizeA;
	minusB == 1 ? sizeB-- : sizeB;
    int wiekszy = 0;
	if (sizeA < sizeB) {
		wiekszy = -1;
	}
	else if (sizeA > sizeB) {
		wiekszy = 1;
	}
	else {
        wiekszy = findWiekszy(A, B);
	}
	
    if (minusA == 0 && minusB == 0) return add(A, B);
    if (minusA == 1 && minusB == 1) return addMinus(add(A, B));
	if (minusA == 1 && minusB == 0) return wiekszy == 1 ? addMinus(subtract(A, B)) : subtract(B, A);
    if (minusA == 0 && minusB == 1) return wiekszy == 1 ? subtract(A, B) : addMinus(subtract(B, A));
}

int createInt(String* a) {
    if (a->next != nullptr && a->next->znak != NULL) {
        int pom = createInt(a->next);
        return pom * 10 + a->znak - '0';
    }
    else {
        return a->znak - '0';
    }
}

String* copyString(String* a) { //kopiowanie stringa
	if (a->next != nullptr) {
		String* pom = new String(a->znak, copyString(a->next));
		return pom;
	}
	else {
		return new String(a->znak, nullptr);
	}
}

String* findString(Stack* top, int a) {
    if (a > 0) return findString(top->prev, a - 1);
    else {
		return copyString(top->value);
    }
}

int main() {
    ifstream cin("wejscie.txt");
    cin >> noskipws;
    //wczytywanie danych
    char instructions[20000];
    int pos = 0;
    while (cin >> instructions[pos]) {
        if (instructions[pos] == '\n') break;
        pos++;
    }

    //Rozpoczęcie stosu
    String emptyString = { 0, nullptr };
    Stack* blank = new Stack(&emptyString, nullptr);
    Stack* top = blank;
    int size = 0;

    for (int i = 0; i < pos; i++) {
        switch (instructions[i]) {
        case '\'': {
            Stack* New = new Stack(&emptyString, top);
            top = push(top, New);
            size++;
            break;
        }
        case '&':
            printout(top, size);
            break;
        case '+': {
            String* A = top->value;
            top = pop(top);
            String* B = top->value;
            top->value = hard_math(A, B);
            size--;
            break;
        }
        case '$': {
            String* A = top->value;
            String* B = new String(A->znak, nullptr);
            A = A->next;
            top->value = A;
            Stack* New = new Stack(B, top);
            top = push(top, New);
            size++;
            break;
        }
        case ',': {
            top = pop(top);
            size--;
            break;
        }
        case ':': {
            Stack* New = new Stack(top->value, top);
            top = push(top, New);
            size++;
            break;
        }
        case ';': {
            String* A = top->value;
            top = pop(top);
            String* B = top->value;
            top->value = A;
            Stack* New = new Stack(B, top);
            top = push(top, New);
            break;
        }
        case '@': {
            String* a = top->value;
            top = pop(top);
            int wart = createInt(a);
            String* data = findString(top, wart);
			top = push(top, new Stack(data, top));
            break;
        }
        case '.': {
            char znak;
            cin >> znak;
			String* newString = new String(znak, top->value);
			top->value = newString;
			break;
        }
        case '>': {
            String* a = top->value;
			top = pop(top);
            cout << a->znak;
            size--;
			break;
        }
        default: {
            String* newString = new String(instructions[i], top->value);
            top->value = newString;
        }
        }
    }
}

