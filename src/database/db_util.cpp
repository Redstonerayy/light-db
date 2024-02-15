#include "db_util.hpp"

#include <string>
#include <vector>

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
    int key_parts = key_attribute_lengths.size();
    int key_part = 0;
    int byte_offset = 0;
    for (const int& bytes : key_attribute_lengths) {
        ++key_part;
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
                if (key_part >= key_parts) return 0;
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

std::vector<std::string> split_string(std::string s, std::string delimiter) {
    size_t pos = 0;
    std::string token;
    std::vector<std::string> parts;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        parts.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    parts.push_back(s);
    return parts;
}
