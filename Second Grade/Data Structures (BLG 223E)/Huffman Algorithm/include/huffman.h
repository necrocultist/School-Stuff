//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//---Name & Surname: Kaan Karataş
//---Student Number: 150200081

#ifndef _huffman
#define _huffman

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "structs.h"

using namespace std;

class Huffman{
    private:
        string key;
        string sortedKey;

        PriorityQueue queue;

        Tree huffmanTree;

        string encodedBinaryPassword;
        string encodedValPassword;

        string decodedPassword;
        
    public:
        void readKey(const char*);
        void sortKey();

        void findFrequency();
        void createHuffmanTree();

        void printHuffmanTree();

        string getTokenBinary(char, Node*, string);
        void encodePassword(string);
        void printEncodedPassword();

        void decodePassword(string, string);
        void decodeToken(string);
        void printDecodedPassword();
};

#endif