// EB
// Implementing Red-Black Tree in C++

#include <iostream>

namespace RBT
{
  struct Node
  {
    int data;
    std::string name;
    Node *parent;
    Node *left;
    Node *right;
    int color;
  };
}

class RedBlackTree
{
private:
  RBT::Node *root;
  int totalNodes;
  int height;
  void preorderTraverse(RBT::Node *, std::pair<std::string, int>[], int);
  void inorderTraverse(RBT::Node *, std::pair<std::string, int>[], int);
  void postorderTraverse(RBT::Node *, std::pair<std::string, int>[], int);
  RBT::Node *searchTreeRecursive(RBT::Node *, int);
  void replaceNode(RBT::Node *, RBT::Node *);
  void leftRotate(RBT::Node *);
  void rightRotate(RBT::Node *);
  void insertFix(RBT::Node *);
  void deleteFix(RBT::Node *);

public:
  RedBlackTree();
  void preorder(std::pair<std::string, int>[], int);
  void inorder(std::pair<std::string, int>[], int);
  void postorder(std::pair<std::string, int>[], int);
  RBT::Node *searchTree(int);
  RBT::Node *successor(RBT::Node *);
  RBT::Node *predecessor(RBT::Node *);
  void insert(std::string, int);
  void deleteNode(int);
  int getHeight();
  int getHeightRecursive(RBT::Node *);
  RBT::Node *getMaximum();
  RBT::Node *getMinimum();
  int getTotalNodes();
};

// Constructor
RedBlackTree::RedBlackTree()
{
  root = NULL;
  totalNodes = 0;
  height = 0;
}

// Function for traversing the dataset in preorder
void RedBlackTree::preorder(std::pair<std::string, int> orderedPairs[], int index)
{
  preorderTraverse(root, orderedPairs, index);
}

// To preorder traverse recursively
void RedBlackTree::preorderTraverse(RBT::Node *node, std::pair<std::string, int> orderedPairs[], int index)
{
  if (node == nullptr) // If node is null, return
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
void RedBlackTree::inorder(std::pair<std::string, int> orderedPairs[], int index)
{
  inorderTraverse(root, orderedPairs, index);
}

// To inorder traverse recursively
void RedBlackTree::inorderTraverse(RBT::Node *node, std::pair<std::string, int> orderedPairs[], int index)
{
  if (node == nullptr) // If node is null, return
  {
    return;
  }

  // Recursive call to left
  inorderTraverse(node->left, orderedPairs, index);

  orderedPairs[index++] = std::make_pair(node->name, node->data);

  // Recursive call to right
  inorderTraverse(node->right, orderedPairs, index);
}

// Function for traversing the dataset in postorder
void RedBlackTree::postorder(std::pair<std::string, int> orderedPairs[], int index)
{
  postorderTraverse(root, orderedPairs, index);
}

// To postorder traverse recursively
void RedBlackTree::postorderTraverse(RBT::Node *node, std::pair<std::string, int> orderedPairs[], int index)
{
  if (node == nullptr) // If node is null, return
  {
    return;
  }

  // Recursive call to left
  postorderTraverse(node->left, orderedPairs, index);

  // Recursive call to right
  postorderTraverse(node->right, orderedPairs, index);

  orderedPairs[index++] = std::make_pair(node->name, node->data);
}

// Function to search a node in the tree
RBT::Node *RedBlackTree::searchTree(int targetData)
{
  return searchTreeRecursive(root, targetData);
}

// To search a node recursively
RBT::Node *RedBlackTree::searchTreeRecursive(RBT::Node *node, int targetData)
{
  if (node == nullptr || node->data == targetData) // If node is null or target is found
  {
    return node;
  }

  if (targetData < node->data) // If target is less than the node, search left
  {
    return searchTreeRecursive(node->left, targetData);
  }
  else // If target is greater than the node, search right
  {
    return searchTreeRecursive(node->right, targetData);
  }
}

// Function to find the successor of a node
RBT::Node *RedBlackTree::successor(RBT::Node *node)
{
  if (node == nullptr) // If node is null, return null
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
  RBT::Node *parent = node->parent;
  while (parent != nullptr && node == parent->right)
  {
    node = parent;
    parent = parent->parent;
  }

  return parent;
}

// Function to find the predecessor of a node
RBT::Node *RedBlackTree::predecessor(RBT::Node *node)
{
  if (node == nullptr) // If node is null, return null
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
  RBT::Node *parent = node->parent;
  while (parent != nullptr && node == parent->left)
  {
    node = parent;
    parent = parent->parent;
  }

  return parent;
}

// Function to replace a node
void RedBlackTree::replaceNode(RBT::Node *targetNode, RBT::Node *replacementNode)
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
    
    replacementNode->parent = targetNode->parent; // Set the parent of the replacement node
  }
}

