#include "db_util.hpp"

#include "structs.hpp"

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