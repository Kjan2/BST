#include <iostream>
#include "BST.h"
int main()
{

    BinarySearchTree<> tree;
    tree.insert(15, 3);
    tree.insert(6, 3);
    tree.insert(3, 3);
    tree.insert(2, 3);
    tree.insert(4, 3);
    tree.insert(7, 3);
    tree.insert(13, 3);
    tree.insert(9, 3);
    tree.insert(18, 4);
    tree.insert(17, 4);
    tree.insert(20, 4);
    tree.insert(3, 4);

    tree.print(tree._root);
    
    //tree.deleteNode(tree._root);
    BinarySearchTree<>::Iterator it(tree._root);
    std::cout << tree.size() << std::endl;
    BinarySearchTree<>::Iterator it2(nullptr);

    tree.print(tree._root);

    for(auto& i : tree)
    {
        std::cout << (i).first << " " << (i).second << std::endl;
    } 
    
    tree.equalRange(0);

    return 0;
}
