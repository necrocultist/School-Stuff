// EB
// Implementing Binary Search Tree in C++

#include <iostream>

namespace BST
{
  struct Node
  {
    int data;
    std::string name;
    Node *parent;
    Node *left;
    Node *right;
  };
}

class BinarySearchTree
{
private:
  BST::Node *root;
  int totalNodes;
  int height;
  void preorderTraverse(BST::Node *, std::pair<std::string, int>[], int);
  void inorderTraverse(BST::Node *, std::pair<std::string, int>[], int);
  void postorderTraverse(BST::Node *, std::pair<std::string, int>[], int);
  BST::Node *searchTreeRecursive(BST::Node *, int);
  void replaceNode(BST::Node *, BST::Node *);

public:
  BinarySearchTree();
  void preorder(std::pair<std::string, int>[], int);
  void inorder(std::pair<std::string, int>[], int);
  void postorder(std::pair<std::string, int>[], int);
  BST::Node *searchTree(int);
  BST::Node *successor(BST::Node *);
  BST::Node *predecessor(BST::Node *);
  void insert(std::string, int);
  void deleteNode(int);
  int getHeight();
  int getHeightRecursive(BST::Node *);
  BST::Node *getMaximum();
  BST::Node *getMinimum();
  int getTotalNodes();
};

// Constructor
BinarySearchTree::BinarySearchTree()
{
  root = NULL;
  totalNodes = 0;
}

// Function for traversing the dataset in preorder
void BinarySearchTree::preorder(std::pair<std::string, int> orderedPairs[], int index)
{
  preorderTraverse(root, orderedPairs, index);
}

// To preorder traverse recursively
void BinarySearchTree::preorderTraverse(BST::Node *node, std::pair<std::string, int> orderedPairs[], int index)
{
  if (node == nullptr)  // If node is null, return
  {
    return;
  }

  orderedPairs[index++] = std::make_pair(node->name, node->data);

  // Recursive call to left
  preorderTraverse(node->left, orderedPairs, index);

  // Recursive call to right
  preorderTraverse(node->right, orderedPairs, index);
}

// Function for traversing the dataset in inorder
void BinarySearchTree::inorder(std::pair<std::string, int> orderedPairs[], int index)
{
  inorderTraverse(root, orderedPairs, index);
}

// To inorder traverse recursively
void BinarySearchTree::inorderTraverse(BST::Node *node, std::pair<std::string, int> orderedPairs[], int index)
{
  if (node == nullptr)  // If node is null, return
  {
    return;
  }

  orderedPairs[index++] = std::make_pair(node->name, node->data);

  // Recursive call to left
  inorderTraverse(node->left, orderedPairs, index);

  // Recursive call to right
  inorderTraverse(node->right, orderedPairs, index);
}

// Function for traversing the dataset in postorder
void BinarySearchTree::postorder(std::pair<std::string, int> orderedPairs[], int index)
{
  inorderTraverse(root, orderedPairs, index);
}

// To postorder traverse recursively
void BinarySearchTree::postorderTraverse(BST::Node *node, std::pair<std::string, int> orderedPairs[], int index)
{
  if (node == nullptr)  // If node is null, return
  {
    return;
  }

  // Recursive call to left
  inorderTraverse(node->left, orderedPairs, index);

  orderedPairs[index++] = std::make_pair(node->name, node->data);

  // Recursive call to right
  inorderTraverse(node->right, orderedPairs, index);
}

// Function to search for a node in the tree
BST::Node *BinarySearchTree::searchTree(int targetData)
{
  return searchTreeRecursive(root, targetData);
}

// To search for a node recursively
BST::Node *BinarySearchTree::searchTreeRecursive(BST::Node *node, int targetData)
{
  if (node == nullptr || node->data == targetData)  // If node is null or target is found
  {
    return node;
  }

  if (targetData < node->data)  // If target is less than the node, search left
  {
    // Recursive call to left
    return searchTreeRecursive(node->left, targetData);
  }
  else  // If target is greater than the node, search right
  {
    // Recursive call to right
    return searchTreeRecursive(node->right, targetData);
  }
}

// Function to find the successor of a node
BST::Node *BinarySearchTree::successor(BST::Node *node)
{
  if (node == nullptr)  // If node is null, return null
  {
    return nullptr;
  }

  // If right subtree is not empty, find the leftmost node
  if (node->right != nullptr)
  {
    node = node->right;
    while (node->left != nullptr)
    {
      node = node->left;
    }
    return node;
  }

  // If right subtree is empty, find the lowest ancestor of the node
  BST::Node *parent = node->parent;
  while (parent != nullptr && node == parent->right)
  {
    node = parent;
    parent = parent->parent;
  }

  return parent;
}

