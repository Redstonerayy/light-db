#pragma once

#include <vector>

struct Key {
    void* key;
    void* data_ptr;
};

class Page {
   public:
    Page* parent_page;
    BTree* btree;
    std::vector<int>& key_bytes;

    std::vector<Key> keys;
    std::vector<Page*> page_ptrs;

    Page(int k_factor, std::vector<int>& key_bytes, Page* parent_page, BTree* btree);
    int insert_key(void* key, void* data_pointer);
    int read_key(void* key);
    int update_key(void* key, void* data_pointer);
    int delete_key(void* key);

   private:
    int k_factor;
};

class BTree {
   public:
    int height;
    Page* root_page;
    std::vector<int> key_bytes;

    BTree(int k_factor, std::vector<int> key_bytes);
    int insert(void* key, void* data_pointer);

   private:
    int k_factor;
};

enum TABLE_DATATYPE {
    INT,             // 32 bit integer
    LONG,            // 64 bit integer
    CHAR_8,          // 8 bytes
    CHAR_16,         // 16 bytes
    CHAR_32,         // 32 bytes
    UNIX_TIMESTAMP,  // 32 bit integer
    DATE,            // 32 bit integer
};

class Table {
   public:
    BTree* btree;
    std::vector<int> key_bytes;
    std::vector<int> key_schema_offsets;
    std::vector<TABLE_DATATYPE> schema;

    Table(std::vector<TABLE_DATATYPE> schema, std::vector<bool> key_attributes);
    int insert(void* data);

   private:
    void compute_key_data(std::vector<bool> key_attributes);
};
