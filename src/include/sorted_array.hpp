#pragma once

#include <vector>

#include "structs.hpp"

struct Search_Result {
    int status;
    int index;
};

struct SA_ELEMENT {
    void* key;
    void* data;
};

class Sorted_Array {
   public:
    std::vector<SA_ELEMENT> elements;

    std::vector<int> key_attribute_offsets;
    std::vector<int> key_attribute_lengths;

    Sorted_Array(std::vector<int> key_attribute_offsets, std::vector<int> key_attribute_lengths);
    int insert(void* data);
    void* search(void* key);
    std::vector<void*> search_between_keys(void* key_left, void* key_right);
    int remove(void* key);

   private:
    void* compute_key(void* data);
    Search_Result search_for_key(void* key);
};

class Table {
   public:
    Sorted_Array* sorted_array;

    std::vector<int> key_attribute_offsets;
    std::vector<int> key_attribute_lengths;
    std::vector<TABLE_DATATYPE> schema;

    Table(std::vector<TABLE_DATATYPE> schema, std::vector<bool> key_attributes);
    int insert(void* data);
    void* search(void* key);
    int remove(void* key);

   private:
    void compute_key_data(std::vector<bool> key_attributes);
};
