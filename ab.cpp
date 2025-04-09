#include <iostream>
#include <stdexcept>
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

String* intToString(int a) {
    char b = a % 10 + '0';
    String* New = new String(0, nullptr);
    if (a / 10 > 0)
        New = intToString(a / 10);
    String* nowy = new String(b, New);
    return nowy;
}

int stringToInt(String* a) {
    int b = 0;
    if (a->next && a->next->znak != 0) {
        b = stringToInt(a->next);
    }
    return b * 10 + a->znak - '0';
}

void go_thru_String(String* value) {
    if (value == nullptr) return;
    if (value->znak == 0) return;
    cout << value->znak;
    if (value->next != nullptr) go_thru_String(value->next);
}

void print(String* value, long long size) {
    cout << size << ": ";
    go_thru_String(value);
}

void go_to_first(Stack* top, long long size, long long it) {
    if (top == nullptr) return;
    if (top->prev != nullptr && size > 0) go_to_first(top->prev, size - 1, it + 1);
    print(top->value, it);
    cout << endl;
}

void printout(Stack* top, long long size) {
    if (top == nullptr || size <= 0) return;
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
    if (A == nullptr) return 0;
    if (A->next == nullptr || A->next->znak == 0) {
        return A->znak == '-';
    }
    return findMinus(A->next);
}

int findSize(String* A, int* size, char* lDigit) {
    if (A->next != nullptr && A->next->znak != 0) {
        (*size)++;
        *lDigit = A->znak;
        return findSize(A->next, size, lDigit);
    }
    return ++(*size);
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

    String* nextA = (A != nullptr && A->next != nullptr && A->next->znak != '-' && A->next->znak != 0) ? A->next : nullptr;
    String* nextB = (B != nullptr && B->next != nullptr && B->next->znak != '-' && B->next->znak != 0) ? B->next : nullptr;

    return new String(newDigit, add(nextA, nextB, sum / 10));
}



String* subtract(String* A, String* B, int borrow = 0) {
    if (A == nullptr) return nullptr;

    int digitA = A->znak - '0' - borrow;
    int digitB = (B != nullptr) ? B->znak - '0' : 0;

    if (digitA < digitB) {
        digitA += 10;
        borrow = 1;
    } else {
        borrow = 0;
    }

    String* result = new String(digitA - digitB + '0');
    
    String* nextA = (A->next != nullptr) ? A->next : nullptr;
    String* nextB = (B != nullptr && B->next != nullptr) ? B->next : nullptr;

    result->next = subtract(nextA, nextB, borrow);

    // Usuń wiodące zera (ale zostaw przynajmniej jedną cyfrę)
    if (result->next == nullptr && result->znak == '0') {
        delete result;
        return nullptr;
    }
    
    return result;
}

String* addMinus(String* a) {
    if (a == nullptr || a->znak == 0) {
        return new String('-');
    }
    a->next = addMinus(a->next);
    return a;
}


String* deleteZeros(String* A, int* wsk) {
    if (A->znak == '0') (*wsk)++;
    else if (A->znak == '-') return A;
    else (*wsk) = 0;
    if (A->next != nullptr && A->next->znak != 0) {
        deleteZeros(A->next, wsk);
    }
    if (A->next == nullptr || A->next->znak == 0 || A->next->znak == '-') {
        return A;
    }
    else {
        if ((*wsk) > 0) {
            (*wsk)--;
            A->next = A->next->next;
        }
    }
    return A;
}


String* hard_math(String* A, String* B) { //dodawanie
    int wsk = 0;
    A = deleteZeros(A, &wsk);
    wsk = 0;
    B = deleteZeros(B, &wsk);
    int minusA = findMinus(A);
    int minusB = findMinus(B);

    char lDigitA, lDigitB;
    int sizeA = 0, sizeB = 0;
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
        if(sizeA != 1)
            wiekszy = findWiekszy(A, B);
        else {
            if (A->znak > B->znak) wiekszy = 1;
            else if (B->znak > A->znak) wiekszy = -1;
            else wiekszy = 0;
        }
    }

    if (minusA == 0 && minusB == 0) return add(A, B);
    if (minusA == 1 && minusB == 1) return addMinus(add(A, B));
    if (minusA == 1 && minusB == 0) return wiekszy == 1 ? addMinus(subtract(A, B)) : subtract(B, A);
    if (minusA == 0 && minusB == 1) return wiekszy != -1 ? subtract(A, B) : addMinus(subtract(B, A));
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

String* deleteMinus(String* a) {
    if (a == nullptr) return new String(0, nullptr);
    if (a->next == nullptr || a->next->znak == 0) {
        if (a->znak == '-') {
            delete a;
            return new String(0, nullptr);
        }
        return a;
    }
    a->next = deleteMinus(a->next);
    return a;
}


