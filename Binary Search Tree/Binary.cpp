#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Binary.h"

std::vector<int> node_Search_or_Input_Data;
std::vector<int> node_Delete_Data;

BST ::BST() : data(0), left(NULL), right(NULL) { } 
  
// Parameterized Constructor definition. 
BST ::BST(int value) 
{ 
    data = value; 
    left = right = nullptr; 
} 
  
// Insert function definition. 
BST* BST::Insert(BST* root, int value) 
{ 
    if (!root)  
    { 
        // Insert the first node, if root is NULL. 
        return new BST(value); 
    } 

    // Keeps records of the nodes traversed during insert process
    node_Search_or_Input_Data.push_back(root->data);
    
    // Insert data. 
    if (value > root->data)  
    { 
        // Insert right node data, if the 'value' 
        // to be inserted is greater than 'root' node data. 
  
        // Process right nodes. 
        root->right = Insert(root->right, value); 
    } 
    else if(value < root->data)
    { 
        // Insert left node data, if the 'value' 
        // to be inserted is greater than 'root' node data. 
  
        // Process left nodes. 
        root->left = Insert(root->left, value); 
    } 
    
    else
        std::cerr << "Data already exits in tree. Cannot enter twice." << std::endl;

    // Return 'root' node, after insertion. 
    return root; 
}


BST* BST::minValueNode(BST* node)
{
    if(node == nullptr || node->left == nullptr)
        return node;

    return minValueNode(node->left);
}

// Delete function definition
BST* BST::Delete(BST* root = nullptr, int key = 0)
{
    // base case
    if (root == nullptr)
    {
        std::cout << "Such data doesn't exist. Cannot delete." << std::endl;
        return root;
    }
    // If the key to be deleted is  smaller than the root'skey, then it lies in left subtree
    if (key < root->data)
        root->left = Delete(root->left, key);
 
    // If the key to be deleted is greater than the root's key, then it lies in right subtree
    else if (key > root->data)
        root->right = Delete(root->right, key);
 
    // if key is same as root's key, then This is the node to be deleted
    else
    {
        // node with only one child or no child
        if (root->left == nullptr) {
            BST* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            BST* temp = root->left;
            delete root;
            return temp;
        }
 
        // node with two children: Get the inorder successor
        // (smallest in the right subtree)
        BST* temp = minValueNode(root->right);
 
        // Copy the inorder successor's content to this node
        root->data = temp->data;
 
        // Delete the inorder successor
        root->right = Delete(root->right, temp->data);
    }
    return root;
}


BST* BST::Search(BST* root = nullptr, int key = 0)
{
    // Base Cases: root is null or key is present at root
    if (root == nullptr) 
    {
        std::cout << "Data not found." << std::endl;
        node_Search_or_Input_Data.clear();
        return root; 
    }

    // Keeps record of the nodes traversed during search process
    if(key == root->data)
        return root;

    node_Search_or_Input_Data.push_back(root->data);

    // Key is smaller than root's key
    if(key < root->data)
        return Search(root->left, key);

    // Key is greater than root's key 
    return Search(root->right, key);
}

// Inorder traversal function.
// This gives data in sorted order.
/*void BST ::Inorder(BST* root)
{ 
    if (!root) { 
        return; 
    } 
    Inorder(root->left); 
    cout << root->data << endl; 
    Inorder(root->right); 
} */

// Node Position Calculation Definition
sf::Vector3f BST::node_Position(int node_Check, float window_Size_Width)
{
    int count1 = 0;
    float center = window_Size_Width / 2;
    float before = center;
    float total = center;
    float height = 50.f;

    if (node_Search_or_Input_Data.size() == 0)
         return sf::Vector3f(total, height, before);

    for(auto i : node_Search_or_Input_Data)
    {
        std::cout << "Traverse NODES: " << i << std::endl;
        ++count1;
        height += 100;
        center /= 2;
        if(node_Check < i)
        {   
            total -= center;
            if(count1 < node_Search_or_Input_Data.size())
                before -= center;
        }
        else if(node_Check > i)
        {
            total += center;
            if(count1 < node_Search_or_Input_Data.size())
                before += center;
        }
    }
    node_Search_or_Input_Data.clear();
    return sf::Vector3f(total, height, before);
}

// Checks if same node has been inserted twice
bool BST::duplicate(int node_Content)
{
    for(auto i : node_Search_or_Input_Data)
    {
        if(i == node_Content)
        {
            node_Search_or_Input_Data.clear();
            return true;
        }
    }
    return false;
}