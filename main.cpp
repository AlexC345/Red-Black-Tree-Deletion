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

node* searchTree(node* current, int searchValue){ //recursive function that checks if a value is in the tree
	if (current->value == searchValue){//if current's value is equal to searchValue:
		return current; //return true (found value)
	}
	else{//if current's value is not equal to searchValue, recursively search down the tree based off whether or not we need to go down left or down right
		if (searchValue < current->value){ //search down left
			if (current->left != nullptr){
				return searchTree(current->left, searchValue);
			}
			else{
				return nullptr;
			}
		}
		else{ //search down right
			if (current->right != nullptr){
				return searchTree(current->right, searchValue);
			}
			else{
				return nullptr;
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

node* leftRotation(node* current){
        if (current == nullptr or current->right == nullptr){
                return getRoot(current);
        }

        node* oldParent = current->parent;
        node* Cright = current->right;
        node* rightLeft = Cright->left;
	//relink cRight's parent
        Cright->parent = oldParent;
	//relink old parent's children
        if (oldParent != nullptr){
                if (current == oldParent->left){
                        oldParent->left = Cright;
                }
                else if (current == oldParent->right){
                        oldParent->right = Cright;
                }
        }
	//relink cRIght's children
        Cright->left = current;
	//relink current's parent
        current->parent = Cright;
	//relink current's right
        current->right = rightLeft;
	//relink rightLeft's parent
        if (rightLeft != nullptr){
                rightLeft->parent = current;
        }

        return getRoot(Cright);
}

node* rightRotation(node* current){
        if (current == nullptr || current->left == nullptr){
                return getRoot(current);
        }

        node* oldParent = current->parent;
        node* Cleft = current->left;
        node* leftRight = Cleft->right;
	//relink cLeft's parent
        Cleft->parent = oldParent;
	//relink old parent's children
        if (oldParent != nullptr){
                if (current == oldParent->left){
                        oldParent->left = Cleft;
                }
                else if (current == oldParent->right){
                        oldParent->right = Cleft;
                }
        }
	//relink cLeft's parent
        Cleft->right = current;
	//relink current's parent
        current->parent = Cleft;
	//relink current's left
        current->left = leftRight;
	//relink leftRight's parent
        if (leftRight != nullptr){
                leftRight->parent = current;
        }

        return getRoot(Cleft);
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
	if (current == current->parent->left){
		current->parent->left = replacingNode;
	}
	else if (current == current->parent->right){
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

void deleteUpdate(node* deleteMe, node* successor, node* &root){
	if (deleteMe->color == 'R' or successor->color == 'R'){//case 1: simple case
		cout << "case 1" << endl;
		successor->color = 'B';
	}
	else if (deleteMe->color == 'B' and successor->color == 'B'){//case 2
		cout << "case 2" << endl;
		successor->color = 'b';
		while (successor->color == 'b' and successor != root){
			node* sibling = getSibling(successor);
			if (!sibling){
				successor->color = 'B';
				successor = successor->parent;
				if (successor->color == 'R'){
					successor->color = 'B';
				}
				else{
					successor->color = 'b';
				}
			}
			else if (sibling->color == 'B'){
				if (sibling and ((sibling->left and sibling->left->color == 'R') or (sibling->right and sibling->right->color == 'R'))){//case 3
					cout << "case 3" << endl;
					node* redChild = getFarNephew(successor);
					if (redChild == nullptr or redChild->color == 'B'){
						redChild = getNearNephew(successor);
					}
					bool siblingIsLeftChild = (sibling == sibling->parent->left);
					bool nephewIsLeftChild = (sibling->left == redChild);
					bool bothSiblingRedChildren = (sibling->left and sibling->left->color == 'R' and sibling->right and sibling->right->color == 'R');
					node* parent = sibling->parent;
					if ((siblingIsLeftChild == nephewIsLeftChild) or bothSiblingRedChildren){//LL case and RR case
						redChild->color = sibling->color;
						sibling->color = parent->color;
						if (siblingIsLeftChild and nephewIsLeftChild){//LL case
							root = rightRotation(parent);
							if (sibling->left){
								sibling->left->color = 'B';
							}
						}
						else{//RR case
							root = leftRotation(parent);
							if (sibling->right){
								sibling->right->color = 'B';
							}
						}
						break;
					}
					else{//LR case and RL case
						redChild->color = parent->color;
						parent->color = 'B';
						sibling->color = 'B';
						if (siblingIsLeftChild and !nephewIsLeftChild){//LR case
							root = leftRotation(sibling);
							root = rightRotation(parent);
						}
						else if (!siblingIsLeftChild and nephewIsLeftChild){//RL case
							root = rightRotation(sibling);
							root = leftRotation(parent);
						}
						break;
					}
				}
				else if (sibling->color == 'B' and (!sibling->left or sibling->left->color == 'B') and (!sibling->right or sibling->right->color == 'B')){//case 4
					cout << "case 4" << endl;
					sibling->color = 'R';
					if (successor->parent->color == 'B'){//move double black up the tree
						successor->parent->color = 'b';
						successor->color = 'B';
						successor = successor->parent;
					}
					else{//until it has found a red parent to get rid of double black
						successor->parent->color = 'B';
						successor->color = 'B';
					}
				}
			}
			else if (sibling->color == 'R'){//case 5
				cout << "case 5" << endl;
				sibling->parent->color = 'R';
				sibling->color = 'B';
				if (sibling == sibling->parent->left){//left case
					root = rightRotation(sibling->parent);
				}
				else{//right case
					root = leftRotation(sibling->parent);
				}
				
			}
			else if (successor == root){//case 6
				cout << "case 6" << endl;
				successor->color = 'B';
			}
		}
		if (successor->color == 'b'){
			successor->color = 'B';
		}
	}
}

void deleteNode(node* current, node* &root){//delete function to delete and rearange the tree after deleting a node
	if (current->left == nullptr and current->right == nullptr){//if current has NO CHILDREN:
		if (current != root){
			node* parent = current->parent;

			node* successor = new node();
			successor->color = 'B';
			successor->parent = parent;
			
			eraseNodeFromParent(current, successor);
			deleteUpdate(current, successor, root);

			if (successor->parent != nullptr){
				eraseNodeFromParent(successor, nullptr);
			}
			else if (root == successor){
				root = nullptr;
			}

			delete successor;
			delete current;

		}
		else{//if current is root (that means current is the only node in the tree):
			root = nullptr; //set root to null
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
			deleteUpdate(current, current->right, root);
			root = root->right;//sets new root
			//deleteUpdate(current, current->right, root);
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
			deleteUpdate(current, current->left, root);
			delete current;
		}
		else{//if current is root:
			deleteUpdate(current, current->left, root);
			root = root->left;//sets new root
			//deleteUpdate(current, current->left, root);
		}
	}
	else if (current->left != nullptr and current->right != nullptr){//if current has TWO CHILDREN:
		//gets successor of node to delete
		node* successor = getSuccessor(current);
		current->value = successor->value;
		
		deleteNode(successor, root);
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
				deleteNode(searchTree(root, deleteNum), root);	
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
