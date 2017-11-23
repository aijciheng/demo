#ifndef RBTREE_H_
#define RBTREE_H_

#include <list>

#define NONE                 "\e[0m"
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define GREEN                "\e[0;32m"
#define L_GREEN              "\e[1;32m"
#define BROWN                "\e[0;33m"
#define YELLOW               "\e[1;33m"
#define BLUE                 "\e[0;34m"
#define L_BLUE               "\e[1;34m"
#define PURPLE               "\e[0;35m"
#define L_PURPLE             "\e[1;35m"
#define CYAN                 "\e[0;36m"
#define L_CYAN               "\e[1;36m"
#define GRAY                 "\e[0;37m"
#define WHITE                "\e[1;37m"

#define BOLD                 "\e[1m"
#define UNDERLINE            "\e[4m"
#define BLINK                "\e[5m"
#define REVERSE              "\e[7m"
#define HIDE                 "\e[8m"
#define CLEAR                "\e[2J"
#define CLRLINE              "\r\e[K" //or "\e[1K\r"

class PairInfo {
 public:
    PairInfo(int key, int value) : key_(key), value_(value) {}

    int key() {return key_;}
    int value() {return value_;}
 private:
    int key_;
    int value_;
};

class Node {
 public:
    enum COLOR {
        COLOR_RED,
        COLOR_BLACK
    };
    Node()
        : color_(COLOR::COLOR_BLACK),
          pair_info_(PairInfo(-1, -1)),
          left_(NULL),
          right_(NULL),
          parent_(NULL) {}
    Node(COLOR color, PairInfo pair_info, Node *left, Node *right, Node *parent)
        : color_(color), 
          pair_info_(pair_info),
          left_(left),
          right_(right),
          parent_(parent) {}

    COLOR color() { return color_;}
    void set_color(COLOR color) { color_ = color; }
    int key() { return pair_info_.key(); }
    int value() { return pair_info_.value(); }

    Node *left() {return left_;}
    void set_left(Node *node) { left_ = node; }
    Node *right() {return right_;}
    void set_right(Node *node) { right_ = node; }
    Node *parent() {return parent_;}
    void set_parent(Node *node) { parent_ = node; }

 private:
    COLOR color_;
    PairInfo pair_info_;
    Node *left_;
    Node *right_;
    Node *parent_;
};

class RBTree {
 public:
    typedef Node::COLOR COLOR;
    RBTree() : nil_(), root_(&nil_) {}
    ~RBTree() {
        free_node(root_);
    }

    int add(PairInfo pair_info) {
        Node *parent_node = NULL;
        Node *node = root_;
        /* search right position */
        while (node != &nil_) {
            parent_node = node;
            if (node->key() > pair_info.key()) {
                node = node->left();
            } else if (node->key() < pair_info.key()) {
                node = node->right();
            } else {
                /* exist */
                return 1;
            }
        }

        /* add node */
        if (parent_node == NULL) {
            /* root */
            root_ = new Node(COLOR::COLOR_BLACK, pair_info, &nil_, &nil_, &nil_);
            return 0;
        } else {
            Node *z = new Node(COLOR::COLOR_RED, pair_info, &nil_, &nil_, parent_node);
            if (parent_node->key() > pair_info.key()) {
                parent_node->set_left(z);
            } else {
                parent_node->set_right(z);
            }
            /* insert fixup */
            print_info();

            insert_fixup(z);
        }
        return 0; 
    } 

    int remove(int key) {
        Node *node = root_;
        while (node != &nil_) {
            if (node->key() > key) {
                node = node->left();
            } else if (node->key() < key) {
                node = node->right();
            } else {
                /* found */
                break;
            }
        }

        if (node == &nil_) {
            /* not found */
            return -1;
        }

        delete_node(node);
        delete node;
        return 0;
    }

    int get(int key) {
        Node *node = root_;
        while (node != &nil_) {
            if (node->key() > key) {
                node = node->left();
            } else if (node->key() < key) {
                node = node->right();
            } else {
                return node->value();
            }
        }
        return -1;
    }
        
    int depth() {
        return depth(root_, 0);
    }

    int depth(Node *node, int d) {
        if (node == &nil_) {
            return d;
        }

        int left_depth = depth(node->left(), d + 1);
        int right_depth = depth(node->right(), d + 1);
        return left_depth > right_depth ? left_depth : right_depth;
    }

    int get_blank_count(int depth, int max_depth) {
        return (1 << (max_depth - depth + 1)) - 1;
    }

    int char_count(int depth, int max_depth) {
        int num_count = 1 << (depth - 1); 
        int blank_block_count = num_count - 1;
        int blank_count = get_blank_count(depth, max_depth);
        return num_count + blank_block_count * blank_count;
    }

