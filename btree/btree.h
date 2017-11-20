#ifdef BTREE_H_
#define BTREE_H_

class PairInfo {
 public:
    
    PairInfo(int key, int value)
        : key_(key),
          value_(value) {}
    

    int key() {return key_;}
    int value() {return value_;}

 prviate:
    int key_;
    int value_;
};

class KeyNode {
 public:
    KeyNode(int key, int value)
        : pair_info_(key, value),
          childs_(NULL),
          parent_(NULL) {}

    KeyNode *next() {return next_;}
    void set_next(KeyNode *next) {
        if (next) next->set_next(next_);
        if (next_) next_->set_pre(next_);
        next_ = next;
        if (next) next->set_pre(this);
    }

    KeyNode *pre() {return pre_;}
    void set_pre(KeyNode *pre) {
        if (pre_) pre_->set_next(pre);
        if (pre) pre->set_pre(pre_);
        if (pre) pre->set_next(this);
        pre_ = pre;
    }

    Node *left_child() {return left_child_;}
    void set_left_child(Node *left_child) {left_child_ = left_child;}

    Node *right_child() {return right_child_;}
    void set_right_child(Node *right_child) {right_child_ = right_child;}

    int key() {return pair_info_.key();}
    int value() {return pair_info_.value();}

 private:
    KeyNode *next_;
    KeyNode *pre_;
    Node *left_child_;
    Node *right_child_;
    PairInfo pair_info_;
};

class Node {
 public:
    Node(KeyNode *header)
        : header_(header),
          key_num_(1) {}

    int key_num() {return key_num_;}

    KeyNode *get(int index) {
        KeyNode *node = header_;
        int cur_index = 0;
        while (node && cur_index < index) {
            node = node->next();
            cur_index++;
        }

        if (cur_index == index) {
            return node;
        }
        return NULL;
    }

    void put(KeyNode *keyNode) {
        KeyNode *cur_node = header_;
        while (cur_node) {
            cur_node = cur_node->next();
        }
    }

 private:
    KeyNode *header_;
    int key_num_;
};

class BTree {
 public:
    BTree(int m)
        : root_(NULL),
          m_(m) {}

    void put(PairInfo pair_info) {
        // root null
        if (!root_) {
            KeyNode *key_node = new KeyNode(pair_info.key(), pair_info.value());
            root_ = new Node(key_node);
            return;
        }

        // 检测是否key达到2m-1
        if (root_->key_num() >= (2 * m_ - 1)) {
            KeyNode *key_node = root_->get(m_); 
            if (!key_node) {
                fprint(stderr, "Find %d is error\n", m_);
                return;
            }
            Node *new_root = new Node(key_node);
            KeyNode *next_node = key_node->next();
            Node *new_right_node = new Node(next_node);
            key_node->pre()->set_next(NULL);
            next_node->set_pre(NULL);
            key_node->set_left_child(root_);
            key_node->set_right_child(new_right_node);
            pre_root_ = root_;
            root_ = new_root;
            delete pre_root_;
            return;
        }
    
        // 未达到2m-1

    }

 private:
    int m_;
    Node *root_;
};

#endif
