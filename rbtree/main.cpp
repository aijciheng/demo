#include <iostream>

#include "rbtree.h"

int main(int argc, char **argv) {
    RBTree rbtree;
    for (int i = 0; i < 100; i++) {
        rbtree.add(PairInfo(i, i + 1));
    }
    
    for (int i = 0; i < 100; i++) {
        int value = rbtree.get(i);
        std::cout << value << std::endl;
    }
    return 0;
}
