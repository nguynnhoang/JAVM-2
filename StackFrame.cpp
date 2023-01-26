#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_SPACE_SIZE) {}

bool checkSpace(string method) {
    int size = method.size();
    for (int i = 0; i < size; i++) if (method[i] == ' ') return 1;
    return 0;
}

int str2int(string str) {
    //Convert string to int, Arithmetic Instructions 
    if (str == "iadd") return 1;
    else if (str == "fadd") return 2;
    else if (str == "isub") return 3;
    else if (str == "fsub") return 4;
    else if (str == "imul") return 5;

    else if (str == "fmul") return 6;
    else if (str == "idiv") return 7;
    else if (str == "fdiv") return 8;
    else if (str == "irem") return 9;
    else if (str == "ineg") return 10;

    else if (str == "fneg") return 11;
    else if (str == "iand") return 12;
    else if (str == "ior")  return 13;
    else if (str == "ieq")  return 14;
    else if (str == "feq")  return 15;

    else if (str == "ineq") return 16;
    else if (str == "fneq") return 17;
    else if (str == "ilt")  return 18;
    else if (str == "flt")  return 19;
    else if (str == "igt")  return 20;
    else if (str == "fgt")  return 21;
    else if (str == "ibnot")return 22;

    //Convert string to int, Load and Store Instructions
    else if (str == "iconst")return 23;
    else if (str == "fconst")return 24;
    else if (str == "iload") return 25;
    else if (str == "fload") return 26;
    else if (str == "istore")return 27;
    else if (str == "fstore")return 28;

    //Convert string to int, Type Conversion Instructions
    else if (str == "i2f")   return 29;
    else if (str == "f2i")   return 30;

    //Convert string to int, Operand Stack Management Instructions
    else if (str == "top")   return 31;

    //Convert string to int, Local Variable Management Instructions
    else if (str == "val")   return 32;
    else if (str == "par")   return 33;
    return 0;

}

void StackFrame::run(string filename) {
    ifstream file_name(filename);
    string method, value, testcase;

    if (file_name.is_open()) {
        while (!file_name.eof()) {
            line++;
            getline(file_name, testcase);
            if (checkSpace(testcase))
            {
                method = testcase.substr(0, testcase.find(' '));
                value = testcase.substr(testcase.find(' ') + 1, testcase.size());
                switch (str2int(method)) {
                    //Run the method Load and Store Instructions
                case 23:
                {
                    iconst(value);
                    break;
                }
                case 24:
                {
                    fconst(value);
                    break;
                }
                case 25:
                {
                    iload(value);
                    break;
                }
                case 26:
                {
                    fload(value);
                    break;
                }
                case 27:
                {
                    istore(value);
                    break;
                }
                case 28:
                {
                    fstore(value);
                    break;
                }

                //Run the method Local Variable Management Instructions
                case 32:
                {
                    val(value);
                    break;
                }

                case 33:
                {
                    par(value);
                    break;
                }
                }
            }

            else
            {
                switch (str2int(testcase)) {
                    //Run the method Arithmetic Instructions   
                case 1:
                {
                    iadd();
                    break;
                }
                case 2:
                {
                    fadd();
                    break;
                }
                case 3:
                {
                    isub();
                    break;
                }
                case 4:
                {
                    fsub();
                    break;
                }
                case 5:
                {
                    imul();
                    break;
                }
                case 6:
                {
                    fmul();
                    break;
                }
                case 7:
                {
                    idiv();
                    break;
                }
                case 8:
                {
                    fdiv();
                    break;
                }
                case 9:
                {
                    irem();
                    break;
                }
                case 10:
                {
                    ineg();
                    break;
                }
                case 11:
                {
                    fneg();
                    break;
                }
                case 12:
                {
                    iand();
                    break;
                }
                case 13:
                {
                    ior();
                    break;
                }
                case 14:
                {
                    ieq();
                    break;
                }
                case 15:
                {
                    feq();
                    break;
                }
                case 16:
                {
                    ineq();
                    break;
                }
                case 17:
                {
                    fneq();
                    break;
                }
                case 18:
                {
                    ilt();
                    break;
                }
                case 19:
                {
                    flt();
                    break;
                }
                case 20:
                {
                    igt();
                    break;
                }
                case 21:
                {
                    fgt();
                    break;
                }
                case 22:
                {
                    ibnot();
                    break;
                }

                //Run the method Type conversion Instructions
                case 29:
                {
                    i2f();
                    break;
                }
                case 30:
                {
                    f2i();
                    break;
                }

                //Run the method Operand Stack Management Instructions
                case 31:
                {
                    top();
                    break;
                }
                }
            }
        }
    }
    else throw;
}

