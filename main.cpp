//import stuff
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;

struct node{ //node struct with parent node, left/right nodes, a color, and a value
	node* parent = nullptr;
	node* left = nullptr;
	node* right = nullptr;
	char color = 'R';
	int value;
};

bool searchTree(node* current, int searchValue){ //recursive function that checks if a value is in the tree
	if (current->value == searchValue){//if current's value is equal to searchValue:
		return true; //return true (found value)
	}
	else{//if current's value is not equal to searchValue, recursively search down the tree based off whether or not we need to go down left or down right
		if (searchValue < current->value){ //search down left
			if (current->left != nullptr){
				return searchTree(current->left, searchValue);
			}
			else{
				return false;
			}
		}
		else{ //search down right
			if (current->right != nullptr){
				return searchTree(current->right, searchValue);
			}
			else{
				return false;
			}
		}
	}
}

node* getRoot(node* current){//recursive function that returns the root of the tree
	if (current->parent){
		return getRoot(current->parent);
	}
	else{
		return current;
	}
}

node* getGrandfather(node* current){//gets the grandfather of a node
	if (current->parent->parent){
		return current->parent->parent;
	}
	else{//if grandfather doesnt exist return nullptr
		return nullptr;
	}
}

node* getSibling(node* current){//gets the sibling of a node
	if (current == current->parent->left){
		return current->parent->right;
	}
	else if (current == current->parent->right){
		return current->parent->left;
	}
	else{
		return nullptr;
	}
}

node* getFarNephew(node* current){//gets the far nephew of a node
	node* sibling = getSibling(current);
	if (current->parent->right == sibling){
		return sibling->right;
	}
	else if (current->parent->left == sibling){
		return sibling->left;
	}
	else{
		return nullptr;
	}
}

node* getNearNephew(node* current){//gets the near nephew of a node
	node* sibling = getSibling(current);
	if (current->parent->right == sibling){
		return sibling->left;
	}
	else if (current->parent->left == sibling){
		return sibling->right;
	}
	else{
		return nullptr;
	}
}

node* getUncle(node* current){//gets the uncle of a node
	node* grandfather = getGrandfather(current);
	if (grandfather != nullptr){
		if (current->parent == grandfather->left){//gets opposite child of grandfather (the uncle)
			return grandfather->right;
		}
		else if (current->parent == grandfather->right){
			return grandfather->left;
		}
		else{
			return nullptr;
		}
	}
	else{
		return nullptr;
	}
}

node* leftRotation(node* current){ //left rotation function
	//gets currentRight and rightLeft
	node* Cright = current->right;
	node* rightLeft = Cright->left;
	//linking for left rotation
	Cright->left = current;
	current->right = rightLeft;
	Cright->parent = current->parent;
	if (current->parent != nullptr){ //if current is not root, link its parent's children
		if (current->value < current->parent->value){
			current->parent->left = Cright;
		}
		else{
			current->parent->right = Cright;
		}
	}
	current->parent = Cright;
	if (rightLeft != nullptr){//if rightLeft exists, set its parent to current
		rightLeft->parent = current;
	}
	return getRoot(Cright);//returns the root (since root may be changed here and it must be updated)
}

node* rightRotation(node* current){ //right rotation function
	//gets currentLeft and leftRight
	node* Cleft = current->left;
	node* leftRight = Cleft->right;
	//linking for right rotation
	Cleft->right = current;
	current->left = leftRight;
	Cleft->parent = current->parent;
	if (current->parent != nullptr){ //if current is not root, link its parent's children
		if (current->value < current->parent->value){
			current->parent->left = Cleft;
		}
		else{
			current->parent->right = Cleft;
		}
	}
	current->parent = Cleft;
	if (leftRight != nullptr){//if leftRight exists, set its parent to current
		leftRight->parent = current;
	}
	return getRoot(Cleft);//returns the root (since root may be changed here and it must be updated)
}