    void print_info() {
        if (root_ != &nil_) {
            std::list<Node*> list;   
            list.push_back(root_);
            int tree_depth = depth();
            printf("tree depth : %d\n", tree_depth);
            int exist_count = 1;
            int count = 0;
            int cur_depth = 1;
            int blank_count = -1;
            while (exist_count > 0) {
                Node *node = list.front();
                list.pop_front();
                count++;
                if (count == (1 << (cur_depth - 1))) {
                    printf("\n");
                    int real_print_blank = (char_count(tree_depth, tree_depth) - char_count(cur_depth, tree_depth)) / 2;
                    blank_count = get_blank_count(cur_depth, tree_depth);
                    for (int i = 0; i < real_print_blank; i++) 
                        printf(" ");
                    cur_depth++;
                } else {
                    for (int i = 0; i < blank_count; i++) {
                        printf(" ");
                    }
                }
               
                if (node->key() != -1) {
                    if (node->color() == COLOR::COLOR_RED) {
                        printf(RED);
                    }
                    printf("%d", node->key()); 
                    if (node->color() == COLOR::COLOR_RED) {
                        printf(NONE);
                    }
                    exist_count--;
                } else {
                    printf("n");
                }
            

                if (node == &nil_ || node->left() == &nil_) {
                    list.push_back(&nil_);
                } else {
                    list.push_back(node->left());
                    exist_count++;
                }

                if (node == &nil_ || node->right() == &nil_) {
                    list.push_back(&nil_);
                } else {
                    list.push_back(node->right());
                    exist_count++;
                }

            }
            printf("\n");
        }
    }
    
    bool check_tree() {
        Node *node = &nil_;
        std::list<Node*> list;
        list.push_back(root_);
        while (list.size() > 0) {
            node = list.front();
            list.pop_front();
            if (node->left() != &nil_) {
                if (node->left()->parent() != node) {
                    return false;
                }
                list.push_back(node->left());
            }
                
            if (node->right() != &nil_) {
                if (node->right()->parent() != node) {
                    return false;
                }
                list.push_back(node->right());
            }
        }
        return true;
    }

 private:
    Node *sibling(Node *p) {
        if (p->parent()->left() == p) {
            return p->parent()->right();
        }
        return p->parent()->left();
    }

    bool is_left(Node *p) {
        if (p->parent()->left() == p) {
            return true;
        }
        return false;
    }

    bool is_right(Node *p) {
        return !(is_left(p));
    }

    void left_rotate(Node *x) {
        /** left rotate
         *      x           y
         *    a   y ----> x  c
         *       b c     a b
         */
        Node *y = x->right(); 
        x->set_right(y->left());
        if (y->left() != &nil_) {
            y->left()->set_parent(x);
        }
        y->set_left(x);
        if (x->parent() == &nil_) {
            /* root_ */
            root_ = y;
        } else if (is_left(x)) {
            x->parent()->set_left(y);
        } else {
            x->parent()->set_right(y);
        }
        y->set_parent(x->parent());
        x->set_parent(y);
    }

    void right_rotate(Node *y) {
        /** right rotate
         *      x           y
         *    a   y <---- x  c
         *       b c     a b
         */
        Node *x = y->left();
        y->set_left(x->right());
        if (x->right() != &nil_) {
            x->right()->set_parent(y);
        }
        x->set_right(y);
        if (y->parent() == &nil_) {
            /* root */
            root_ = x;
        } else if (is_left(y)) {
            y->parent()->set_left(x);
        } else {
            y->parent()->set_right(x);
        }
        x->set_parent(y->parent()); 
        y->set_parent(x);
    }

    void insert_fixup(Node *z) {
        if (z == &nil_) {
            return;
        }
        if (z == root_) {
            z->set_color(COLOR::COLOR_BLACK);
        }
        Node *p = z->parent();
        if (p == &nil_) {
            return;
        }
        if (p->color() == COLOR::COLOR_BLACK) {
            return;
        }
        Node *g = p->parent();
        Node *s = sibling(p);
        if (s->color() == COLOR::COLOR_RED) {
            /* case 1 */
            g->set_color(COLOR::COLOR_RED);
            p->set_color(COLOR::COLOR_BLACK);
            s->set_color(COLOR::COLOR_BLACK);
            return insert_fixup(g);
        }

        if (is_right(z) && is_left(p)) {
            /* case 2 */
            left_rotate(p);
            z = p;
            p = z->parent();
        }
        if (is_left(z) && is_right(p)) {
            /* case 2 */
            right_rotate(p);
            z = p;
            p = z->parent();
        }

        if (is_left(z) && is_left(p)) {
            /* case 3 */
            if (p->parent() != &nil_) {
                p->parent()->set_color(COLOR::COLOR_RED); 
            }
            if (p != &nil_) {
                p->set_color(COLOR::COLOR_BLACK);
            }
            right_rotate(p->parent());
        }
        if (is_right(z) && is_right(p)) {
            /* case 3 */
            if (p->parent() != &nil_) {
                p->parent()->set_color(COLOR::COLOR_RED);
            }
            if (p != &nil_) {
                p->set_color(COLOR::COLOR_BLACK);
            }
            left_rotate(p->parent());
        }
    }

