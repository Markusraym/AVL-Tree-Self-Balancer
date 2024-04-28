#include <iostream>
#include <sstream>
#include <regex>
#include "AVL.h"
using namespace std;

/*
ISSUES FROM TEST CASE 15: 
Inserting and then removing that node results in unsuccessful when it shouldnt, but removing a node that isnt inserted results in unsuccessful.
When doing insert "m" 99999999 and then remove 99999999 should result in successful, but mine results in unsuccessful 

insert "m" 99999999, resulting in successful. Then, remove 99999999 results in unsuccessful. Then, insert "at" 9999999 results in successful.
Then, when search 99999999, it results in unsuccessful, when it should reult in "at". 

insert "l" 10000065, it outputs successful. Then, insert "bj" 10000065 results in successful, when it should be unsuccessful because duplicate. Then, 
insert "cx" 10000065 results in successful, when it should be unsuccessful because duplicate. Then search "l" results in unsuccessful, when it should 
result in 10000065. This happens because everytime I insert a node with the same UFID, the previous one is overwritten. So for me, "l" doesn't exist.

ISSUES FROM TEST CASE 14:
When inserting over a 100 unique nodes, it all outputs successful. But when doing printInorder, printPreorder, and printPostorder, it 
outputs only 4 nodes, with 3 of them being near the last nodes inserted and one being the 8th from last inserted, and are not in the correct order. 
 
 ISSUES FROM TEST CASE 10: 


*/


int balanceFactor = 0; // Used in the insert function to calculate the balance of every node inserted
Node* removePointer = nullptr; //pointer used for inorderN(), points to the Nth node I want to delete, part of helper function of removeInorder()
int removeCounter = 0;
int counter = 1;
int nodeCount = 0;
bool removeSuccessful = false;
bool isRootAssigned = false; //Used to initialize the root in Insert() if it's the first node of the tree
bool namefoundMatch = false;  //Used in search functions as to cout "unsuccessful" if the bool is false
bool idfoundMatch = false;
Node* root = nullptr; //root pointer for when tree is made
int levelCount = 0;
int treeHeight = 0;
bool duplicate = false;
int UFIDduplicate = 0;
void inputHelper(string line); //helps read commands from the user and calls the right functions
Node* Insert(Node*& root, string name, string ufid); //inserts new nodes into the tree
void printInorder(Node*& root);//traverses and prints the tree in inorder
void printPreorder(Node*& root); //traverses and prints the tree in preorder
void printPostorder(Node*& root); //traverses and prints the tree in post order
void searchName(Node*& node, string name); //This searches the tree given a name and outputs a ufid
void searchID(Node*& node, string ufid); //This searches the tree given a ufid and outputs a name
Node* removeID(Node* node, string ufid); //This removes a node given a ufid
int printLevelCount(Node* root); //This returns the amount of levels in the tree
int nodeCounter(Node*& root); //This returns the total node count of the tree
Node* inorderN(Node* node, int& N); //This function returns the Nth node from an inorder traversal
int height(Node* node);
int balance(Node* node);
Node* leftRightRotation(Node* node);
Node* rightLeftRotation(Node* node);
Node* rightRotation(Node* node);
Node* leftRotation(Node* node);
bool checkDuplicates(Node* node, bool& duplicate, int UFIDduplicate);

int main()
{
    AVLTree tree;
    //only private stuff deals with memory 
    string commandCountString = "";
    int commandCount = 0;
    string line;
    getline(cin, commandCountString);
    commandCount = stoi(commandCountString);
    for (int i = 0; i < commandCount; i++)
    {
        getline(cin, line);
        inputHelper(line);  //Call a helper function to seperate the getline i.e. insert "Mark" 12345678     
    }
    return 0;
}