// Function to left rotate a node
void RedBlackTree::leftRotate(RBT::Node *node)
{
  RBT::Node *rightChild = node->right;
  // Move right child's left subtree to node's right subtree
  node->right = rightChild->left;
  if (rightChild->left != nullptr)
  {
    rightChild->left->parent = node;
  }
  // Link node's parent to right child
  rightChild->parent = node->parent;
  if (node->parent == nullptr)
  {
    // If node is root, make right child the root
    root = rightChild;
  }
  else if (node == node->parent->left)
  {
    // If node is left child, make right child the left child of node's parent
    node->parent->left = rightChild;
  }
  else
  {
    // If node is right child, make right child the right child of node's parent
    node->parent->right = rightChild;
  }

  // Put node on right child's left
  rightChild->left = node;

  // Put right child as node's parent
  node->parent = rightChild;
}

// Function to right rotate a node
void RedBlackTree::rightRotate(RBT::Node *node)
{
  // Move left child's right subtree to node's left subtree
  RBT::Node *leftChild = node->left;
  node->left = leftChild->right;
  if (leftChild->right != nullptr)  // If left child's right subtree is not empty
  {
    leftChild->right->parent = node;
  }
  leftChild->parent = node->parent;
  if (node->parent == nullptr)  // If node is root
  {
    root = leftChild;
  }
  else if (node == node->parent->right) // If node is right child
  {
    node->parent->right = leftChild;
  }
  else  // If node is left child
  {
    node->parent->left = leftChild;
  }

  // Put node on left child's right
  leftChild->right = node;
  node->parent = leftChild;
}

// Function to fixup the tree after insertion
void RedBlackTree::insertFix(RBT::Node *node)
{
  RBT::Node *uncle = nullptr;
  while (node != root && node->parent != nullptr && node->parent->color == 1) // If node is not root and node's parent is red
  {
    if (node->parent == node->parent->parent->right)  // If node's parent is right child
    {
      uncle = node->parent->parent->left; // Get uncle
      if (uncle != nullptr && uncle->color == 1)  // If uncle is red
      {
        // Recolor
        uncle->color = 0; // Uncle is black
        node->parent->color = 0;  // Parent is black
        node->parent->parent->color = 1;  // Grandparent is red
        node = node->parent->parent;  // Move node to grandparent
      }
      else
      {
        if (node == node->parent->left) // If node is left child
        {
          node = node->parent;
          rightRotate(node);
        }
        node->parent->color = 0;  // Parent is black
        node->parent->parent->color = 1;  // Grandparent is red
        leftRotate(node->parent->parent);
      }
    }
    else  // If node's parent is left child
    {
      uncle = node->parent->parent->right;  // Get uncle

      if (uncle != nullptr && uncle->color == 1)  // If uncle is red
      {
        // Recolor  
        uncle->color = 0; // Uncle is black
        node->parent->color = 0;  // Parent is black
        node->parent->parent->color = 1;  // Grandparent is red
        node = node->parent->parent;  // Move node to grandparent
      }
      else
      {
        if (node == node->parent->right)  // If node is right child
        {
          node = node->parent;
          leftRotate(node);
        }
        node->parent->color = 0;  // Parent is black
        node->parent->parent->color = 1;  // Grandparent is red
        rightRotate(node->parent->parent);  // Right rotate grandparent
      }
    }
  }
  if (node->parent == nullptr)  // If node is root
  {
    root->color = 0;
  }
}

