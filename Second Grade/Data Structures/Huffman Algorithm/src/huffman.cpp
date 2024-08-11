//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//---Name & Surname: Kaan Karataş
//---Student Number: 150200081

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <bits/stdc++.h>

#include "huffman.h"
#include "structs.h"

using namespace std;

void Huffman::readKey(const char *argv)
{
    ifstream readKeyFile;
    readKeyFile.open(argv);

    if (readKeyFile.is_open())
    {
        while (!readKeyFile.eof())
        {
            readKeyFile >> key;
        }
    }
    readKeyFile.close();
};

void Huffman::sortKey()
{
    sortedKey = key;
    sort(sortedKey.begin(), sortedKey.end());
};

void Huffman::findFrequency()
{
    sortKey();

    int traverse = 0;
    while (traverse != (signed int) sortedKey.length())
    {
        int count = 0; // count is for keeping track
        for (int i = traverse; sortedKey[i] == sortedKey[i + 1]; i++)
        {
            count++;
        }

        // save the properties to a token, and set it token of newly created node
        string key1;
        key1 = sortedKey[traverse];
        Token newtoken;
        newtoken.symbol = key1;
        newtoken.val = ++count;
        
        Node *newnode = new Node();
        newnode->token = newtoken;

       // finally, add the node to priority queue
        queue.enque(newnode);

        traverse += count;
    }
};

void Huffman::createHuffmanTree()
{
    // first create and find priorities of all nodes and send them to priority queue
    findFrequency();

    // then merge 1st and 2nd in a loop untill only 1 node is left
    while (queue.head->next != NULL)
    {
        Node *mergednode = huffmanTree.mergeNodes(queue.dequeue(), queue.dequeue());
        queue.enque(mergednode);
        mergednode = NULL;
    }

    // set last node as root
    huffmanTree.root = queue.head;
    queue.head = NULL;
};

//-----------------------------------------------------//
void Huffman::printHuffmanTree()
{
    huffmanTree.printTree(huffmanTree.root, 0);
};

string Huffman::getTokenBinary(char tokenChar, Node *traverse, string tokenBinary)
{
    // if no root, return empty
    if(!huffmanTree.root)
        return "";

    // if on a leaf, return binary
    if(traverse->token.symbol[0] == tokenChar && (!traverse->left && !traverse->right))
    {
        return tokenBinary;
    }

    // traverse left if there is left
    if(traverse->left && getTokenBinary(tokenChar, traverse->left, tokenBinary + "0") != "")
    {
        return getTokenBinary(tokenChar, traverse->left, tokenBinary + "0");
    }

    // traverse right if there is right
    else if(traverse->right && getTokenBinary(tokenChar, traverse->right, tokenBinary + "1") != "")
        return getTokenBinary(tokenChar, traverse->right, tokenBinary + "1");

    else return "";
}

void Huffman::encodePassword(string password)
{
    // add binaries and depths to encodedpasswords in a loop
    for(int i = 0; i < (signed int) password.length(); i++)
    {
        string tempBinary = getTokenBinary(password[i], huffmanTree.root, "");
        encodedBinaryPassword += tempBinary;
        encodedValPassword += std::to_string(tempBinary.length());
    }
};

void Huffman::printEncodedPassword()
{
    cout << "Encoded Password Binary: " << encodedBinaryPassword << endl;
    cout << "Encoded Password Value: " << encodedValPassword << endl;
};

void Huffman::decodePassword(string encodedBinaryPassword, string encodedValPassword)
{
    string tempBin;
    int count = 0; // for tracking process
    for(int i = 0; i < (signed int) encodedValPassword.length(); i++)
    {
        // initialize a string and set it to current binary value
        string a;
        a += encodedValPassword[i];
        for(int j = 0; j < stoi(a); j++)
        {
            tempBin += encodedBinaryPassword[count];
            count++;
        }

        // decode the currently token
        decodeToken(tempBin);
        tempBin = "";
    }
};

void Huffman::decodeToken(string encodedToken)
{
    Node* rootof = huffmanTree.root;

    // a loop to find the wanted leaf by checking wether the binaries are 0 or 1 with traversing using a node identical to root of the tree
    for(int i = 0; i < (signed int) encodedToken.length(); i++)
    {
        if(encodedToken[i] == '0')
            rootof = rootof->left;
        else if(encodedToken[i] == '1')
            rootof = rootof->right;
    }
    decodedPassword += rootof->token.symbol;
};

void Huffman::printDecodedPassword()
{
    cout << "Decoded Password: " << decodedPassword << endl;
};