// Function to find the predecessor of a node
BST::Node *BinarySearchTree::predecessor(BST::Node *node)
{
  if (node == nullptr)  // If node is null, return null
  {
    return nullptr;
  }

  // If left subtree is not empty, find the rightmost node
  if (node->left != nullptr)
  {
    node = node->left;
    while (node->right != nullptr)
    {
      node = node->right;
    }
    return node;
  }

  // If left subtree is empty, find the lowest ancestor of the node
  BST::Node *parent = node->parent;
  while (parent != nullptr && node == parent->left)
  {
    node = parent;
    parent = parent->parent;
  }

  return parent;
}

// Function to replace a node
void BinarySearchTree::replaceNode(BST::Node *targetNode, BST::Node *replacementNode)
{
  if (targetNode->parent == nullptr)  // If target node is root
  {
    root = replacementNode; // make replacement node the root
  }
  else if (targetNode == targetNode->parent->left)  // If target node is left child
  {
    targetNode->parent->left = replacementNode;
  }
  else  // If target node is right child
  {
    targetNode->parent->right = replacementNode;
  }

  if (replacementNode != nullptr)
  {
    replacementNode->parent = targetNode->parent;   // Set the parent of the replacement node
  }
}

// Function to insert a node in the tree
void BinarySearchTree::insert(std::string name, int targetData)
{
  // Create a new node
  BST::Node *newNode = new BST::Node;
  newNode->data = targetData;
  newNode->name = name;
  newNode->parent = nullptr;
  newNode->left = nullptr;
  newNode->right = nullptr;

  BST::Node *currentNode = root;  // Current node
  BST::Node *parentNode = nullptr;

  if (root == nullptr)  // If tree is empty, insert at root
  {
    root = newNode;
    totalNodes++;
    return;
  }

  while (currentNode != nullptr)  // Traverse the tree to find the right position
  {
    parentNode = currentNode;
    if (newNode->data < currentNode->data)  // If node is less than the current node, go left
    {
      currentNode = currentNode->left;
    }
    else
    {
      currentNode = currentNode->right;
    }
  }

  newNode->parent = parentNode;
  if (newNode->data < parentNode->data) // If node is less than the parent node, insert left
  {
    parentNode->left = newNode;
  }
  else
  {
    parentNode->right = newNode;
  }

  totalNodes++; // Increment total nodes
}

// Function to delete a node
void BinarySearchTree::deleteNode(int targetData)
{
    BST::Node *node = searchTree(targetData); // Search for the node to be deleted
    if (node == nullptr)  // If node is not found, return
  {
    return;
  }

  if (node->left == nullptr)  // If node has no left child
  {
    replaceNode(node, node->right); // Replace node with its right child
  }
  else if (node->right == nullptr)  // If node has no right child
  {
    replaceNode(node, node->left);  // Replace node with its left child
  }
  else
  {
    BST::Node *successorNode = successor(node); // Find the successor of the node
    if (successorNode->parent != node)  
    {
      replaceNode(successorNode, successorNode->right); 
      successorNode->right = node->right; 
      successorNode->right->parent = successorNode; 
    }
    replaceNode(node, successorNode); 
    successorNode->left = node->left; 
    successorNode->left->parent = successorNode;  
  }

  totalNodes--; // Decrement total nodes
}

// Function to get the height of the tree
int BinarySearchTree::getHeight()
{
  return getHeightRecursive(root) - 1;
}

// To get the height of the tree recursively
int BinarySearchTree::getHeightRecursive(BST::Node *node)
{
  if (node == nullptr)
  {
    return 0;
  }

  int leftHeight = getHeightRecursive(node->left);  // Get the height of the left subtree
  int rightHeight = getHeightRecursive(node->right);  // Get the height of the right subtree

  return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
}

// Function to get the maximum node
BST::Node *BinarySearchTree::getMaximum()
{
  BST::Node *current = root;
  while (current->right != nullptr)
  {
    current = current->right; // Traverse to the rightmost node
  }
  return current;
}

// Function to get the minimum node
BST::Node *BinarySearchTree::getMinimum()
{
  BST::Node *current = root;
  while (current->left != nullptr)
  {
    current = current->left;  // Traverse to the leftmost node
  }
  return current;
}

// Function to get the total number of nodes
int BinarySearchTree::getTotalNodes()
{
  return totalNodes;  // Return total nodes
}