#include "fs_b_tree.hpp"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "db_util.hpp"

/*
// B-Tree file structure on disk
<1 to n Bytes> key type by id
<0 Byte>
<1 to n Bytes> data types by id
<0 Byte>
<1 Byte> page size
<1 to n Pages>
*/

/*
// Page structure on disk
<4 Bytes> size
<4 Bytes> fill
<8 Bytes> number of parent page (-1 for root)
<Page data>
    <8 Bytes> page number (-1 for uninitialized)
    <n Bytes> key
    <n Bytes> record to previous key
    ...
    <8 Bytes> page position
    zeros for everything not filled
*/

void exit_m(std::string s) {
    std::cout << s;
    exit(EXIT_FAILURE);
}

int id_to_byte_size(int id) {
    switch (id) {
        case 1:
            return 4;
        case 2:
            return 8;
        case 3:
            return 8;
        case 4:
            return 16;
        case 5:
            return 32;
        case 6:
            return 4;
        case 7:
            return 4;
        default:
            return 0;
    }
}

void b_tree_open(BTree& btree, std::string filepath) {
    btree.filepath = filepath;

    if (!std::filesystem::exists(filepath)) {
        std::ofstream of(filepath);
        of.close();
    }

    btree.fs = std::fstream(filepath, std::ios::in | std::ios::out | std::ios::binary);
    std::fstream* fs = &btree.fs;

    char data[] = {1, 0, 1, 0, 4};
    fs->write(data, sizeof(char) * 5);

    fs->seekg(0);

    char buf;
    std::vector<int> key_ids = {};
    while (true) {
        fs->read(&buf, 1);
        if (fs->eof()) exit_m("Cant read key ids\n");
        if (buf == 0) break;
        key_ids.emplace_back(buf);
    }

    std::vector<int> data_ids = {};
    while (true) {
        fs->read(&buf, 1);
        if (fs->eof()) exit_m("Can't read data ids\n");
        if (buf == 0) break;
        data_ids.emplace_back(buf);
    }

    fs->read(&buf, 1);
    if (fs->eof()) exit_m("Can't read page size (k)\n");
    if (buf < 2) exit_m("Page size to small");
    int page_k = buf;

    int page_size = 4 + 4 + 8;
    int key_size = 0;
    for (const int& id : key_ids) {
        key_size += id_to_byte_size(id);
    }
    int data_size = 0;
    for (const int& id : data_ids) {
        data_size += id_to_byte_size(id);
    }
    int entry_size = key_size + data_size;
    page_size += entry_size * page_k + 8 * (page_k + 1);

    int page_starting_offset = fs->tellg();

    btree.key_ids = key_ids;
    btree.data_ids = data_ids;
    btree.page_k = page_k;
    btree.key_size = key_size;
    btree.data_size = data_size;
    btree.page_size = page_size;
    btree.page_data_size = page_size - 16;
    btree.page_offset = page_starting_offset;
}

Page* b_tree_fetch_page(BTree& btree, int position) {
    std::fstream* fs = &btree.fs;
    fs->seekg(btree.page_offset + position * btree.page_size);
    char* buf = (char*)malloc(btree.page_data_size);

    fs->read(buf, 4);
    if (fs->eof()) return nullptr;
    int size = *((int*)buf);
    fs->read(buf, 4);
    if (fs->eof()) return nullptr;
    int fill = *((int*)buf);
    fs->read(buf, 8);
    if (fs->eof()) return nullptr;
    long parent = *((long*)buf);

    memset(buf, 0, btree.page_data_size);
    fs->read(buf, btree.page_data_size);

    Page* p = new Page;
    p->size = size;
    p->fill = fill;
    p->parent_page = parent;
    p->page_data = buf;
    return p;
}

void b_tree_write_page(BTree& btree, Page* page, int position) {
    std::fstream* fs = &btree.fs;
    fs->clear();
    fs->seekp(btree.page_offset);

    fs->write((char*)&page->size, 4);
    fs->write((char*)&page->fill, 4);
    fs->write((char*)&page->parent_page, 8);
    fs->write((char*)page->page_data, btree.page_data_size);
    if (fs->fail()) exit_m("Page write failed");
    fs->flush();
    if (fs->fail()) exit_m("Writebuffer Flush failed");
}

enum { LEFT_LARGER = 1, RIGHT_LARGER, EQUAL };