//Load and Store Instructions
void StackFrame::iconst(string value) {
    if (countStack == opStackMaxSize) throw StackFull(line);
    StackArr[countStack] = stoi(value);
    countStack++;
    StackArr[countStack] = 0;
    countStack++;
}

void StackFrame::fconst(string value) {
    if (countStack == opStackMaxSize) throw StackFull(line);
    StackArr[countStack] = stof(value);
    countStack++;
    StackArr[countStack] = 1;
    countStack++;
}

void StackFrame::iload(string var) {
    Node* Node_found = searchNode(var);
    if (!Node_found)                   throw UndefinedVariable(line);
    if (Node_found->datatype == 1)     throw TypeMisMatch(line);
    if (countLocal >= localVarArrSize) throw LocalSpaceFull(line);
    iconst(to_string(Node_found->data));
}

void StackFrame::fload(string var) {
    Node* Node_found = searchNode(var);
    if (!Node_found)                   throw UndefinedVariable(line);
    if (Node_found->datatype == 0)     throw TypeMisMatch(line);
    if (countLocal >= localVarArrSize)             throw LocalSpaceFull(line);
    fconst(to_string(Node_found->data));
}

void StackFrame::istore(string var) {
    if (countStack == 0)                throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1)  throw TypeMisMatch(line);
    if (countLocal >= localVarArrSize)  throw LocalSpaceFull(line);
    insertAVLNode(var, 0, StackArr[countStack - 2]);
    countLocal += 1;
    countStack -= 2;
}

void StackFrame::fstore(string var) {
    if (countStack == 0)                throw StackEmpty(line);
    if (StackArr[countStack - 1] == 0)  throw TypeMisMatch(line);
    if (countLocal >= localVarArrSize)      throw LocalSpaceFull(line);
    insertAVLNode(var, 1, StackArr[countStack - 2]);
    countLocal += 1;
    countStack -= 2;
}

//Operand Stack Management Instructions
void StackFrame::top() {
    if (countStack == 0)                throw StackEmpty(line);
    cout << StackArr[countStack - 2] << endl;
}

//Local Variable Management Instructions
void StackFrame::val(string var) {
    Node* Node_found = searchNode(var);
    if (!Node_found)                     throw UndefinedVariable(line);
    cout << Node_found->data << endl;
}

void StackFrame::par(string var) {
    Node* Node_found = searchNode(var);
    if (!Node_found)                     throw UndefinedVariable(line);
    Node* Nodeparent = findparent(var);
    if (!Nodeparent) cout << "null" << endl;
    else cout << Nodeparent->key << endl;
}

//Type Conversion Instructions
void StackFrame::i2f() {
    if (countStack == 0)          throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1)       throw TypeMisMatch(line);
    float result_i2f = (float)StackArr[countStack - 2];
    countStack -= 2;
    fconst(to_string(result_i2f));
}

void StackFrame::f2i() {
    if (countStack == 0)          throw StackEmpty(line);
    if (StackArr[countStack - 1] == 0)       throw TypeMisMatch(line);
    int result_f2i = (int)StackArr[countStack - 2];
    countStack -= 2;
    iconst(to_string(result_f2i));
}

//Arithmetic Instructions
void StackFrame::iadd() {
    if (countStack <= 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1) throw TypeMisMatch(line);
    int sum = StackArr[countStack - 4] + StackArr[countStack - 2];
    countStack -= 4;
    iconst(to_string(sum));
}

void StackFrame::fadd() {
    if (countStack <= 2)                       throw StackEmpty(line);
    float sum = (float)StackArr[countStack - 4] + (float)StackArr[countStack - 2];
    countStack -= 4;
    fconst(to_string(sum));
}

void StackFrame::isub() {
    if (countStack <= 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1) throw TypeMisMatch(line);
    int sub = StackArr[countStack - 4] - StackArr[countStack - 2];
    countStack -= 4;
    iconst(to_string(sub));
}

void StackFrame::fsub() {
    if (countStack <= 2)                       throw StackEmpty(line);
    float sub = (float)StackArr[countStack - 4] - (float)StackArr[countStack - 4];
    countStack -= 4;
    fconst(to_string(sub));
}

void StackFrame::imul() {
    if (countStack <= 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1) throw TypeMisMatch(line);
    int mul = StackArr[countStack - 4] * StackArr[countStack - 2];
    countStack -= 4;
    iconst(to_string(mul));
}

void StackFrame::fmul() {
    if (countStack <= 2)                       throw StackEmpty(line);
    float sub = (float)StackArr[countStack - 4] - (float)StackArr[countStack - 2];
    countStack -= 4;
    fconst(to_string(sub));
}

