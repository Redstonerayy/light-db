#pragma once

#include <vector>

class Page;

struct Page_Entry {
    void* key;
    void* data_pointer;
    Page* child_page_pointer;
};

class Page {
   public:
    Page* parent_page;
    Page* first_child_page;
    std::vector<Page_Entry> entries;

    Page(int k_factor, Page* parent_page);
    int insert(void* key, std::vector<int>& key_bytes, void* data_pointer);

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
    std::vector<int> key_schema_offsets;
    std::vector<int> key_bytes;
    std::vector<TABLE_DATATYPE> schema;

    Table(std::vector<TABLE_DATATYPE> schema, std::vector<bool> key_attributes);
    void insert(void* data);

   private:
    void compute_key_data(std::vector<bool> key_attributes);
};