void inputHelper(string line) //Use this function to split up the commands in each line and call the correct functions 
{
    istringstream in(line); //create an input stream from line
    string command; //variable to hold "insert" 
    string userName; //variable to hold "Mark"
    string userUfid; //variable to hold 12345678
    string space; //variable to hold " " between each word in the line variable
    in >> command; //read the first word from the input stream, this variable will either be something like "insert, printInorder, remove, printInorder, etc
    if (command == "insert") //if the command's first word is "insert, read the next two lines, name, and ufid and make a node from it 
    {
        //read in the command for the name and the userUFID, such as insert "Mark" 12345678
        getline(in, space, '"'); //getline command to get the space between insert and "Mark"
        getline(in, userName, '"'); //getline command will now have next variable contain the name "Mark"
        getline(in, space, ' '); //getline will compensate for empty space
        getline(in, userUfid, ' '); //getline command to get the userUFID

        //check if "brandon" doesnt have a dollar sign, only A-z and no spaces, and ufid is only 8 digits with no A-z 
        regex obj = regex("^[a-zA-Z\\s]*$");
        string myText = string(userName);
        bool userNameValid = regex_match(myText, obj);

        //check if userUfid, 12345678, if it's only a 8 digit ufid
        obj = regex("^[0-9]{8}$");
        myText = string(userUfid);
        bool ufidValid = regex_match(myText, obj);

        //check if the userUfid, 12345678 is already inserted into the tree
        int intufid = stoi(userUfid);
        checkDuplicates(root, duplicate, intufid);

        if (ufidValid == false || userNameValid == false || duplicate == true)
        {
            cout << "unsuccessful" << '\n';
        }
        else
        {
            cout << "successful" << '\n';
        }

        if (userNameValid == true && ufidValid == true && duplicate == false)
        {
            Insert(root, userName, userUfid); //If both name and id are valid, insert it into the tree
        }
        duplicate = false;

    }
    else if (command == "printInorder")
    {
        counter = 1;
        nodeCount = nodeCounter(root);
        printInorder(root);
    }
    else if (command == "printPreorder")
    {
        counter = 1;
        nodeCount = nodeCounter(root);
        printPreorder(root);
    }
    else if (command == "printPostorder")
    {
        counter = 1;
        nodeCount = nodeCounter(root);
        printPostorder(root);
    }
    else if (command == "printLevelCount")
    {
        cout << printLevelCount(root) << '\n';
    }
    else if (command == "removeInorder")
    {
        string Nth;
        int N;
        getline(in, space, ' '); //getline command to get the space between removeInorder and N
        getline(in, Nth, ' '); //getline command will now have next variable contain the Nth item from the tree inorder
        N = stoi(Nth); //Convert the Nth variable into an integer to pass to removeInorder()
        nodeCount = nodeCounter(root);
        if (N > nodeCount || N < 0)
        {
            cout << "unsuccessful" << '\n';
        }
        else
        {
            removePointer = inorderN(root, N);

            string ufidconverted; //Using stringstream to convert the int ufid to a string ufid 
            stringstream convertIntToString;
            convertIntToString << removePointer->ufid;
            convertIntToString >> ufidconverted;


            removeID(root, ufidconverted);
            cout << "successful" << '\n';
        }
    }
    else if (command == "search")
    {
        if (line.find('"') != std::string::npos) //if the line contains quotation marks, its a name
        {
            getline(in, space, '"'); //getline command to get the space
            getline(in, userName, '"');
            searchName(root, userName);
            if (namefoundMatch == false)
            {
                cout << "unsuccessful" << '\n';
            }
            namefoundMatch = false;
        }
        else //if the line doesn't contain quotation marks, its a ufid
        {
            in >> userUfid;
            searchID(root, userUfid);
            if (idfoundMatch == false)
            {
                cout << "unsuccessful" << '\n';
            }
            idfoundMatch = false;
        }

    }
    else if (command == "remove")
    {
        getline(in, space, ' '); //getline command to get the space between remove and ID
        getline(in, userUfid, ' '); //getline command will now have next variable contain the UFID
        removeID(root, userUfid);
        if (removeSuccessful == false)
        {
            cout << "unsuccessful" << '\n';
        }
        else
        {
            cout << "successful" << '\n';
        }
        removeSuccessful = false;
    }
    else
    {
        cout << "unsuccessful" << '\n';
    }

}


Node* newNode(string name, int ufid)
{
    Node* node = new Node;
    node->name = name;
    node->ufid = ufid;
    node->height = 0;
    node->left = nullptr;
    node->right = nullptr;

    return node;
}



Node* Insert(Node*& node, string name, string ufid) //Add a student node with their name and ID to tree and balance the tree 
{
    int intUfid;
    intUfid = stoi(ufid); //Convert ufid from string to int
    if (isRootAssigned == false)
    {
        root = newNode(name, intUfid); //level = 0
        isRootAssigned = true;
        return root;
    }

    if (node == nullptr)
    {
        return newNode(name, intUfid);
    }
    else if (intUfid < node->ufid)
    {
        node->left = Insert(node->left, name, ufid);
    }
    else
    {
        node->right = Insert(node->right, name, ufid);
    }

    //credit for the line below goes to the stepik solutions 5.2a. Never used the max function before this project
    node->height = max(height(node->left), height(node->right)) + 1;

    balanceFactor = balance(node); //Calculate the balance factor of the node, and do rotations depending on it

    if (balanceFactor < -1) //IF TREE IS RIGHT HEAVY
    {
        if (balance(node->right) > 0) //IF TREE'S RIGHT SUBTREE IS LEFT HEAVY
        {
            //Perform right left rotation
            node = rightLeftRotation(node);
        }
        else //TREES RIGHT SUBTREE IS RIGHT HEAVY
        {
            //Perform left rotation 
            node = leftRotation(node);
        }
    }
    else if (balanceFactor > 1) //IF TREE IS LEFT HEAVY
    {
        if (balance(node->left) < 0) //IF THE TREE'S LEFT SUBTREE IS RIGHT HEAVY
        {
            //perform left right rotation
            node = leftRightRotation(node);
        }
        else //TREES LEFT SUBTREE IS LEFT HEAVY
        {
            //perform right rotation
            node = rightRotation(node);
        }
    }

    return node;
}

