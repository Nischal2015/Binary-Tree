#pragma once

class BST  
{ 
    int data; 
    BST *left, *right; 
  
public: 
    // Default constructor. 
    BST(); 
  
    // Parameterized constructor. 
    BST(int); 
  
    // Insert function. 
    BST* Insert(BST*, int); 

    
    // Min Value Node
    BST* minValueNode(BST*);

    // Delete function
    BST* Delete(BST*, int);
    

    // Search function
    BST* Search(BST*, int);

    // Inorder traversal. 
    /*void Inorder(BST*); */

    // Calculate X position
    sf::Vector3f node_Position(int, float);

    // Check for node duplicacy
    bool duplicate(int);
}; 