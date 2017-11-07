#include <iostream>

#include "rbtree.h"

int main(int argc, char **argv) {
    RBTree rbtree;
    for (int i = 0; i < 5; i++) {
        rbtree.add(PairInfo(i, i + 1));
    }

    rbtree.print_info();

    //int max_char_count = rbtree.char_count(5, 5);
    //for (int i = 1; i <= 5; i++) {
    //    printf("%d\n", (max_char_count - rbtree.char_count(i, 5)) / 2);
    //}
    
    //for (int i = 0; i < 100; i++) {
    //    int value = rbtree.get(i);
    //    std::cout << value << std::endl;
    //}
    return 0;
}
