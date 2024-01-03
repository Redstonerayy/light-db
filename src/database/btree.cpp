// #include "btree.hpp"

// #include <cstdlib>
// #include <cstring>
// #include <iostream>
// #include <numeric>
// #include <vector>

// int get_byte_size(TABLE_DATATYPE type) {
//     switch (type) {
//         case INT:
//             return 4;
//         case LONG:
//             return 8;
//         case CHAR_8:
//             return 8;
//         case CHAR_16:
//             return 16;
//         case CHAR_32:
//             return 32;
//         case UNIX_TIMESTAMP:
//             return 4;
//         case DATE:
//             return 4;
//     }
// }

// int compare_keys(void* key_l, void* key_r, std::vector<int>& key_bytes) {
//     int byte_offset = 0;
//     for (const int& bytes : key_bytes) {
//         if (bytes <= 8) {
//             long left;
//             long right;
//             if (bytes == 4) {
//                 left = *((int*)((char*)key_l + byte_offset));
//                 right = *((int*)((char*)key_r + byte_offset));
//             } else if (bytes == 8) {
//                 left = *((long*)((char*)key_l + byte_offset));
//                 right = *((long*)((char*)key_r + byte_offset));
//             }
//             if (left > right) {
//                 return -1;
//             } else if (left < right) {
//                 return 1;
//             } else {
//                 return 0;
//             }
//         } else {
//             char* left_begin = (char*)key_l + byte_offset;
//             char* right_begin = (char*)key_r + byte_offset;
//             for (int i = 0; i < bytes; ++i) {
//                 if (left_begin[i] == right_begin[i]) continue;
//                 if (left_begin[i] > right_begin[i]) return -1;
//                 if (left_begin[i] < right_begin[i]) return 1;
//             }
//             return 0;
//         }
//         byte_offset += bytes;
//     }
//     return 0;
// }

// Page::Page(int k_factor, std::vector<int> key_bytes, Page* parent_page, BTree* btree) {
//     this->k_factor = k_factor;
//     this->key_bytes = key_bytes;
//     this->parent_page = parent_page;
//     this->btree = btree;
// }

// int insertion_index_search(void* key, std::vector<Key>& keys, std::vector<int>& key_bytes) {
//     if (keys.size() == 0) return 0;

//     int l = 0;
//     int r = keys.size() - 1;
//     while (l <= r) {
//         int m = l + (r - l) / 2;
//         int comparison_result = compare_keys(keys.at(m).key, key, key_bytes);
//         if (comparison_result == 1) {
//             l = m + 1;
//         } else if (comparison_result == -1) {
//             r = m - 1;
//         } else {
//             return -1;  // no equal keys
//         }
//     }

//     return l;
// }

// int page_overflow(Page* page, int child_index, int k_factor) {
//     Page* overfull_page = page->page_ptrs.at(child_index);
//     if (child_index > 0 && page->page_ptrs.at(child_index - 1)->keys.size() < 2 * k_factor) {
//         Page* adjacent_page = page->page_ptrs.at(child_index - 1);
//         adjacent_page->keys.insert(adjacent_page->keys.end(), page->keys.at(child_index - 1));
//         adjacent_page->keys.insert(adjacent_page->keys.end(), overfull_page->keys.begin(), overfull_page->keys.end());
//         adjacent_page->page_ptrs.insert(adjacent_page->page_ptrs.end(), overfull_page->page_ptrs.begin(), overfull_page->page_ptrs.end());
//         int m = adjacent_page->keys.size() / 2;
//         overfull_page->keys = std::vector<Key>(adjacent_page->keys.begin() + m + 1, adjacent_page->keys.end());
//         overfull_page->page_ptrs = std::vector<Page*>(adjacent_page->page_ptrs.begin() + m + 2, adjacent_page->page_ptrs.end());
//         page->keys.at(child_index - 1) = adjacent_page->keys.at(m);

//     } else if (page->page_ptrs.size() > 1 && page->page_ptrs.at(child_index + 1)->keys.size() < 2 * k_factor) {
//         Page* adjacent_page = page->page_ptrs.at(child_index + 1);
//     } else {
//         return -1;
//     }
// }

// int page_split(Page* page, int child_index) { Page* overfull_page = page->page_ptrs.at(child_index); }

// int Page::insert_key(void* key, void* data_pointer) {
//     int index = insertion_index_search(key, this->keys, this->key_bytes);
//     if (index == -1) return -1;
//     if (index < this->page_ptrs.size()) {
//         int insertion_status = this->page_ptrs.at(index)->insert_key(key, data_pointer);
//         if (insertion_status == -1) return -1;
//         if (insertion_status == 1) {
//             int status = page_overflow(this, index, this->k_factor);
//             if (status == -1) page_split(this, index);
//             if (this->keys.size() > 2 * this->k_factor) return 1;
//         }
//     } else {
//         this->keys.insert(this->keys.begin() + index, Key{key, data_pointer});
//         if (this->keys.size() > 2 * this->k_factor) return 1;
//     }
// }

// int Page::read_key(void* key) {}
// int Page::update_key(void* key, void* data_pointer) {}
// int Page::delete_key(void* key) {}

// BTree::BTree(int k_factor, std::vector<int> key_bytes) {
//     this->k_factor = k_factor;
//     this->key_bytes = key_bytes;
//     this->height = 0;
//     this->root_page = new Page(k_factor, this->key_bytes, nullptr, this);
// }

// int BTree::insert(void* key, void* data_pointer) { return this->root_page->insert_key(key, data_pointer); }

// Table::Table(std::vector<TABLE_DATATYPE> schema, std::vector<bool> key_attributes) {
//     this->schema = schema;
//     this->compute_key_data(key_attributes);
//     this->btree = new BTree(2, this->key_bytes);
// }

// int Table::insert(void* data) {
//     int key_byte_length = std::accumulate(this->key_bytes.begin(), this->key_bytes.end(), 0);
//     void* key = malloc(sizeof(char) * key_byte_length);
//     int key_offset_bytes = 0;
//     for (int i = 0; i < this->key_schema_offsets.size(); ++i) {
//         memcpy(((char*)key + key_offset_bytes), ((char*)data + this->key_schema_offsets.at(i)), this->key_bytes.at(i));
//         key_byte_length += this->key_bytes.at(i);
//     }

//     return this->btree->insert(key, data);
// }

// void Table::compute_key_data(std::vector<bool> key_attributes) {
//     int offset_bytes = 0;
//     for (int i = 0; i < this->schema.size() && i < key_attributes.size(); ++i) {
//         if (key_attributes.at(i)) {
//             key_schema_offsets.emplace_back(offset_bytes);
//             key_bytes.emplace_back(get_byte_size(this->schema.at(i)));
//         }
//         offset_bytes += get_byte_size(this->schema.at(i));
//     }
// }
