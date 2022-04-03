#include <iostream>
#include "tree.h"

int main(){
	Tree<int, std::string> test(4);
	test.insert(7,"elso");
	test.insert(9,"masodik");
	test.insert(11,"harmadik");
	test.insert(15,"negyedik");
	test.structure();
	test.insert(8,"otodik");
	test.structure();
	test.insert(12,"hatodik");
	test.insert(17,"hetedik");
	test.insert(6,"nyolcadik");
	test.structure();
	test.insert(10,"kilencedik");
	test.structure();
	test.insert(16,"tizedik");
	test.structure();
	std::cout<<test.search(17)<<std::endl;
	std::cout<<test.search(6)<<std::endl;
	std::cout<<test.search(7)<<std::endl;
	//test.remove(6);
	//test.structure();
	test.remove(17);
	test.structure();
	test.remove(12);
	test.structure();
	//std::cout<<test.search(17)<<std::endl;
	return 0;
}

