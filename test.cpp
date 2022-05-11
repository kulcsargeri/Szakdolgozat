#include <iostream>
#include "tree.h"

int main(){
	Tree<int, std::string> test(4);
	test.insert(785,"elso");
	test.insert(912,"masodik");
	test.insert(1107,"harmadik");
	test.insert(1595,"negyedik");
	test.insert(826,"otodik");
	test.insert(1274,"hatodik");
	test.insert(1751,"hetedik");
	test.insert(682,"nyolcadik");
	test.insert(1009,"kilencedik");
	test.insert(1667,"tizedik");
    //test.structure();

    //AddWithoutSplit()
    test.insert(1483,"11edik");
    //test.structure();
    
    //CreateNewRoot()
    test.insert(1378,"hatodik");
    //test.structure();

    test.insert(275, "13adik");

    //SplitToLastHalf()
    test.insert(803, "14edik");
    //test.structure();
	
    test.insert(1320, "15odik");

    //SplitToFirstHalf()
    test.insert(1162, "16odik");
    //test.structure();

    //AskYoungerSiblingForChildren()
    test.remove(1320);
    //test.structure();

    //MergeWithYoungerSibling()
    test.remove(1009);
    //test.structure();

    //MergeWithOlderSibling()
    test.remove(1483);
    //test.structure();

    //AfterRemoveAboveLeafs()
    test.remove(785);
    //test.structure();

    //AskOlderSiblingForChildren()
    test.remove(275);
    //test.structure();

    //Search key not in tree
    std::cout<<test.search(275)<<"\n";

    //Search key in tree
    std::cout<<test.search(1667)<<"\n";

    test.ChangeChildrenCount(5);
    //test.structure();

    //SplitToFirstHalf()
    test.insert(572, "17edik");
    //test.structure();

    //MergeWithYoungerSibling()
    test.remove(1274);
    //test.structure();

    //SplitToLastHalf()
    test.insert(1140, "18adik");
    //test.structure();

    //MergeWithOlderSibling()
    test.remove(572);
    //test.structure();

    test.remove(1595);
    test.remove(1162);
    test.remove(1751);
    
    //AskYoungerSiblingForChildren()
    test.remove(1140);
    //test.structure();

    test.remove(1107);

    //AddWithoutSplit()
    test.insert(1163, "19edik");
    //test.structure();

    //AskOlderSiblingForChildren()
    test.remove(803);
    //test.structure();

    test.remove(1378);

    //CreateNewRoot()
    test.insert(2594, "20adik");
    //test.structure();

    // std::cout<<test.search(17)<<std::endl;
	// std::cout<<test.search(6)<<std::endl;
	// std::cout<<test.search(7)<<std::endl;
	// test.remove(6);
	// test.structure();
	// test.remove(15);
	// test.structure();
	// test.remove(12);
	// test.structure();
	// test.remove(8);
	// test.structure();
	// std::cout<<test.search(17)<<std::endl;
	return 0;
}