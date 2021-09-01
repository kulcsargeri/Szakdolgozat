#include <iostream>
#include "tree.h"

int main(){
	Tree<int, std::string> test(4);
	test.insert(7,"első");
	test.insert(9,"második");
	test.insert(11,"harmadik");
	test.insert(15,"negyedik");
	test.insert(8,"ötödik");
	test.insert(12,"hatodik");
	test.insert(17,"hetedik");
	test.insert(6,"nyolcadik");
	test.insert(10,"kilencedik");
	test.insert(16,"tizedik");
	test.insert(1,"tizenegyedik");
	std::cout<<test.search(17)<<std::endl;
	std::cout<<test.search(6)<<std::endl;
	std::cout<<test.search(7)<<std::endl;
	std::cout<<test.search(16)<<std::endl;
	return 0;
}