void swapColorScheme(node* &root, node* current){
	if (current == root){//if current is root, immediately set root to black
		current->color = 'B';
		return;
	}
	else{
		if (current->parent->parent == nullptr){//if current's grandfather doesn't exist, end function
			return;
		}
		if (current->parent->color != 'B'){ //if parent's color is not black:
			node* uncle = getUncle(current);
			node* grandfather = getGrandfather(current);
			node* parent = current->parent;
			//cout << "parent value: " << current->parent->value << endl;
			if (uncle and uncle->color == 'R'){ //if uncle is red:
				//cout << "RED UNCLE" << endl;
				//cout << "grandfather value: " << grandfather->value << endl;
				uncle->color = 'B';
				current->parent->color = 'B';
				grandfather->color = 'R';
				swapColorScheme(root, grandfather);
				
				//cout << parent->value << endl;
			}
			else if ((uncle != nullptr and uncle->color == 'B') or uncle == nullptr){ //if uncle is black (or doesnt exist):
				//cout << "BLACK UNCLE" << endl;
				char ogGrandfatherColor;
				if ((parent == grandfather->left) and (current == parent->left)){ //LL Case
					//cout << "LL" << endl;
					root = rightRotation(grandfather); //right rotation of grandfather
					ogGrandfatherColor = grandfather->color; //swap the colors of grandfather and parent
					grandfather->color = parent->color;
					parent->color = ogGrandfatherColor;
				}
				else if ((parent == grandfather->left) and (current == parent->right)){ //LR Case
					//cout << "LR" << endl;
					root = leftRotation(parent); //left rotation of parent
					root = rightRotation(grandfather); //left left case but swap current and grandfather's color
					ogGrandfatherColor = grandfather->color;
					grandfather->color = current->color;
					current->color = ogGrandfatherColor;
				}
				else if ((parent == grandfather->right) and (current == parent->right)){ //RR Case
					//cout << "RR" << endl;
					root = leftRotation(grandfather); //left rotation of grandfather
					ogGrandfatherColor = grandfather->color; //swap the colors of grandfather and parent
					grandfather->color = parent->color;
					parent->color = ogGrandfatherColor;
				}
				else if ((parent == grandfather->right) and (current == parent->left)){ //RL Case
					//cout << "RL" << endl;
					root = rightRotation(parent); //right rotation of parent
					root = leftRotation(grandfather); //right right case but swap current and grandfather's color
					ogGrandfatherColor = grandfather->color;
					grandfather->color = current->color;
					current->color = ogGrandfatherColor;
				}
			}
			else{
				cout << "PROPERTY VIOLATION" << endl;
			}
		}
	}
}

void addNode(node* NodeAdd, node* &current, node* &root){//adds a node to the root black tree
	if (current == nullptr){ //if root does not exist:
		current = NodeAdd;
		current->color = 'B'; //set root to black
	}
	else{	
		node* left = current->left;
		node* right = current->right;
		if (NodeAdd->value >= current->value){//if node to add is greater than or equal to current:
			if (right != nullptr){//if current has no vacant right position:
				addNode(NodeAdd, right, root);//recursively move to current's right node and check again for it
			}
			else{//if current has a vacant right position:
				current->right = NodeAdd;//add node to add to the tree
				NodeAdd->parent = current;
				
				swapColorScheme(root, NodeAdd);
			}
		}
		else if (NodeAdd->value < current->value){//if node to add is less current:
			if (left != nullptr){//if current has no vacant left position:
				addNode(NodeAdd, left, root);//recursively move to current's left node and check again for it
			}
			else{//if current has a vacant left position:
				current->left = NodeAdd;//add node to add to the tree;
				NodeAdd->parent = current;

				swapColorScheme(root, NodeAdd);
			}
		}
	}
}

void eraseNodeFromParent(node* current, node* replacingNode){ //erases the (parent of current)'s child, and replaces it with replacingNode
	if (current->value < current->parent->value){//current still exists though and links to its parent, just no parent links to it
		current->parent->left = replacingNode;
	}
	else{
		current->parent->right = replacingNode;
	}
}

node* getSuccessor(node* current){ //gets the successor of a node
	node* right = current->right;
	node* rightCur = right;
	while (current and rightCur->left != nullptr){
		rightCur = rightCur->left;
	}
	return rightCur;
}

void deleteRecolor(node* &root, node* parent, node* sibling, node* successor){
	if (!sibling){//if sibling doesn't exist:
		sibling = new node();
		sibling->color = 'B';
		sibling->parent = parent;
	}
	sibling->color = 'R';
	if (parent->color == 'R'){
		parent->color = 'B';
		successor->color = 'B';
	}
	else if (parent->color == 'B'){
		if (parent != root){
			parent->color = 'b';
			successor->color = 'B';
			return deleteRecolor(root, parent->parent, getSibling(parent), parent);
		}
		else{
			successor->color = 'B';
		}
	}
}

