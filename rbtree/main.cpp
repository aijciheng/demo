#include <iostream>

#include "rbtree.h"

int main(int argc, char **argv) {
    RBTree rbtree;
    for (int i = 0; i < 10; i++) {
        std::cout << "==============================" << std::endl;
        std::cout << "add " << i << std::endl;
        rbtree.add(PairInfo(i, i + 1));
        rbtree.print_info();
        if (rbtree.check_tree()) {
            std::cout << "通过测试" << std::endl;
        } else {
            std::cout << "未通过测试" << std::endl;
            return -1;
        }
    }


    std::cout << "==============================" << std::endl;
    std::cout << "remove 4" << std::endl;
    rbtree.remove(4); 
    rbtree.print_info();
    if (rbtree.check_tree()) {
        std::cout << "通过测试" << std::endl;
    } else {
        std::cout << "未通过测试" << std::endl;
        return -1;
    }
    

    std::cout << "==============================" << std::endl;
    std::cout << "remove 1" << std::endl;
    rbtree.remove(1);
    rbtree.print_info();
    if (rbtree.check_tree()) {
        std::cout << "通过测试" << std::endl;
    } else {
        std::cout << "未通过测试" << std::endl;
        return -1;
    }

    std::cout << "==============================" << std::endl;
    std::cout << "remove 3" << std::endl;
    rbtree.remove(3);
    rbtree.print_info();
    if (rbtree.check_tree()) {
        std::cout << "通过测试" << std::endl;
    } else {
        std::cout << "未通过测试" << std::endl;
        return -1;
    }
    return 0;
}
