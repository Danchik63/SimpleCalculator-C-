#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

class History {
    vector<string> assembly;
    vector<double> result;
public:
    void Record(string assembly, double result) {
        this->assembly.push_back(assembly);
        this->result.push_back(result);
    }
    void Print() {
        for (int i = 0; i < result.size(); i++) {
            cout << assembly[i] << "=" << result[i] << endl;
        }
    }
    void ClearHistory() {
        assembly.clear();
        result.clear();
    }
    ~History() {
        ClearHistory();
    }
};

class Calculator {
protected:
    vector <double> operands;
    vector <char> signs;
    vector <double> operands_1;
    vector <char> signs_1;
    string value;
    double result{ 0 };
private:
    double calculation(double operand1, double operand2, char sign) {
        switch (sign) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/':
            if (operand2 != 0) {
                return operand1 / operand2;
            }
            else {
                cout << "Ошибка! На ноль делить нельзя.";
                exit(0);
            }
        }
    }
    void enumeration(vector <double> operands, vector <double> operands_1, vector <char> signs) {
        for (int i = 0; i < signs.size(); i++) {
            if (signs[i] == '*' || signs[i] == '/') {
                result = calculation(operands[i], operands[i + 1], signs[i]);
                operands[i] = result;
                operands.erase(operands.begin() + i + 1);
                signs.erase(signs.begin() + i);
                i--;
            }
        }
        for (int i = 0; i < signs.size(); i++) {
            result = calculation(operands[i], operands[i + 1], signs[i]);
            operands[i] = result;
            operands.erase(operands.begin() + i + 1);
            signs.erase(signs.begin() + i);
            result;
            i--;
        }
    }
    void Clear() {
        operands.clear();
        operands_1.clear();
        signs.clear();
        signs_1.clear();
        value.clear();
    }
public:
    double Operation(const string assembly) {
        for (int i = 0; i < assembly.size(); i++) {
            if (assembly[i] == '(') {
                int j = i + 1;
                while (assembly[j] != ')') {
                    if (isdigit(assembly[j]) || assembly[j] == '.' && assembly[j] != ')') {
                        value += assembly[j];
                    }
                    else {
                        operands_1.push_back(stod(value));
                        value.clear();
                        if (assembly[j] != ')') {
                            signs_1.push_back(assembly[j]);
                        }
                    }
                    j = j + 1;
                }
                i = j;
                if (!value.empty()) {
                    operands_1.push_back(stod(value));
                    value.clear();
                }
                enumeration(operands_1, operands_1, signs_1);
                operands.push_back(result);
                result = 0;
                operands_1.clear();
                signs_1.clear();
            }
            if (isdigit(assembly[i]) || assembly[i] == '.') {
                value += assembly[i];
            }
            else {
                if (value.empty() && assembly[i] != ')' && (!isdigit(assembly[i]) || assembly[i] != '.')) {
                    signs.push_back(assembly[i]);
                }
                if (!value.empty() && (!isdigit(assembly[i]) || assembly[i] != '.')) {
                    operands.push_back(stod(value));
                    value.clear();
                    if (assembly[i] != ')') {
                        signs.push_back(assembly[i]);
                    }
                }
            }
        }
        if (!value.empty()) {
            operands.push_back(stod(value));
        }
        enumeration(operands, operands, signs);
        Clear();
        return result;
    }
};
struct Pravila {
private:
    string pravila[6] = { "Правила", "1.Пример выражения: (21+2*200)*3-(6+300/20)",
        "2.Калькулятор не будет вычислять выражение, если в начале выражения или внутри скобок\n  первое число будет отрицательным (например: -2 + 2 или (-2+2) или -2+2*(-2+2)","3.Для остановки приложения, напишите - stop",
    "3.Для вывода истории напишите - history", "4.Для очистки истории напишите - clear"};
public:
    void print_pravila() {
        for (int i = 0; i < 6; i++) {
            cout << pravila[i] << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "RU");
    double result{ 0 };
    string assembly;
    Pravila pravila;
    Calculator calc;
    History history;
    cout << "Арифметический калькулятор." << endl;
    pravila.print_pravila();
    do {
        cout << "Введите выражение: ";
        cin >> assembly;
        system("cls");
        for (int i = 0; i < assembly.size(); i++) {
            if (assembly[0] == '-') {
                throw("Ошибка! Первое число имеет отрицательное значение.");
            }
            if (assembly[i] == '(' && assembly[i + 1] == '-') {
                throw("Ошибка! Первое число после скобок имеет отрицательное значение.");
            }
        }
        if (assembly == "stop") {
            exit(0);
        }
        else if (assembly == "history") {
            history.Print();
            continue;
        }
        else if (assembly == "pravila") {
            pravila.print_pravila();
            continue;
        }
        else if (assembly == "clear") {
            history.ClearHistory();
            continue;
        }
        result = calc.Operation(assembly);
        history.Record(assembly, result);
        cout << assembly << "=" << calc.Operation(assembly) << endl;
    } while (true);
}