void deleteUpdate(node* deleteMe, node* successor, node* &root){
	if (!successor){ //no child case where successor = nullptr
		successor = new node();
		successor->color = 'B';
		successor->parent = deleteMe;
	}
	if (deleteMe->color == 'B' and successor->color == 'B'){//if both deleteMe and successor is black:   double black case
		successor->color = 'b'; //lowercase b as double black
		while (successor->color == 'b' and successor != root){
			node* sibling = getSibling(successor);
			if (!sibling){//if sibling doesn't exist:
				sibling = new node();
				sibling->color = 'B';
				sibling->parent = deleteMe;
			}
			if (sibling->color == 'B' and ((sibling->left and sibling->left->color == 'R') or (sibling->right and sibling->right->color == 'B'))){ //if sibling is red and at least one of sibling's children is red:
				node* redChild = getFarNephew(successor);
				if ((sibling == sibling->parent->left and redChild == sibling->left) or (sibling->left and sibling->left->color == 'R' and sibling->right and sibling->right->color == 'R')){//LL Case
					root = rightRotation(sibling->parent);
				}
				else if ((sibling == sibling->parent->left) and (redChild == sibling->right)){//LR Case
					root = leftRotation(sibling);
					root = rightRotation(successor->parent);
				}
				else if ((sibling == sibling->parent->right and redChild == sibling->right) or (sibling->left and sibling->left->color == 'R' and sibling->right and sibling->right->color == 'R')){//RR Case
					root = leftRotation(sibling->parent);
				}
				else if ((sibling == sibling->parent->right) and (redChild = sibling->left)){//RL Case
					root = rightRotation(sibling);
					root = leftRotation(successor->parent);
				}
			}
			else if (sibling->color == 'B' and ((sibling->left and sibling->left->color == 'B') or !sibling->left) and ((sibling->right and sibling->right->color == 'B') or !sibling->right)){//if sibling is black and both its children are black:
				deleteRecolor(root, successor->parent, sibling, successor);//recolors tree, moving upwards
			}
			else if (sibling->color == 'R'){
				//perform a rotation to move the sibling up
				if (sibling == sibling->parent->left){//L case
					root = rightRotation(sibling->parent);
				}
				else if (sibling == sibling->parent->right){//R case
					root = leftRotation(sibling->parent);
				}
				deleteRecolor(root, successor->parent, sibling, successor);
			}
		}
		if (successor == root){
			successor->color = 'B';
		}
	}
	else if ((deleteMe->color == 'R' and (successor or successor->color == 'B')) or (deleteMe->color == 'B' and (successor and successor->color == 'R'))){//if either deleteMe or successor is red: (but not both)   red black case
		successor->color = 'B';
	}
}

void deleteNode(node* current, int delValue, node* &root){//recursive delete function that finds the node with the value to delete and rearanges the tree after deleting
	if (current->value == delValue){//if the current node is the one to delete:
		if (current->left == nullptr and current->right == nullptr){//if current has NO CHILDREN:
			if (current != root){
				eraseNodeFromParent(current, nullptr);//sets current's parent's child to null
				cout << current->parent->value << endl;
				deleteUpdate(current, nullptr, root);
				delete current;
			}
			else{//if current is root (that means current is the only node in the tree):
				root = nullptr; //set root to null
				deleteUpdate(root, nullptr, root);
			}
		}
		else if (current->left == nullptr and current->right != nullptr){//if current has ONE CHILD (right):
			if (current != root){
				if (current->parent->left == current){
					eraseNodeFromParent(current, nullptr);
					current->parent->left = current->right;
					current->right->parent = current->parent;
				}
				else if (current->parent->right == current){
					eraseNodeFromParent(current, nullptr);
					current->parent->right = current->right;
					current->right->parent = current->parent;
				}
				deleteUpdate(current, current->right, root);
				delete current;
			}
			else{//if current is root:
				root = root->right;//sets new root
				deleteUpdate(current, current->right, root);
			}
		}
		else if (current->left != nullptr and current->right == nullptr){//if current has ONE CHILD (left):
			if (current != root){
				if (current->parent->left == current){
					eraseNodeFromParent(current, nullptr);
					current->parent->left = current->left;
					current->left->parent = current->parent;
				}
				else if (current->parent->right == current){
					eraseNodeFromParent(current, nullptr);
					current->parent->right = current->left;
					current->left->parent = current->parent;
				}
				deleteUpdate(current, root->left, root);
				delete current;
			}
			else{//if current is root:
				root = root->left;//sets new root
				deleteUpdate(current, current->left, root);
			}
		}
		else if (current->left != nullptr and current->right != nullptr){//if current has TWO CHILDREN:
			//gets successor of node to delete
			node* successor = getSuccessor(current);
			//cout << "succ: " << successor->getValue() << endl;
			eraseNodeFromParent(successor, nullptr);
			if (successor->right != nullptr){//if successor has any children (don't check left because successor is already farthest left):
				eraseNodeFromParent(successor, successor->right);//set successor's parent's child to be the right child of successor
				successor->right->parent = successor->parent;
				successor->right = nullptr;
			}
			//sets successor's parent to be current's parent (this can be null in some cases when current is the root)
			successor->parent = current->parent;
			successor->left = current->left;
			current->left->parent = successor;
			if (current->right != nullptr){
				successor->right = current->right;
				current->right->parent = successor;
			}
			else{
				successor->right = nullptr;
			}	
			

			if (current != root){
				eraseNodeFromParent(current, successor); //sets current's parent's child to successor
			}
			else{ //if current == root
				root = successor; //sets the new root to successor
			}
			delete current;
		}
	}
	else{ //if current node is not the one to delete, recursively run function until we find current;
		if (delValue < current->value){
			deleteNode(current->left, delValue, root);
		}
		else{
			deleteNode(current->right, delValue, root);
		}
	}
}