// Function to insert a node in the tree
void RedBlackTree::insert(std::string name, int targetData)
{
  // Create new node
  RBT::Node *newNode = new RBT::Node;
  newNode->data = targetData;
  newNode->name = name;
  newNode->parent = nullptr;
  newNode->left = nullptr;
  newNode->right = nullptr;
  newNode->color = 1;

  RBT::Node *current = root;  // Current node
  RBT::Node *parent = nullptr;

  if (root == nullptr)  // If tree is empty, insert at root
  {
    root = newNode;
    newNode->color = 0;
    totalNodes++;
    return;
  }

  while (current != nullptr)  // Traverse the tree to find the position
  {
    parent = current;
    if (newNode->data < current->data)  // If node is less than current node, go left
    {
      current = current->left;
    }
    else
    {
      current = current->right;
    }
  }

  newNode->parent = parent;

  if (newNode->data < parent->data) // If node is less than parent, insert left
  {
    parent->left = newNode;
  }
  else
  {
    parent->right = newNode;
  }

  totalNodes++; // Increment total nodes
  insertFix(newNode); // Fix the tree
}

// Function to fixup the tree after deletion
void RedBlackTree::deleteFix(RBT::Node *node)
{
  RBT::Node *uncle = nullptr;
  while (node != root && node->color == 0)  // If node is not root and node is black
  {
    if (node == node->parent->left) // If node is left child
    {
      uncle = node->parent->right;  // Get uncle

      if (uncle != nullptr && uncle->color == 1)  // If uncle is red
      {
        uncle->color = 0; // Uncle is black
        node->parent->color = 1;  // Parent is red
        leftRotate(node->parent); // Left rotate parent
        uncle = node->parent->right;  // Get uncle
      }

      if (uncle->left == nullptr && uncle->left->color == 0 &&
          uncle->right == nullptr && uncle->right->color == 0)  // If uncle's children are black
      {
        uncle->color = 1; // Uncle is red
        node = node->parent;  // Move node to parent
      }
      else  // If uncle's children are not black
      {
        if (uncle != nullptr && uncle->right == nullptr && uncle->right->color == 0)  // If uncle's right child is black
        {
          if (uncle->left != nullptr) 
            uncle->left->color = 0;
          uncle->color = 1; // Uncle is red
          rightRotate(uncle); // Right rotate uncle
          uncle = node->parent->right;  
        }
        {
          if (uncle->left != nullptr) 
            uncle->left->color = 0;
          uncle->color = 1; // Uncle is red
          rightRotate(uncle); // Right rotate uncle
          uncle = node->parent->right; 
        }
        if (uncle != nullptr) 
        {
          uncle->color = node->parent->color; // Uncle is parent's color
          node->parent->color = 0;  // Parent is black
          if (uncle->right != nullptr)  
            uncle->right->color = 0;  // Uncle's right child is black
        }

        leftRotate(node->parent); // Left rotate parent
        node = root;  // Move node to root
      }
    }
    else  // If node is right child
    {
      uncle = node->parent->left; // Get uncle

      if (uncle != nullptr && uncle->color == 1)  // If uncle is red
      {
        uncle->color = 0; // Uncle is black
        node->parent->color = 1;  // Parent is red
        rightRotate(node->parent);  // Right rotate parent
        uncle = node->parent->left; 
      }

      if (uncle != nullptr && uncle->right != nullptr && uncle->left != nullptr &&
          uncle->right->color == 0 && uncle->left->color == 0)  // If uncle's children are black
      {
        uncle->color = 1; // Uncle is red
        node = node->parent;  // Move node to parent
      }
      else  // If uncle's children are not black
      {
        if (uncle != nullptr && uncle->left != nullptr && uncle->left->color == 0)  // If uncle's left child is black
        {
          if (uncle->right != nullptr) 
            uncle->right->color = 0;  
          uncle->color = 1; // Uncle is red
          rightRotate(uncle); // Right rotate uncle
          uncle = node->parent->left;
        }

        if (uncle != nullptr)
        {
          uncle->color = node->parent->color; // Uncle is parent's color
          node->parent->color = 0;  // Parent is black
          if (uncle->right != nullptr) 
            uncle->right->color = 0;  
          leftRotate(node->parent); // Left rotate parent
          node = root;  // Move node to root
        }
      }
      {
        if (uncle != nullptr && uncle->left != nullptr && uncle->left->color == 0)  // If uncle's left child is black
        {
          if (uncle->right != nullptr)
            uncle->right->color = 0;
          uncle->color = 1; // Uncle is red
          leftRotate(uncle);  // Left rotate uncle
          uncle = node->parent->left;
        }

        if (uncle != nullptr)
        {
          uncle->color = node->parent->color; // Uncle is parent's color
          node->parent->color = 0;  // Parent is black
          if (uncle->left != nullptr) 
            uncle->left->color = 0;
          rightRotate(node->parent);  // Right rotate parent
          node = root;  // Move node to root
        }
      }
    }
  }

  if (node != nullptr) 
  {
    node->color = 0;  // Node is black
  }
}

