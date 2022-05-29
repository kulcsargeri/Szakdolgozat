#include <iostream>
#include "tree.h"

void displaymenu(){ 	
	std::cout<<"===================================================== \n";
	std::cout<<" \t\tMENU \t \n ";
	std::cout<<"===================================================== \n";
	std::cout<<" 0. Exit\n";
	std::cout<<" 1. Create a new tree\n";
	std::cout<<" 2. Add data\n";
	std::cout<<" 3. Remove data\n";
	std::cout<<" 4. Search data\n";
	std::cout<<" 5. Print tree to file\n";
	std::cout<<" 6. Change tree parameter\n";
}

void CreateNewTree(int children_count, Tree<int, std::string>* tree){
	int print_count = tree->number_of_prints_;
	delete tree;
	Tree<int, std::string>* newtree = new Tree<int, std::string>(children_count, print_count);
	tree = newtree;
	tree->number_of_prints_ = print_count;
}

void AddData(int key, std::string value, Tree<int, std::string>* tree){
	tree->insert(key, value);
}

void RemoveData(int key, Tree<int, std::string>* tree){
	tree->remove(key);
}

void SearchData(int key, Tree<int, std::string>* tree){
	std::cout<<tree->search(key)<<"\n";
}

void PrintTree(Tree<int, std::string>* tree){
	tree->structure();
}

void ChangeTree(int children_count, Tree<int, std::string>* tree){
	tree->ChangeChildrenCount(children_count);
}

int main(){
	std::cout<<"You are given a tree with int, string parameters and children count is 4\n";
	Tree<int, std::string>* tree = new Tree<int, std::string>(4);
	int your_choice;
	displaymenu();
	do
	{
		std::cout<<"Enter your choice(0-6):\n";
		std::cin>>your_choice;
		int children_count;
		int key;
		std::string value;
		switch (your_choice)
		{
			case 1:
				std::cout<<"Enter children count for the new tree\n";
				std::cin>>children_count;
				CreateNewTree(children_count, tree);
				break;
			case 2:
				std::cout<<"Enter key, value you want to insert in tree\n";
				std::cin>>key;
				std::cin>>value;
				AddData(key, value, tree);
				break;
			case 3:
				std::cout<<"Enter the key of the data you want to remove from tree\n";
				std::cin>>key;
				RemoveData(key, tree);
				break;
			case 4:
				std::cout<<"Enter the key of the data you are searching for in the tree\n";
				std::cin>>key;
				SearchData(key, tree);
				break;
			case 5:
				PrintTree(tree);
				break;
			case 6:
				std::cout<<"Enter the new children count for changing the tree\n";
				std::cin>>children_count;
				ChangeTree(children_count, tree);
				break;
			case 0:
				break;
			default: 
				std::cout<<"invalid"; 
				break;
		}
	} while (your_choice != 0);
	return 0;
}