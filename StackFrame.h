#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>
using namespace std;
/*
StackFrame declaration
*/
class StackFrame {
public:
    class Node;
private:
    int opStackMaxSize; // max size of operand stack
    int localVarArrSize; // size of local variable array
    int line;
    //Stack Operand
private:
    float StackArr[32];
    int countStack = 0;

    //Local Variable
private:
    Node* root;
    int countLocal = 0;

public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(string filename);

    //Run the method Arithmetic Instructions   
    void iadd();
    void fadd();
    void isub();
    void fsub();
    void imul();
    void fmul();
    void idiv();
    void fdiv();
    void irem();
    void ineg();
    void fneg();
    void iand();
    void ior();
    void ieq();
    void feq();
    void ineq();
    void fneq();
    void ilt();
    void flt();
    void igt();
    void fgt();
    void ibnot();

    //Run the method Load and Store Instructions
    void iconst(string val);
    void fconst(string val);
    void iload(string var);
    void fload(string var);
    void istore(string var);
    void fstore(string var);

    //Run the method Type conversion Instructions
    void i2f();
    void f2i();

    //Run the method Operand Stack Management Instructions
    void top();
    void val(string var);
    void par(string var);

    // AVL Tree Local Variable
    int getHeightRec(Node* root) {
        if (!root) return 0;
        else if (getHeightRec(root->LeftChild) > getHeightRec(root->RightChild)) return getHeightRec(root->LeftChild) + 1;
        else return getHeightRec(root->RightChild) + 1;
    }
    int   balanceFactor(Node* root) {
        if (!root) return 0;
        return getHeightRec(root->LeftChild) - getHeightRec(root->RightChild);
    }
    Node* rotateLeft(Node* root) {
        if (root->RightChild) {
            Node* tempR = root->RightChild;
            root->RightChild = tempR->LeftChild;
            tempR->LeftChild = root;
            return tempR;
        }
        return root;
    }
    Node* rotateRight(Node* root) {
        if (root->LeftChild) {
            Node* tempL = root->LeftChild;
            root->LeftChild = tempL->RightChild;
            tempL->RightChild = root;
            return tempL;
        }
        return root;
    }
    Node* searchNodeRec(Node* root, string var) {
        if (!root || var.compare(root->key) == 0) return root;
        if (var.compare(root->key) > 0) return searchNodeRec(root->RightChild, var);
        else return searchNodeRec(root->LeftChild, var);
    }
    Node* searchNode(string var) {
        return searchNodeRec(root, var);
    }
    Node* insertAVLNodeRec(Node* root, string var, float datatype, float data) {
        Node* newnode = new Node(var, datatype, data);
        if (!root) return newnode;
        else {
            if (var.compare(root->key) > 0) root->RightChild = insertAVLNodeRec(root->RightChild, var, datatype, data);
            else if (var.compare(root->key) < 0) root->LeftChild = insertAVLNodeRec(root->LeftChild, var, datatype, data);
            else {
                root = newnode;
                countLocal--;
            }
        }

        int balance = balanceFactor(root);
        //left left 
        if (balance > 1 && var.compare(root->LeftChild->key) < 0) return rotateRight(root);
        //left right 
        if (balance > 1 && var.compare(root->LeftChild->key) > 0) {
            root->LeftChild = rotateLeft(root->LeftChild);
            return rotateRight(root);
        }
        //right right
        if (balance < -1 && var.compare(root->RightChild->key) > 0) return rotateLeft(root);
        //right left 
        if (balance < -1 && var.compare(root->RightChild->key) < 0) {
            root->RightChild = rotateRight(root->RightChild);
            return rotateLeft(root);
        }
        return root;
    }
    void  insertAVLNode(const string& var, float datatype, float data) {
        root = insertAVLNodeRec(root, var, datatype, data);
    }
    Node* findparentRec(Node* root, string var) {
        Node* parent = NULL;
        Node* proot = root;
        while (proot && proot->key != var) {
            parent = proot;
            if (var.compare(proot->key) > 0) proot = proot->RightChild;
            else proot = proot->LeftChild;
        }
        return parent;
    }
    Node* findparent(string var) {
        return findparentRec(root, var);
    }

public:
    class Node {
    private:
        string key;
        float datatype;
        float data;
        Node* LeftChild;
        Node* RightChild;
        friend class StackFrame;
    public:
        Node(string key, float datatype, float data) {
            this->key = key;
            this->datatype = datatype;
            this->data = data;
            LeftChild = NULL;
            RightChild = NULL;
        }
    };

};

#endif // !__STACK_FRAME_H__