// Function to delete a node from the tree
void RedBlackTree::deleteNode(int targetData)
{
  RBT::Node *targetNode = searchTree(targetData); // Search for the node to be deleted
  if (targetNode == nullptr)  // If node is not found, return
  {
    return;
  }

  RBT::Node *replacementNode = nullptr;
  RBT::Node *successorNode = nullptr;
  int originalColor = 0;

  if (targetNode->left == nullptr && targetNode->right == nullptr)  // If node is leaf
  {
    successorNode = targetNode; 
  } 
  else  // If node is not leaf
  {
    successorNode = successor(targetNode);  // Get the successor of the node
  }

  if (successorNode->left != nullptr)
  {
    replacementNode = successorNode->left;  // Get the left child of the successor
  }
  else
  {
    replacementNode = successorNode->right; // Get the right child of the successor
  }

  replaceNode(successorNode, replacementNode); // Replace successor node with replacement node

  if (successorNode != targetNode)  // If successor node is not the node to be deleted
  {
    targetNode->data = successorNode->data; // Copy successor node's data 
    targetNode->name = successorNode->name; // Copy successor node's name
  }

  originalColor = successorNode->color; // Get the color of the successor node

  delete successorNode;

  if (originalColor == 0 && replacementNode != nullptr) // If original color is black and replacement node is not null
  {
    deleteFix(replacementNode); // Fix the tree
  }

  totalNodes--; // Decrement total nodes
}

// Function to get the height of the tree
int RedBlackTree::getHeight()
{
  return getHeightRecursive(root) - 1;  
}

// To get the height of the tree recursively
int RedBlackTree::getHeightRecursive(RBT::Node *node)
{
  if (node == nullptr)
  {
    return 0;
  }

  int leftHeight = getHeightRecursive(node->left);  // Get the height of left subtree
  int rightHeight = getHeightRecursive(node->right);  // Get the height of right subtree

  return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
}

// Function to get the maximum node
RBT::Node *RedBlackTree::getMaximum()
{
  RBT::Node *current = root;
  while (current->right != nullptr)
  {
    current = current->right; // Traverse to the rightmost node
  }
  return current;
}

// Function to get the minimum node
RBT::Node *RedBlackTree::getMinimum()
{
  RBT::Node *current = root;
  while (current->left != nullptr)
  {
    current = current->left;  // Traverse to the leftmost node
  } 
  return current;
}

// Function to get the total number of nodes
int RedBlackTree::getTotalNodes()
{
  return totalNodes;  // Return total nodes
}