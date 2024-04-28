#include <iostream>
using namespace std;
#pragma once

struct Node {
	string name = " ";
	int ufid = 0;
	int height;
	Node* left = nullptr;
	Node* right = nullptr;
	//Node(string name, string ufid) : name(name), ufid(ufid), left(nullptr), right(nullptr) {} //default constuctor
};

class AVLTree {
private:
	Node* InsertHelper(Node* node, string name, string ufid, AVLTree tree); //Make this private so it can't be called in main
public:
	Node* rootNode;
	//CREDIT TOWARDS THIS INSERTFUNCT AND INSERTHELPER GOES TOWARDS ANDREW PENTON'S YOUTUBE VIDEO "AVL PROJECTBREAKDOWN/Q&A SESSION
	void InsertFunct(string name, string ufid, AVLTree tree) { this->rootNode = InsertHelper(this->rootNode, name, ufid, tree); }
	AVLTree() : rootNode() {}

};

//Steps
/* 1. Traverse through the tree starting wit hthe root and insert the node in the correct spot using recurssion
   2. Get the balance factor of this ancestor node to check whether this node became unbalanced. As we're recurssing up thats when
   we'll be checking the balance of each node to make sure it didn't become unbalanced. If it did, we'll have to perform rotations.
   As we're going up, we'll also want to update the height of that node.
   3. Prints "successful" if insertion is successful and print "unsuccessful" otherwise. If the value is already in the tree, print
   unsuccessful
   Start by inserting the node into the tree, find an empty spot. Then, as you go back up, start updating the heights and checking the
   balance for each node. Once you find the first node that is unbalanced on your way up, peform rotations. This will all be done during insert.
   Heights will also need to be updated during the rotation
   Doing stuff while recursing back up psudeocode example:
   Node* recursive(Node* n) {
   //base case:
   if (n == nullptr)
   {
   return nullptr;
   }
   cout << n->value << '\n'; //print the value
   //recursive call
   recursive(n->next); //then go onto the next node

   }

   1 -> 2 -> 3 -> 4 prints going forward from cout


	Node* recursive(Node* n) {
   //base case:
   if (n == nullptr)
   {
   return nullptr;
   }

   //recursive call
   n->next = recursive(n->next); //then go onto the next node

   cout << n->value << '\n'; //print the value
   //check and update heights and check balance factors and rotations

   }

   4 -> 3 -> 2 -> 1 prints backwards cout. before or after recurssibe call depends if itll be executed forwards or backwards
   */

   //Remove Command
   /*
   Find and remove the account with the specified ID from the tree; balancing is not required
   1. Ensure ID exists within the tree (Useful to use search())
   2. Traverse recursively through the tree to reach the desired node to delete
	   if (id < root->id) root->left = remove(root->left, id);
   3. Three scenarios (no children/one child/two children) to handle removing the node
   4. Make sure to return the correct node or null at the end to reconnect subtrees at the ancestor level

   If you remove a node with no children, just delete it, deallocate it, and set it so the parent is pointing to nullptr. What you return is what
   the parent ends up pointing to

   If you remove something with one child, return to what that one child is so the new parent is now pointing to that

   If you remove something with two children, replace it with the right subtree of the deleted node's left most node. Just copy the information
   of the left most node you're going to move to the one you're going to delete, and then remove the one you copied, the left most one

   //Remove inorder: gonna be given a number n, and remove the nth node from the inorder traversla of the tree, n= 0 for first node. print
   successful or unsuccesfful is n is outside the desired range. Like if we give you n thats bigger than the amount of nodes in a tree, no
   need to balance

   1. Find the nth node
   2. delete it (Remove() would be helpful)

   Finding the nth node:
   check the bounds of n
   Inorder traversal, but keep track of what node you're at
   Hint: check out slide 19 from disuccsion 1 ppt on canvas


   Tips:
   Start by to go ahead and start on parsing and building your mina()
   Work in the order of insert(), traverse, search(), and remove()
   Build your own test cases
   */