    void delete_node(Node *z) {
        Node *y = z; 
        Node::COLOR orign_y_color = y->color();
        Node *x = NULL;
        if (z->left() == &nil_) {
            x = z->right(); 
            exchange(y, x);
        } else if (z->right() == &nil_) {
            x = z->left();
            exchange(y, x);
        } else {
            y = min(z->right());
            orign_y_color = y->color();
            x = y->right();
            if (y->parent() == z) {
                x->set_parent(y); // may be x is nil 
            } else {
                exchange(y, x);
                y->set_right(z->right());
                z->right()->set_parent(y);
            }
            exchange(z, y);
            y->set_left(z->left());    
            z->left()->set_parent(y); 

            y->set_color(z->color());
        }

        if (orign_y_color == COLOR::COLOR_BLACK) {
            delete_fixup(x);
        }
    }

    void delete_fixup(Node *node) {
        while (node != root_ && node->color() == COLOR::COLOR_BLACK) {
            if (node->parent()->left() == node) {
                /* left */
                Node *w = node->parent()->right();
                if (w->color() == COLOR::COLOR_RED) {
                    /* case 1, change case 1 to case 2, 3 or 4 */
                    node->parent()->set_color(COLOR::COLOR_RED);
                    w->set_color(COLOR::COLOR_BLACK);
                    left_rotate(node->parent());
                    w = node->parent()->right();
                }

                if (w->left()->color() == COLOR::COLOR_BLACK && w->right()->color() == COLOR::COLOR_BLACK){
                    /* case 2, only can iterator case, it may change case 1, 3, 4.
                       but if case 1 to case 2, and then will end while soon. */
                    w->set_color(COLOR::COLOR_RED);  
                    node = node->parent();
                } else {
                    if (w->right()->color() == COLOR::COLOR_BLACK) {
                        /* case 3, change case 4 */
                        w->left()->set_color(COLOR::COLOR_BLACK);
                        w->set_color(COLOR::COLOR_RED);
                        right_rotate(w);
                        w = node->parent()->right();
                    }
                    /* case 4 */
                    w->set_color(node->parent()->color());
                    node->parent()->set_color(COLOR::COLOR_BLACK);
                    w->right()->set_color(COLOR::COLOR_BLACK);
                    left_rotate(node->parent());
                    node = root_;
                }
            } else {
                /* right */
                Node *w = node->parent()->left();
                if (w->color() == COLOR::COLOR_RED) {
                    /* case 1, change case 1 to case 2, 3 or 4 */
                    node->parent()->set_color(COLOR::COLOR_RED);
                    w->set_color(COLOR::COLOR_BLACK);
                    right_rotate(node->parent());
                    w = node->parent()->left();
                }

                if (w->left()->color() == COLOR::COLOR_BLACK && w->right()->color() == COLOR::COLOR_BLACK) {
                    /* case 2, only can iterator case, it my change case 1, 3, 4.
                       but if case 1 to case 2, and then will end while soon. */
                    w->set_color(COLOR::COLOR_RED);
                    node = node->parent();
                } else {
                    if (w->left()->color() == COLOR::COLOR_BLACK) {
                        /* case 3, change case 4 */
                        w->right()->set_color(COLOR::COLOR_BLACK);
                        w->set_color(COLOR::COLOR_RED);
                        left_rotate(w);
                        w = node->parent()->left();
                    }
                    /* case 4 */
                    w->set_color(node->parent()->color());
                    node->parent()->set_color(COLOR::COLOR_BLACK);
                    w->left()->set_color(COLOR::COLOR_BLACK);
                    right_rotate(node->parent());
                    node = root_;
                }
            }
        }

        node->set_color(COLOR::COLOR_BLACK);
    }

    void exchange(Node *z, Node *y) {
        if (z->parent() == &nil_) {
            root_ = y;
        } else if(z->parent()->left() == z) {
            z->parent()->set_left(y); 
        } else {
            z->parent()->set_right(y);
        } 
        y->set_parent(z->parent());
    }

    Node *min(Node *node) {
        while (node->left() != &nil_) {
            node = node->left();
        }
        return node;
    }

    void free_node(Node *node) {
        if (node && node != &nil_) {
            free_node(node->left());
            free_node(node->right());
        }
        
        if (node != &nil_) {
            delete node;
        }
    }

    Node *root_;
    Node nil_;
};

#endif
