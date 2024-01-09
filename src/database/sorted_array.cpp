#include "sorted_array.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <numeric>
#include <stack>

#include "db_util.hpp"

Sorted_Array::Sorted_Array(std::vector<int> key_attribute_offsets, std::vector<int> key_attribute_lengths) {
    this->key_attribute_offsets = key_attribute_offsets;
    this->key_attribute_lengths = key_attribute_lengths;
}

int Sorted_Array::insert(void* data) {
    void* key = this->compute_key(data);

    if (this->elements.size() == 0) {
        this->elements.emplace_back(SA_ELEMENT{key, data});
        return 0;
    }

    Search_Result res = this->search_for_key(key);
    if (res.status == 1) {
        return -1;
    } else {
        SA_ELEMENT new_el = SA_ELEMENT{key, data};
        if (compare_keys(this->elements.at(res.index).key, key, this->key_attribute_lengths) == 1) {
            this->elements.insert(this->elements.begin() + res.index + 1, new_el);
        } else {
            this->elements.insert(this->elements.begin() + res.index, new_el);
        }
        return 0;
    }
}

void* Sorted_Array::search(void* key) {
    Search_Result res = this->search_for_key(key);
    if (res.status == 1) {
        return this->elements.at(res.index).data;
    }

    return nullptr;
}

std::vector<void*> Sorted_Array::search_between_keys(void* key_left, void* key_right) {
    Search_Result res_l = this->search_for_key(key_left);
    Search_Result res_r = this->search_for_key(key_right);

    int left_start = res_l.index;
    int right_start = res_r.index;

    if (res_l.status == 0 && compare_keys(this->elements.at(res_l.index).key, key_left, this->key_attribute_lengths) == 1) ++left_start;
    if (res_r.status == 0 && compare_keys(this->elements.at(res_r.index).key, key_right, this->key_attribute_lengths) == -1) --right_start;

    std::vector<void*> data_ptrs;
    for(int i = left_start; i < right_start + 1; ++i){
        data_ptrs.emplace_back(this->elements.at(i).data);
    }
    return data_ptrs;
}

Search_Result Sorted_Array::search_for_key(void* key) {
    int l = 0;
    int r = this->elements.size() - 1;
    int m;

    while (l <= r) {
        m = l + ((r - l) / 2);
        int compare_result = compare_keys(this->elements.at(m).key, key, this->key_attribute_lengths);
        if (compare_result == -1) {
            r = m - 1;
        } else if (compare_result == 1) {
            l = m + 1;
        } else {
            return Search_Result{1, m};
        }
    }

    return Search_Result{0, m};
}

int Sorted_Array::remove(void* key) {
    Search_Result res = this->search_for_key(key);

    if (res.status == 1) {
        this->elements.erase(this->elements.begin() + res.index);
    }
    return 0;
}

void* Sorted_Array::compute_key(void* data) {
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
    this->sorted_array = new Sorted_Array(this->key_attribute_offsets, this->key_attribute_lengths);
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

int Table::insert(void* data) { return this->sorted_array->insert(data); }
void* Table::search(void* key) { return this->sorted_array->search(key); }
int Table::remove(void* key) { return this->sorted_array->remove(key); }
