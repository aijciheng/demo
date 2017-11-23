#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#include <iostream>

#include "rbtree.h"

long get_current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#define N 1000000
#define DN 50000

int main() {
    srand(time(NULL));
    int *num = new int[N];
    int *dnum = new int[DN];
    RBTree rbtree;

    for (int i = 0; i < N; i++) {
        num[i] = (int)rand();
        if (rbtree.add(PairInfo(num[i], i)) == 1) {
            std::cout << "重复了" << std::endl;
            i--;
            continue;
        }
    }

    if (rbtree.check_tree()) {
        std::cout << "父节点测试通过" << std::endl;
    } else { 
        std::cout << "未通过测试" << std::endl;
    }

    /* test add */
    for (int i = 0; i < N; i++) {
        int num_value = rbtree.get(num[i]);
        //std::cout << num_value << std::endl;
        if (num_value != i) {
            std::cout << "查询无此数据，测试失败!" << std::endl;
            return -1;
        }
    }

    std::cout << "添加测试通过" << std::endl;

    /* test remove */
    for (int i = 0; i < DN; i++) {
        int dn = rand() % N;
        int num_value = num[dn];
        if (num_value == -1) {
            i--;
            continue;
        }
        dnum[i] = num_value;
        num[dn] = -1;
        rbtree.remove(num_value);
    }

    for (int i = 0; i < N; i++) {
        if (num[i] != -1) {
            int num_value = rbtree.get(num[i]);
            if (num_value != i) {
                std::cout << "查询无此数据, 测试失败!" << std::endl;
                return -1;
            }
        }
    }
    for (int i = 0; i < DN; i++) {
        if (rbtree.get(dnum[i]) != -1) {
            std::cout << "没有删除, 测试失败!" << std::endl;
            return -1;
        }
    }
    std::cout << "删除测试通过" << std::endl;

    delete[] num;
    delete[] dnum;
    return 0;
}
