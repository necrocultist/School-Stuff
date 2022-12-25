//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//--------------------------//
//---Name & Surname: Kaan Karataş
//---Student Number: 150200081
//--------------------------//

//-------------Do Not Add New Libraries-------------//
//-------------All Libraries Needed Were Given-------------//
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "structs.h"

using namespace std;

//-------------Complete The Functions Below-------------//

//-------------Initialize Priority Queue-------------//
PriorityQueue::PriorityQueue()
{
    this->head = NULL;
};

//-------------Insert New Node To Priority Queue-------------//
void PriorityQueue::enque(Node *newnode)
{
    // if no head, make newnode head
    if (head == NULL)
    {
        head = newnode;
        return;
    }

    // condition to add before head
    if (newnode->token.val < head->token.val)
    {
        newnode->next = head;
        head = newnode;
        return;
    }

    Node *traverse = head;
    while (traverse->next != NULL)
    {
        if (newnode->token.val >= traverse->token.val && newnode->token.val < traverse->next->token.val)
        {
            break;
        }

        traverse = traverse->next;
    }

    if (traverse->next != NULL)
        newnode->next = traverse->next;

    traverse->next = newnode;
};

//-------------Remove Node From Priority Queue-------------//
Node *PriorityQueue::dequeue()
{
    Node *removed_node = head;

    // if no node, return null
    if (head == NULL)
    {
        return NULL;
    }

    // set new head
    if (head->next != NULL)
    {
        head = head->next;
    }
    // only head in the queue
    else
    {
        head = NULL;
    }

    removed_node->next = NULL;
    return removed_node;
};

//-------------Initialize Tree-------------//
Tree::Tree()
{
    this->root = NULL;
};

//-------------Delete Tree Nodes From Memory-------------//
Tree::~Tree()
{
    deleteTree(root);
};

//-------------Delete Tree From Memory-------------//
void Tree::deleteTree(Node *node)
{
    //-------------This Function Is Not Necessary-------------//
    //-----------------Use It At Your Own Will----------------//

    //traverse through the tree deleting every node
    if(!node)
        return;
    deleteTree(node->left);
    deleteTree(node->right);
        delete node;
}

//-------------Merges Two Node Into One-------------//
Node *Tree::mergeNodes(Node *temp1, Node *temp2)
{
    // create a new node and set its properties the sum of left and right, then set its left and right
    Node* newnode = new Node();
    newnode->token.symbol = temp2->token.symbol + temp1->token.symbol;
    newnode->token.val = temp2->token.val + temp1->token.val;
    newnode->left = temp2;
    newnode->right = temp1;

    return newnode;
};

void Tree::printTree(Node *traverse, int level)
{
    cout << level << "\t";
    for (int i = 0; i < level; i++)
        cout << "\t";

    cout << traverse->token.symbol << "(" << traverse->token.val << ")" << endl;

    if (traverse->left)
        printTree(traverse->left, level + 1);
    if (traverse->right)
        printTree(traverse->right, level + 1);
};