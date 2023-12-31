#include "btree.hpp"

#include <cstdlib>
#include <cstring>
#include <numeric>
#include <vector>

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

Page::Page(int k_factor, Page* parent_page) {
    this->k_factor = k_factor;
    this->parent_page = parent_page;
    this->first_child_page = nullptr;
    this->entries.reserve(2 * k_factor);
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
}

int Page::insert(void* key, std::vector<int>& key_bytes, void* data_pointer) {
    if (this->entries.size() < 2 * this->k_factor) {
        if (this->entries.size() == 0) {
            this->entries.emplace_back(Page_Entry{key, data_pointer, nullptr});
            return 0;
        }

        int l = 0;
        int r = this->entries.size() - 1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            int comparison_result = compare_keys(this->entries.at(m).key, key, key_bytes);
            if (comparison_result == -1) {
                l = m + 1;
            } else if (comparison_result == 1) {
                r = m - 1;
            } else {
                return -1;  // no equal keys
            }
        }
        this->entries.insert(this->entries.begin() + l, Page_Entry{key, data_pointer, nullptr});
    } else {
        // split nodes usw.
        return 0;
    }
    return 0;
}

BTree::BTree(int k_factor, std::vector<int> key_bytes) {
    this->k_factor = k_factor;
    this->key_bytes = key_bytes;
    this->height = 0;
    this->root_page = new Page(k_factor, nullptr);
}

int BTree::insert(void* key, void* data_pointer) { return this->root_page->insert(key, this->key_bytes, data_pointer); }

Table::Table(std::vector<TABLE_DATATYPE> schema, std::vector<bool> key_attributes) {
    this->schema = schema;
    this->compute_key_data(key_attributes);
    this->btree = new BTree(32, this->key_bytes);
}

void Table::insert(void* data) {
    int key_byte_length = std::accumulate(this->key_bytes.begin(), this->key_bytes.end(), 0);
    void* key = malloc(sizeof(char) * key_byte_length);
    int key_offset_bytes = 0;
    for (int i = 0; i < this->key_schema_offsets.size(); ++i) {
        memcpy(
            ((char*)key + key_offset_bytes),
            ((char*)data + this->key_schema_offsets.at(i)),
            this->key_bytes.at(i)
        );
        key_byte_length += this->key_bytes.at(i);
    }

    this->btree->insert(key, data);
}

void Table::compute_key_data(std::vector<bool> key_attributes) {
    int offset_bytes = 0;
    for (int i = 0; i < this->schema.size() && i < key_attributes.size(); ++i) {
        if (key_attributes.at(i)) {
            key_schema_offsets.emplace_back(offset_bytes);
            key_bytes.emplace_back(get_byte_size(this->schema.at(i)));
        }
        offset_bytes += get_byte_size(this->schema.at(i));
    }
}