struct C_Res {
    int res;
    int offset_add;
};

C_Res int_compare(void* l_key, void* r_key, int offset) {
    int l = *((int*)((char*)l_key + offset));
    int r = *((int*)((char*)r_key + offset));

    if (l > r)
        return {LEFT_LARGER, 4};
    else if (l < r)
        return {RIGHT_LARGER, 4};
    else
        return {EQUAL, 4};
}

C_Res long_compare(void* l_key, void* r_key, int offset) {
    long l = *((long*)((char*)l_key + offset));
    long r = *((long*)((char*)r_key + offset));

    if (l > r)
        return {LEFT_LARGER, 8};
    else if (l < r)
        return {RIGHT_LARGER, 8};
    else
        return {EQUAL, 8};
}

C_Res char_compare(void* l_key, void* r_key, int offset, int id) {
    int length = id_to_byte_size(id);
    char* l_p = (char*)l_key;
    char* r_p = (char*)r_key;

    for (int i = 0; i < length; ++i) {
        if (l_p[offset + i] == r_p[offset + i]) continue;
        if (l_p[offset + i] > r_p[offset + i]) return {LEFT_LARGER, length};
        if (l_p[offset + i] < r_p[offset + i]) return {RIGHT_LARGER, length};
    }
    return {EQUAL, length};
}

int comp_keys(void* l_key, void* r_key, std::vector<int> keys_ids) {
    int char_offset = 0;
    for (const int& id : keys_ids) {
        C_Res res;
        if (id == 1 || id == 6 || id == 7) res = int_compare(l_key, r_key, char_offset);
        if (id == 3 || id == 4 || id == 5) res = char_compare(l_key, r_key, char_offset, id);
        if (id == 2) res = long_compare(l_key, r_key, char_offset);
        if (res.res != EQUAL) return res.res;
        char_offset += res.offset_add;
    }
    return EQUAL;
}

bool b_tree_insert_record(BTree& btree, void* key, void* data) {
    Page* root = b_tree_fetch_page(btree, 0);
    if (root == nullptr) {
        char buf[btree.page_data_size];
        memset(buf, 0, btree.page_data_size);
        memcpy(buf + 8, key, btree.key_size);
        memcpy(buf + 8 + btree.key_size, data, btree.data_size);
        Page* new_root = new Page{btree.page_k, 1, -1, buf};
        b_tree_write_page(btree, new_root, 0);
        return true;
    }

    char* start = (char*)root->page_data + 8;
    int step = btree.key_size + btree.data_size + 8;

    int left = 0;
    int right = root->fill - 1;
    int m = (left + right) / 2;
    while (true) {
        int res = comp_keys(key, start + step * m, btree.key_ids);
        if (res == EQUAL) {
            return false;
        } else if (res == LEFT_LARGER) {
            left = m + 1;
        } else if (res == RIGHT_LARGER) {
            right = m - 1;
        }
        m = (left + right) / 2;
        if (left > right) break;
    }

    if (root->fill == root->size) {
        // split page
        // Page* new_page = new Page{};
        // new_page->size = btree.page_k;
        // new_page->fill = btree.page_k / 2 + 1;
        // new_page->parent_page = 0;
        
        // char buf[btree.page_data_size];
        // memset(buf, 0, btree.page_data_size);
        // memcpy(buf + 8, key, btree.key_size);
        // memcpy(buf + 8 + btree.key_size, data, btree.data_size);
        // Page* new_root = new Page{btree.page_k, 1, -1, buf};
        // b_tree_write_page(btree, new_root, 0);
        // return true;
        return false;
    } else {
        if (left > root->fill - 1) {
            memcpy(start + step * left, key, btree.key_size);
            memcpy(start + step * left + btree.key_size, data, btree.data_size);
            ++root->fill;
            b_tree_write_page(btree, root, 0);
            return true;

        } else {
            for(int i = root->fill - 1; i >= left; --i){
                memcpy(start + step * (i + 1), start + step * i, btree.key_size);
                memcpy(start + step * (i + 1) + btree.key_size, start + step * i + btree.key_size, btree.data_size);
            }
            memcpy(start + step * left, key, btree.key_size);
            memcpy(start + step * left + btree.key_size, data, btree.data_size);
            ++root->fill;
            b_tree_write_page(btree, root, 0);
            return true;
        }
    }

    return false;
}
