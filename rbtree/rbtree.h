#ifndef RBTREE_H_
#define RBTREE_H_

#include <list>

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
    RBTree() : root_(&nil_) {}
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

            insert_fixup(z);
        }
         
    } 

    ~RBTree() {
        free_node(root_);
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
        return (1 + 1 << (max_depth - depth + 1));
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
                    printf("%d", node->key()); 
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
        y->set_left(x);
        if (is_left(x)) {
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
        x->set_right(y);
        if (is_left(y)) {
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
            right_rotate(p->parent());
            p->parent()->set_color(COLOR::COLOR_RED); 
            p->set_color(COLOR::COLOR_BLACK);
        }
        if (is_right(z) && is_right(p)) {
            /* case 3 */
            left_rotate(p->parent());
            p->parent()->set_color(COLOR::COLOR_RED);
            p->set_color(COLOR::COLOR_BLACK);
        }
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
    static Node nil_;
};

Node RBTree::nil_;
#endif
