#include "binary_tree.hpp"

#include <cstdlib>
#include <cstring>
#include <numeric>

enum DIRECTIONS { LEFT = -1, RIGHT = 1 };

int get_byte_size(TABLE_DATATYPE type) {
    switch (type) {
        case INT:
            return 4;
        case LONG:
            return 8;
        case CHAR_8:
            return 8;
        case CHAR_16:
            return 16;
        case CHAR_32:
            return 32;
        case UNIX_TIMESTAMP:
            return 4;
        case DATE:
            return 4;
    }
}

int compare_keys(void* key_l, void* key_r, std::vector<int>& key_attribute_lengths) {
    int byte_offset = 0;
    for (const int& bytes : key_attribute_lengths) {
        if (bytes <= 8) {
            long left;
            long right;
            if (bytes == 4) {
                left = *((int*)((char*)key_l + byte_offset));
                right = *((int*)((char*)key_r + byte_offset));
            } else if (bytes == 8) {
                left = *((long*)((char*)key_l + byte_offset));
                right = *((long*)((char*)key_r + byte_offset));
            }
            if (left > right) {
                return -1;
            } else if (left < right) {
                return 1;
            } else {
                return 0;
            }
        } else {
            char* left_begin = (char*)key_l + byte_offset;
            char* right_begin = (char*)key_r + byte_offset;
            for (int i = 0; i < bytes; ++i) {
                if (left_begin[i] == right_begin[i]) continue;
                if (left_begin[i] > right_begin[i]) return -1;
                if (left_begin[i] < right_begin[i]) return 1;
            }
            return 0;
        }
        byte_offset += bytes;
    }
    return 0;
}

Binary_Tree::Binary_Tree(std::vector<int> key_attribute_offsets, std::vector<int> key_attribute_lengths) {
    this->key_attribute_offsets = key_attribute_offsets;
    this->key_attribute_lengths = key_attribute_lengths;
    this->root_node = nullptr;
}

void update_parent_balance(BT_Node* bt_node) {
    if (bt_node->parent->left == bt_node) {
        --bt_node->parent->balance;
    } else {
        ++bt_node->parent->balance;
    }
}

void Binary_Tree::rebalance(BT_Node* bt_node) {
    BT_Node* current_node = bt_node;
    while (current_node->parent != nullptr) {
        update_parent_balance(current_node);
        if (current_node->parent->balance == -1 || current_node->parent->balance == 1) {
            current_node = current_node->parent;
        } else if (current_node->parent->balance == 0)
            return;
        else if (current_node->parent->balance == -2 || current_node->parent->balance == 2) {
            // rebalance parent
            
            return;
        }
    }
}

int Binary_Tree::insert(void* data) {
    void* key = this->compute_key(data);

    if (this->root_node == nullptr) {
        this->root_node = new BT_Node{key, data, 0, nullptr, nullptr, nullptr};
        return 0;
    }

    BT_Node* bt_node = this->root_node;

    while (true) {
        int compare_result = compare_keys(bt_node->key, key, this->key_attribute_lengths);
        if (compare_result == -1) {
            if (bt_node->left == nullptr) {
                bt_node->left = new BT_Node{key, data, 0, bt_node, nullptr, nullptr};
                this->rebalance(bt_node->left);
                return 0;
            } else {
                bt_node = bt_node->left;
            }
        } else if (compare_result == 1) {
            if (bt_node->right == nullptr) {
                bt_node->right = new BT_Node{key, data, 0, bt_node, nullptr, nullptr};
                this->rebalance();
                return 0;
            } else {
                bt_node = bt_node->right;
            }
        } else {
            return -1;
        }
    }
}

void* Binary_Tree::search(void* key) { return this->search_for_key(key)->data; }

BT_Node* Binary_Tree::search_for_key(void* key) {
    if (this->root_node == nullptr) {
        return nullptr;
    }

    BT_Node* bt_node = this->root_node;

    while (true) {
        int compare_result = compare_keys(bt_node->key, key, this->key_attribute_lengths);
        if (compare_result == -1) {
            if (bt_node->left == nullptr) {
                return bt_node;
            } else {
                bt_node = bt_node->left;
            }
        } else if (compare_result == 1) {
            if (bt_node->right == nullptr) {
                return bt_node;
            } else {
                bt_node = bt_node->right;
            }
        } else {
            return bt_node;
        }
    }
}

bool has_no_children(BT_Node* node) { return node->left == nullptr && node->right == nullptr; }

bool has_two_children(BT_Node* node) { return node->left != nullptr && node->right != nullptr; }

BT_Node* Binary_Tree::find_inorder_successor(BT_Node* node) {
    BT_Node* bt_node = node->right;
    while (bt_node->left != nullptr) {
        bt_node = bt_node->left;
    }

    return bt_node;
}

void replace_in_parent(BT_Node* bt_node, BT_Node* bt_replace) {
    if (bt_node->parent->left == bt_node) {
        bt_node->parent->left = bt_replace;
    } else if (bt_node->parent->right == bt_node) {
        bt_node->parent->right = bt_replace;
    }
}

int Binary_Tree::remove(void* key) {
    BT_Node* bt_node = this->search_for_key(key);
    if (bt_node == nullptr) return -1;
    if (has_no_children(bt_node)) {
        if (bt_node->parent == nullptr) {
            this->root_node = nullptr;
        } else {
            replace_in_parent(bt_node, nullptr);
        }
    } else if (has_two_children(bt_node)) {
        BT_Node* inorder_successor = this->find_inorder_successor(bt_node);
        if (bt_node->parent == nullptr) {
            this->root_node = inorder_successor;
            replace_in_parent(inorder_successor, nullptr);
        } else {
            replace_in_parent(bt_node, inorder_successor);
            replace_in_parent(inorder_successor, nullptr);
        }
    } else {
        BT_Node* child;
        if (bt_node->left != nullptr) {
            child = bt_node->left;
        } else {
            child = bt_node->right;
        }
        if (bt_node->parent == nullptr) {
            this->root_node = child;
        } else {
            replace_in_parent(bt_node, child);
        }
    }
    return 0;
}

void* Binary_Tree::compute_key(void* data) {
    int key_byte_length = std::accumulate(this->key_attribute_lengths.begin(), this->key_attribute_lengths.end(), 0);
    void* key = malloc(sizeof(char) * key_byte_length);
    int key_offset_bytes = 0;
    for (int i = 0; i < this->key_attribute_offsets.size(); ++i) {
        memcpy(((char*)key + key_offset_bytes), ((char*)data + this->key_attribute_offsets.at(i)), this->key_attribute_lengths.at(i));
        key_byte_length += this->key_attribute_lengths.at(i);
    }

    return key;
}

Table::Table(std::vector<TABLE_DATATYPE> schema, std::vector<bool> key_attributes) {
    this->schema = schema;
    this->compute_key_data(key_attributes);
    this->binary_tree = new Binary_Tree(this->key_attribute_offsets, this->key_attribute_lengths);
}

void Table::compute_key_data(std::vector<bool> key_attributes) {
    int offset_bytes = 0;
    for (int i = 0; i < this->schema.size() && i < key_attributes.size(); ++i) {
        if (key_attributes.at(i)) {
            this->key_attribute_offsets.emplace_back(offset_bytes);
            this->key_attribute_lengths.emplace_back(get_byte_size(this->schema.at(i)));
        }
        offset_bytes += get_byte_size(this->schema.at(i));
    }
}

int Table::insert(void* data) { return this->binary_tree->insert(data); }
void* Table::search(void* key) { return this->binary_tree->search(key); }
int Table::remove(void* key) { return this->binary_tree->remove(key); }
