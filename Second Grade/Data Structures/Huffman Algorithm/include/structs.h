//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//---Name & Surname: Kaan Karataş
//---Student Number: 150200081

#ifndef _structs
#define _structs

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

class Token{
    public:
        string symbol;
        int val = 0;
};

class Node{
    public:
        Token token;
        Node *next;
        Node *left;
        Node *right;
};

class PriorityQueue{
    public:
        Node *head;
        PriorityQueue();
        void enque(Node*);
        Node* dequeue();
};

class Tree{
    public:
        Node *root;
        Tree();
        ~Tree();
        void deleteTree(Node*);
        Node* mergeNodes(Node*, Node*);
        void printTree(Node*, int);
};

#endif