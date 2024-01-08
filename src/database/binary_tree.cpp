#include "binary_tree.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <numeric>
#include <stack>

#include "db_util.hpp"

// void print_binary_tree_node(BT_Node* node, std::vector<int>& key_attribute_lengths);
// void print_binary_tree(Binary_Tree* binary_tree, std::vector<int>& key_attribute_lengths);

bool has_no_children(BT_Node* node) { return node->left == nullptr && node->right == nullptr; }

bool has_two_children(BT_Node* node) { return node->left != nullptr && node->right != nullptr; }

void replace_in_parent(BT_Node* bt_node, BT_Node* bt_replace, BT_Node** root_node) {
    if (bt_node->parent == nullptr) {
        *root_node = bt_replace;
        bt_replace->parent = nullptr;
    } else {
        if (bt_node->parent->left == bt_node) {
            bt_node->parent->left = bt_replace;
        } else if (bt_node->parent->right == bt_node) {
            bt_node->parent->right = bt_replace;
        }
        if (bt_replace != nullptr) {
            bt_replace->parent = bt_node->parent;
            bt_node->parent = nullptr;
        }
    }
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

bool apply_right_left_rotation(BT_Node* node) { return node->right->left != nullptr; }

bool apply_left_right_rotation(BT_Node* node) { return node->left->right != nullptr; }

void Binary_Tree::rebalance(BT_Node* bt_node) {
    BT_Node* current_node = bt_node;
    while (current_node->parent != nullptr) {
        update_parent_balance(current_node);
        if (current_node->parent->balance == -1 || current_node->parent->balance == 1) {
            current_node = current_node->parent;
        } else if (current_node->parent->balance == 0) {
            return;
        } else if (current_node->parent->balance == -2 || current_node->parent->balance == 2) {
            BT_Node* imbalanced_node = current_node->parent;
            if (imbalanced_node->balance == 2) {  // right-heavy
                // if(imbalanced_node->right == nullptr){
                //     print_binary_tree_node(imbalanced_node, this->key_attribute_lengths);
                //     print_binary_tree(this, this->key_attribute_lengths);
                // }
                if (has_two_children(imbalanced_node->right)) {
                    BT_Node* temp = imbalanced_node->right->left;
                    imbalanced_node->right->left = imbalanced_node;
                    replace_in_parent(imbalanced_node, imbalanced_node->right, &this->root_node);
                    imbalanced_node->parent = imbalanced_node->right;
                    imbalanced_node->right = temp;
                    imbalanced_node->parent->balance = 0;
                    imbalanced_node->balance = 0;
                } else {
                    if (apply_right_left_rotation(imbalanced_node)) {
                        imbalanced_node->right->left->right = imbalanced_node->right;
                        imbalanced_node->right->left->left = imbalanced_node;

                        replace_in_parent(imbalanced_node, imbalanced_node->right->left, &this->root_node);

                        imbalanced_node->parent = imbalanced_node->right->left;
                        imbalanced_node->right = nullptr;

                        imbalanced_node->parent->right->left = nullptr;
                        imbalanced_node->parent->right->parent = imbalanced_node->parent;
                    } else {  // left rotation
                        imbalanced_node->right->left = imbalanced_node;
                        replace_in_parent(imbalanced_node, imbalanced_node->right, &this->root_node);
                        imbalanced_node->parent = imbalanced_node->right;
                        imbalanced_node->right = nullptr;
                    }
                    imbalanced_node->parent->balance = 0;
                    imbalanced_node->balance = 0;
                }

            } else if (imbalanced_node->balance == -2) {  // left-heavy
                if (has_two_children(imbalanced_node->left)) {
                    BT_Node* temp = imbalanced_node->left->right;
                    imbalanced_node->left->right = imbalanced_node;
                    replace_in_parent(imbalanced_node, imbalanced_node->left, &this->root_node);
                    imbalanced_node->parent = imbalanced_node->left;
                    imbalanced_node->left = temp;
                    imbalanced_node->parent->balance = 0;
                    imbalanced_node->balance = 0;
                } else {
                    if (apply_left_right_rotation(imbalanced_node)) {
                        imbalanced_node->left->right->left = imbalanced_node->left;
                        imbalanced_node->left->right->right = imbalanced_node;

                        replace_in_parent(imbalanced_node, imbalanced_node->left->right, &this->root_node);

                        imbalanced_node->parent = imbalanced_node->left->right;
                        imbalanced_node->left = nullptr;

                        imbalanced_node->parent->left->right = nullptr;
                        imbalanced_node->parent->left->parent = imbalanced_node->parent;
                    } else {  // right rotation
                        imbalanced_node->left->right = imbalanced_node;
                        replace_in_parent(imbalanced_node, imbalanced_node->left, &this->root_node);
                        imbalanced_node->parent = imbalanced_node->left;
                        imbalanced_node->left = nullptr;
                    }
                    imbalanced_node->parent->balance = 0;
                    imbalanced_node->balance = 0;
                }
            }

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
        if (compare_result == 1) {
            if (bt_node->right == nullptr) {
                bt_node->right = new BT_Node{key, data, 0, bt_node, nullptr, nullptr};
                this->rebalance(bt_node->right);
                return 0;
            } else {
                bt_node = bt_node->right;
            }
        } else if (compare_result == -1) {
            if (bt_node->left == nullptr) {
                bt_node->left = new BT_Node{key, data, 0, bt_node, nullptr, nullptr};
                this->rebalance(bt_node->left);
                return 0;
            } else {
                bt_node = bt_node->left;
            }
        } else {
            return -1;
        }
    }
}

void* Binary_Tree::search(void* key) {
    BT_Node* ptr = this->search_for_key(key);
    return ptr == nullptr ? nullptr : this->search_for_key(key)->data;
}

std::vector<void*> Binary_Tree::search_between_keys(void* key_left, void* key_right) {
    if (this->root_node == nullptr) {
        return {};
    }

    BT_Node* leftmost_in_range = this->root_node;
    BT_Node* current_node_l = this->root_node;

    while (true) {
        int compare_result = compare_keys(current_node_l->key, key_left, this->key_attribute_lengths);
        if (compare_result == -1) {
            if (current_node_l->left == nullptr) {
                leftmost_in_range = current_node_l;
                break;
            } else {
                leftmost_in_range = current_node_l;
                current_node_l = current_node_l->left;
            }
        } else if (compare_result == 1) {
            if (current_node_l->right == nullptr) {
                break;
            } else {
                current_node_l = current_node_l->right;
            }
        } else {
            break;
        }
    }

    BT_Node* rightmost_in_range = this->root_node;
    BT_Node* current_node_r = this->root_node;

    while (true) {
        int compare_result = compare_keys(current_node_r->key, key_right, this->key_attribute_lengths);
        if (compare_result == -1) {
            if (current_node_r->left == nullptr) {
                break;
            } else {
                current_node_r = current_node_r->left;
            }
        } else if (compare_result == 1) {
            if (current_node_r->right == nullptr) {
                rightmost_in_range = current_node_r;
                break;
            } else {
                rightmost_in_range = current_node_r;
                current_node_r = current_node_r->right;
            }
        } else {
            break;
        }
    }

    std::cout << *((int*)leftmost_in_range->key) << "-" << *((int*)leftmost_in_range->key + 1) << "|\n";
    std::cout << *((int*)rightmost_in_range->key) << "-" << *((int*)rightmost_in_range->key + 1) << "|\n";

    if (leftmost_in_range == rightmost_in_range) {
        int left_compare = compare_keys(key_left, leftmost_in_range->key, this->key_attribute_lengths);
        int right_compare = compare_keys(key_right, rightmost_in_range->key, this->key_attribute_lengths);
        if ((left_compare == 1 || left_compare == 0) && (right_compare == -1 || right_compare == 0)) {
            return {leftmost_in_range->data};
        }
    }

    std::vector<void*> data_ptrs;
    BT_Node* current = leftmost_in_range;
    while (true) {
        data_ptrs.emplace_back(current->data);
        if (current == rightmost_in_range) return data_ptrs;

        if (current->right != nullptr) {
            // traverse right subtree until rightmost found (or not)
            std::stack<BT_Node*> s;
            BT_Node* node = current->right;
            while (!s.empty() || node != nullptr) {
                if (node != nullptr) {
                    s.push(node);
                    node = node->left;
                } else {
                    node = s.top();
                    s.pop();
                    data_ptrs.emplace_back(node->data);
                    if (node == rightmost_in_range) return data_ptrs;
                    node = node->right;
                }
            }
        }

        // find next node
        BT_Node* previous_node = current;
        current = current->parent;
        while (current->right == previous_node) {
            previous_node = current;
            current = current->parent;
        }
    }
}

BT_Node* Binary_Tree::search_for_key(void* key) {
    if (this->root_node == nullptr) {
        return nullptr;
    }

    BT_Node* bt_node = this->root_node;

    while (true) {
        int compare_result = compare_keys(bt_node->key, key, this->key_attribute_lengths);
        if (compare_result == -1) {
            if (bt_node->left == nullptr) {
                return nullptr;
            } else {
                bt_node = bt_node->left;
            }
        } else if (compare_result == 1) {
            if (bt_node->right == nullptr) {
                return nullptr;
            } else {
                bt_node = bt_node->right;
            }
        } else {
            return bt_node;
        }
    }
}

BT_Node* Binary_Tree::find_inorder_successor(BT_Node* node) {
    BT_Node* bt_node = node->right;
    while (bt_node->left != nullptr) {
        bt_node = bt_node->left;
    }

    return bt_node;
}

int Binary_Tree::remove(void* key) {
    BT_Node* bt_node = this->search_for_key(key);
    if (bt_node == nullptr) return -1;
    if (has_no_children(bt_node)) {
        if (bt_node->parent == nullptr) {
            this->root_node = nullptr;
        } else {
            replace_in_parent(bt_node, nullptr, &this->root_node);
        }
    } else if (has_two_children(bt_node)) {
        BT_Node* inorder_successor = this->find_inorder_successor(bt_node);
        if (bt_node->parent == nullptr) {
            this->root_node = inorder_successor;
            replace_in_parent(inorder_successor, nullptr, &this->root_node);
        } else {
            replace_in_parent(bt_node, inorder_successor, &this->root_node);
            replace_in_parent(inorder_successor, nullptr, &this->root_node);
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
            replace_in_parent(bt_node, child, &this->root_node);
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
        key_offset_bytes += this->key_attribute_lengths.at(i);
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
