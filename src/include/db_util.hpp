#pragma once

#include "structs.hpp"

int get_byte_size(TABLE_DATATYPE type);
int compare_keys(void* key_l, void* key_r, std::vector<int>& key_attribute_lengths);