void StackFrame::idiv() {
    if (countStack <= 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1) throw TypeMisMatch(line);
    int divi = StackArr[countStack - 4] / StackArr[countStack - 2];
    countStack -= 4;
    iconst(to_string(divi));
}

void StackFrame::fdiv() {
    if (countStack <= 2)                       throw StackEmpty(line);
    float divi = (float)StackArr[countStack - 4] / (float)StackArr[countStack - 2];
    countStack -= 4;
    fconst(to_string(divi));
}

void StackFrame::irem() {
    if (countStack <= 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1) throw TypeMisMatch(line);
    int rem = (int)StackArr[countStack - 4] % (int)StackArr[countStack - 2];
    countStack -= 4;
    iconst(to_string(rem));
}

void StackFrame::ineg() {
    if (countStack == 0)                      throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1)        throw TypeMisMatch(line);
    int neg = StackArr[countStack - 2] * -1;
    countStack -= 2;
    iconst(to_string(neg));
}

void StackFrame::fneg() {
    if (countStack == 0)                      throw StackEmpty(line);
    float neg = StackArr[countStack - 2] * -1.0;
    countStack -= 2;
    fconst(to_string(neg));
}

void StackFrame::iand() {
    if (countStack <= 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1)         throw TypeMisMatch(line);
    int result_and = (int)StackArr[countStack - 4] & (int)StackArr[countStack - 2];
    countStack -= 4;
    iconst(to_string(result_and));
}

void StackFrame::ior() {
    if (countStack <= 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1)         throw TypeMisMatch(line);
    int result_or = (int)StackArr[countStack - 4] & (int)StackArr[countStack - 2];
    countStack -= 4;
    iconst(to_string(result_or));
}

void StackFrame::ieq() {
    if (countStack <= 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1)         throw TypeMisMatch(line);
    if (StackArr[countStack - 4] == StackArr[countStack - 2]) {
        countStack -= 4;
        iconst("1");
    }
    else {
        countStack -= 4;
        iconst("0");
    }
}

void StackFrame::feq() {
    if (countStack < 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 4] == StackArr[countStack - 2]) {
        countStack -= 4;
        iconst("1.0");
    }
    else {
        countStack -= 4;
        iconst("0.0");
    }
}

void StackFrame::ineq() {
    if (countStack < 2)                       throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1)        throw TypeMisMatch(line);
    if (StackArr[countStack - 4] != StackArr[countStack - 2]) {
        countStack -= 4;
        iconst("1");
    }
    else {
        countStack -= 4;
        iconst("0");
    }
}

void StackFrame::fneq() {
    if (countStack < 2)                       throw StackEmpty(line);
    if ((float)StackArr[countStack - 4] != (float)StackArr[countStack - 2]) {
        countStack -= 4;
        iconst("1.0");
    }
    else {
        countStack -= 4;
        iconst("0.0");
    }
}

void StackFrame::ilt() {
    if (countStack < 2)                        throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1)         throw TypeMisMatch(line);
    if (StackArr[countStack - 4] < StackArr[countStack - 2]) {
        countStack -= 4;
        iconst("1");
    }
    else {
        countStack -= 4;
        iconst("0");
    }
}

void StackFrame::flt() {
    if (countStack < 2)                       throw StackEmpty(line);
    if ((float)StackArr[countStack - 4] < (float)StackArr[countStack - 2]) {
        countStack -= 4;
        iconst("1");
    }
    else {
        countStack -= 4;
        iconst("0");
    }
}

void StackFrame::igt() {
    if (countStack < 2)                        throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1 || StackArr[countStack - 3] == 1)         throw TypeMisMatch(line);
    if (StackArr[countStack - 4] > StackArr[countStack - 2]) {
        countStack -= 4;
        iconst("1");
    }
    else {
        countStack -= 4;
        iconst("0");
    }
}

void StackFrame::fgt() {
    if (countStack < 2)                        throw StackEmpty(line);
    if ((float)StackArr[countStack - 4] > (float)StackArr[countStack - 2]) {
        countStack -= 4;
        iconst("1");
    }
    else {
        countStack -= 4;
        iconst("0");
    }
}

void StackFrame::ibnot() {
    if (countStack < 2)                        throw StackEmpty(line);
    if (StackArr[countStack - 1] == 1)         throw TypeMisMatch(line);
    if (!StackArr[countStack - 2]) {
        countStack -= 2;
        iconst("1");
    }
    else {
        countStack -= 2;
        iconst("0");
    }
}