//CREDIT FOR THIS HEIGHT FUNCTION GOES TOWARDS THE STEPIK 5.2A SOLUTIONS PDF ON CANVAS
int height(Node* node)
{
    if (!node)
    {
        return -1;
    }
    return 1 + max(height(node->left), height(node->right));
}

int balance(Node* node)
{
    if (node == nullptr) //if its a leaf node, the balance is 0 
    {
        return 0;
    }
    return height(node->left) - height(node->right); //return the balance between left and right height
}

//CREDIT FOR THIS FUNCTION GOES TOWARDS PROFESSOR AMAN AND HIS SLIDES, 4 - BALANCED TREES.PDF SLIDE NUMBER 13
Node* rightRotation(Node* node)
{
    Node* grandchild = nullptr;
    Node* newParent = nullptr;
    grandchild = node->left->right;
    newParent = node->left;
    newParent->right = node;
    node->left = grandchild;
    newParent->height = max(height(newParent->left), height(newParent->right)) + 1;
    return newParent;

}

//CREDIT FOR THIS FUNCTION GOES TOWARDS PROFESSOR AMAN AND HIS SLIDES, 4 - BALANCED TREES.PDF SLIDE NUMBER 13
Node* leftRotation(Node* node)
{
    Node* grandchild = nullptr;
    Node* newParent = nullptr;
    grandchild = node->right->left;
    newParent = node->right;
    newParent->left = node;
    node->right = grandchild;
    newParent->height = max(height(newParent->left), height(newParent->right)) + 1;
    return newParent;
}

Node* rightLeftRotation(Node* node)
{
    node->right = rightRotation(node->right);
    node = leftRotation(node);
    return node;
}

Node* leftRightRotation(Node* node)
{
    node->left = leftRotation(node->left);
    node = rightRotation(node);
    return node;

}


Node* removeID(Node* node, string ufid) //Find and remove the account with the specified ID from the tree, prioritizing inorder successor
{
    int intUfid = stoi(ufid);
    //removeSuccessful = false;

    if (node == nullptr) //if the root is null, return null
    {
        return nullptr;
    }
    if (intUfid < node->ufid) //if item is less than data at local root, return result of deleting from left subtree
    {
        node->left = removeID(node->left, ufid);
    }
    else if (intUfid > node->ufid)
    {
        root->right = removeID(node->right, ufid); //if item is greater than local root, reutnr result of deleting from right subtree
    }
    else //The item is in the local root
    {
        removeSuccessful = true;
        if (node->right == nullptr && node->left == nullptr) // Node has no children, set parent of local root to reference null 
        {
            //cout << "The node has no children executed" << '\n';
            //cout << "The node to be deleted is: " << node->name << " " << node->ufid << '\n';
            if (node == root) //This is in case we're deleting the root, so we need to update where the new root is
            {
                root = nullptr;
                isRootAssigned = false;
            }
            delete node;
            //parent->left = nullptr; 
            node = nullptr;
        }
        //if the root has 1 child
        else if (node->right == nullptr) // && node->left != nullptr) //it has 1 left child, set parent of local root to reference the child
        {
            //cout << "the node has 1 left child executed" << '\n';
            //cout << "The node to be deleted is: " << node->name << " " << node->ufid << '\n';
            Node* tempPointer = node;
            node = node->left;
            if (tempPointer == root) //This is in case we're deleting the root, so we need to update where the new root is
            {
                root = node;
            }
            delete tempPointer;

        }
        else if (node->left == nullptr) // && node->right != nullptr) //it has 1 right child, set parent of local root to reference the child
        {
            //cout << "the node has 1 right child executed" << '\n';
            Node* tempPointer = node;
            node = node->right;
            if (tempPointer == root) //This is in case we're deleting the root, so we need to update where the new root is 
            {
                root = node;
            }
            delete tempPointer;
            /*
            node* temp = Node* current;
            parent->right = node->right;
            delete temp;
            */
        }
        else //The node has two children :( We must find the inorder successor, copy the data into the local root and delete the successor 
        {
            //cout << "The node has two children executed" << '\n';
            Node* tempPointer = node->right; //Set a temporary pointer to the right child of the node to be deleted
            while (tempPointer->left != nullptr) //Keep moving the temporary pointer as far left as it can go
            {
                tempPointer = tempPointer->left;
            }
            string ufidconverted; //Using stringstream to convert the int ufid to a string ufid 
            stringstream convertIntToString;
            convertIntToString << tempPointer->ufid;
            convertIntToString >> ufidconverted;

            node->ufid = tempPointer->ufid; //Swap the node to be deleted value with the inorder successor's value
            node->name = tempPointer->name; //Swap the node to be deleted name with the inorder successor's value
            node->right = removeID(node->right, ufidconverted); //Call RemoveID and then delete the inorder successsor for a finished deletion

        }
    }
    return node;
}