String* merge(String* a, String* b) {
    if (b == nullptr) return a;
    if (b->next == nullptr || b->next->znak == 0) {
        b->next = a;
        return b;
    }
    b->next = merge(a, b->next);
    return b;
}



bool whetherEqualZero(String* A) {
	if (A == nullptr || A->znak == 0) return true;
	if (A->znak == '0' || A->znak == '-') return whetherEqualZero(A->next);
	else return false;
}

bool isEmpty(String* A) {
	if (A == nullptr || A->znak == 0) return true;
	if (A->znak == '0' && (A->next == nullptr || A->next->znak == 0)) return true;
	else return false;
}

int Compare(String* A, String* B) { // 1-A>b, -1-A<B, 0-A=B
    int wsk = 0;
    char znak;
    bool isZeroA = whetherEqualZero(A);
	bool isZeroB = whetherEqualZero(B);
	if (isZeroA) A = new String('0', new String(0, nullptr));
	if (isZeroB) B = new String('0', new String(0, nullptr));
    A = deleteZeros(A, &wsk);
    wsk = 0;
    B = deleteZeros(B, &wsk);
	int minusA = findMinus(A);
	int minusB = findMinus(B);
    int sizeA = 0, sizeB = 0;
	findSize(A, &sizeA, &znak);
    findSize(B, &sizeB, &znak);
	minusA == 1 ? sizeA-- : sizeA;
	minusB == 1 ? sizeB-- : sizeB;
    if (minusA && minusB) {
        if (sizeA > sizeB) {
            return -1;
		}
		else if (sizeA < sizeB) {
			return 1;
		}
		else {
			int pom = findWiekszy(A, B);
			if (pom == 0) return 0;
			else if (pom == 1) return -1;
			else return 1;
		}
    }
    else if (!minusA && !minusB) {
        if (sizeA > sizeB) {
            return 1;
        }
        else if (sizeA < sizeB) {
            return -1;
        }
        else {
            int pom = findWiekszy(A, B);
            if (pom == 0) return 0;
            else if (pom == 1) return 1;
            else return -1;
        }
    }
    else if (!minusA && minusB) {
        return 1;
    }
    else {
        return -1;
    }
    return 0;
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
            top->value = (top->value == nullptr ? new String('0', new String(0, nullptr)) : top->value);
            size--;
            break;
        }
        case '$': {
            String* A = top->value;
            String* B = new String(A->znak, new String(0, nullptr));
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
            String* jajo = copyString(top->value);
            Stack* New = new Stack(jajo, top);
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
        case '-': {
            String* a = top->value;
            if (findMinus(a)) {
                a = deleteMinus(a);
            }
            else {
                a = addMinus(a);
            }
            top->value = a;
            break;
        }
        case '^': {
            String* a = top->value;
            if (a->znak != 0 && findMinus(a)) {
                a = deleteMinus(a);
            }
            top->value = a;
            break;
        }
        case '[': {
            String* a = top->value;
            char B = a->znak;
            int A = B;
            a = intToString(A);
            top->value = a;
            break;
        }
        case ']': {
            String* a = top->value;
            char A = stringToInt(a);
            top->value = new String(A, new String(0, nullptr));
            break;
        }
        case '#': {
            String* a = top->value;
            top = pop(top);
            String* b = top->value;
            b = merge(a, b);
            top->value = b;
            size--;
            break;
        }
        case '<': {
            String* a = top->value;
            top = pop(top);
            String* b = top->value;
            if (Compare(a, b) == 1) {
                String* New = new String('1', new String(0, nullptr));
                top->value = New;
            }
            else {
                String* New = new String('0', new String(0, nullptr));
                top->value = New;
            }
            size--;
            break;
        }
        case '=': {
            String* a = top->value;
            top = pop(top);
            String* b = top->value;
            if (Compare(a, b) == 0) {
                String* New = new String('1', new String(0, nullptr));
                top->value = New;
            }
            else {
                String* New = new String('0', new String(0, nullptr));
                top->value = New;
            }
            size--;
            break;
        }
        case '!': {
            String* a = top->value;
            if (isEmpty(a)) {
				a = new String('1', new String(0, nullptr));
			}
			else {
				a = new String('0', new String(0, nullptr));
			}
			top->value = a;
			break;
        }
        case '~': {
            String* newString = intToString(i);
			Stack* newStack = new Stack(newString, top);
            top = newStack;
			size++;
			break;
        }
        case '?': {
            String* a = top->value;
			top = pop(top);
            String* b = top->value;
            top = pop(top);
            size -= 2;
            if (!isEmpty(b)) {
                int incik = stringToInt(a);
                i = incik - 1;
            }
            break;
        }
        default: {
            String* newString = new String(instructions[i], top->value);
            top->value = newString;
        }
        }
    }
}

