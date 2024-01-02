#include "binary_tree.hpp"

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

int compare_keys(void* key_l, void* key_r, std::vector<int>& key_bytes) {
    int byte_offset = 0;
    for (const int& bytes : key_bytes) {
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

Binary_Tree::Binary_Tree(std::vector<int> key_attribute_offsets, std::vector<int> key_attribute_lengths){
    this->key_attribute_offsets = key_attribute_offsets;
    this->key_attribute_lengths = key_attribute_lengths;
    this->root_node = nullptr;
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

int Table::insert(void* data) {
    int key_byte_length = std::accumulate(this->key_bytes.begin(), this->key_bytes.end(), 0);
    void* key = malloc(sizeof(char) * key_byte_length);
    int key_offset_bytes = 0;
    for (int i = 0; i < this->key_schema_offsets.size(); ++i) {
        memcpy(((char*)key + key_offset_bytes), ((char*)data + this->key_schema_offsets.at(i)), this->key_bytes.at(i));
        key_byte_length += this->key_bytes.at(i);
    }

    return this->btree->insert(key, data);
}