void searchID(Node*& node, string ufid) //search for student by ID, if found, print out their name. if not, print unsuccessful
{
    int integerUfid = stoi(ufid);
    if (node == nullptr)
    {
        return;
    }
    else if (node->ufid == integerUfid)
    {
        cout << node->name << '\n';
        idfoundMatch = true;
    }
    else if (integerUfid < node->ufid)
    {
        searchID(node->left, ufid);
    }
    else
    {
        searchID(node->right, ufid);
    }
}


void searchName(Node*& node, string name) //search for student by name, if found, print ID, if not, print unsuccessful
{
    if (node == nullptr)
    {
        return;
    }

    if (node->name == name)
    {
        cout << node->ufid << '\n';
        namefoundMatch = true;
    }
    searchName(node->left, name);
    searchName(node->right, name);
}

int nodeCounter(Node*& node) //This function returns the total count of the nodes, specifically to be used for the print functions 
{
    if (node == nullptr)
    {
        return 0;
    }
    int rightChild = nodeCounter(node->right);
    int leftChild = nodeCounter(node->left);
    return rightChild + 1 + leftChild;
}

void printInorder(Node*& node) //Print out the names of the tree in inorder, seperated by commas
{
    if (node == nullptr) {
        return;
    }
    printInorder(node->left);
    if (counter < nodeCount) //This is so that if it's printing the last node, do not add a comma to it when printing it
    {
        cout << node->name << ", ";
        counter++;
    }
    else
    {
        cout << node->name << '\n';
    }
    printInorder(node->right);
}


void printPreorder(Node*& node) //Print out the names of the tree in preorder, seperated by commas
{
    if (node == nullptr)
    {
        return;
    }
    if (counter < nodeCount) //This is so that if it's printing the last node, do not add a comma to it when printing it. 
    {
        cout << node->name << ", ";
        counter++;
    }
    else
    {
        cout << node->name << '\n';
    }
    printPreorder(node->left);
    printPreorder(node->right);
}

void printPostorder(Node*& node) //Print out the names of the tree in postorder, seperated by commas
{
    if (node == nullptr)
    {
        return;
    }
    printPostorder(node->left);
    printPostorder(node->right);
    if (counter < nodeCount) //This is so that if it's printing the last node, do not add a comma to it when printing it. 
    {
        cout << node->name << ", ";
        counter++;
    }
    else
    {
        cout << node->name << '\n';
    }
}


int printLevelCount(Node* node) //Prints the number of levels that exist in the tree, print 0 if head is null
{
    if (root == nullptr)
    {
        return 0;
    }
    return height(root) + 1; //God this was so simple, my old printLevelCount function was like 30 lines
}

Node* inorderN(Node* node, int& N) //This function returns the Nth node from an inorder traversal
{
    if (node == nullptr) {
        return 0;
    }
    inorderN(node->left, N);
    if (removeCounter == N)
    {
        removePointer = node;
        removeCounter = 0;
        return removePointer;
    }
    else
    {
        removeCounter++;
    }
    inorderN(node->right, N);
}

bool checkDuplicates(Node* node, bool& duplicate, int UFIDduplicate)
{
    if (node == nullptr) {
        return duplicate;
    }
    checkDuplicates(node->left, duplicate, UFIDduplicate);
    if (node->ufid == UFIDduplicate) //This is so that if it's printing the last node, do not add a comma to it when printing it
    {
        duplicate = true;
        return duplicate;
    }
    checkDuplicates(node->right, duplicate, UFIDduplicate);
}