void printTree(node* current, int depth){//prints the tree in sideways format
	node* left = current->left;
	node* right = current->right;
	if (right != nullptr){
		printTree(right, depth + 1); //right recursion
	}
	for (int i = 0; i < depth; i++){ //print tabs
		cout << '\t';
	}
	if (current != nullptr){//if current exists:
		cout << current->value << " " << current->color;//print out current's value, color, and parent (if it exists)
		if (current->parent != nullptr){
			cout << " " << current->parent->value;
		}
		else{
			cout << " " << "N/A";
		}
		cout << endl;
	}
	if (left != nullptr){
		printTree(left, depth + 1); //left recursion
	}
}

int main(){
	//command variables
	string command;
	bool whileRun = true;

	node* root = nullptr;
	int addNum;
	int searchNum;
	int deleteNum;
	string readName;

	while(whileRun){
		cout << "Enter command: (ADD, READ, PRINT, SEARCH, DELETE, QUIT)";
		cin >> command;//takes in command
		if (command == "ADD"){//if you entered ADD:
			cout << "Enter add value: ";
			cin >> addNum;//takes in add input
			node* nodeToAdd = new node();
			nodeToAdd->value = addNum;
			nodeToAdd->color = 'R';
			addNode(nodeToAdd, root, root);
		}
		else if (command == "READ"){//if you entered READ (add file):
			cout << "Enter file name: (include the .txt) ";
			cin >> readName;//takes in file name

			ifstream numFile(readName);
			if (!numFile){//checks if file exists
				cout << "Invalid file name!" << endl;
			}
			else{
				int number;
				while (numFile >> number){//adds each number in the file to the tree
					cout << "Adding " << number << endl;
					node* nodeToAdd = new node();
					nodeToAdd->value = number;
					addNode(nodeToAdd, root, root);
				}
				numFile.close();
				cout << "Added numbers!" << endl;
			}
		}
		else if (command == "PRINT"){//if you entered PRINT:
			if (root != nullptr){
				printTree(root, 0);
			}
			else{
				cout << "Tree is empty!" << endl;
			}
		}
		else if (command == "SEARCH"){//if you entered SEARCH:
			cout << "Enter value to search for: ";//get user input on what number to search for
			cin >> searchNum;
			if (searchTree(root, searchNum)){//print whether or not its in the tree
				cout << searchNum << " is in the tree!" << endl;
			}
			else{
				cout << searchNum << " is not in the tree!" << endl;
			}
		}
		else if (command == "DELETE"){//if you entered DELETE:
			cout << "Enter value to delete: ";//gets user input on what number to delete
			cin >> deleteNum;
			if (searchTree(root, deleteNum)){//first checks if deleteNum exists:
				deleteNode(root, deleteNum, root);	
			}
			else{
				cout << "That number is not in the tree!" << endl;
			}
		}
		else if (command == "QUIT"){//if you entered QUIT:
			whileRun = false;//set running while loop to false
			cout << "Quitted program." << endl;
		}
		else{ //if you entered none of the above:
			cout << "Unrecognized input, please try again." << endl;
		}
	}
	return 0;
}
