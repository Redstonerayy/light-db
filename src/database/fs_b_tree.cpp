#include "fs_b_tree.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

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
<8 Bytes> offset to parent page (0 for root)
<Page data>
    <8 Bytes> page pointer
    <n Bytes> key
    <n Bytes> record to previous key
    ...
    <8 Bytes> page pointer
    zeros for everything not filled
*/

int id_to_byte_size(int id){
    switch(id){
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

void open_b_tree(BTree& btree, std::string filepath) {
    btree.filepath = filepath;
    
    if(!std::filesystem::exists(filepath)){
        std::ofstream of(filepath);
        of.close();
    }

    btree.fs = std::fstream(filepath, std::ios::in | std::ios::out | std::ios::binary);
    std::fstream* fs = &btree.fs;

    unsigned long data_1 = 0x0100100;
    fs->write(reinterpret_cast<char*>(&data_1), sizeof(data_1));
    unsigned long data_2 = 0x0400000;
    fs->write(reinterpret_cast<char*>(&data_2), 1);

    fs->seekg(0);

    char buf;
    std::vector<int> key_ids = {};
    while(true){
        fs->read(&buf, 1);
        if(fs->eof()) exit(EXIT_FAILURE); 
        if(buf == 0) break;
        std::cout << buf << "\n";
        key_ids.emplace_back(buf);
    }

    std::vector<int> data_ids = {};
    while(true){
        fs->read(&buf, 1);
        if(fs->eof()) exit(EXIT_FAILURE); 
        if(buf == 0) break;
        std::cout << buf << "\n";
        data_ids.emplace_back(buf);
    }

    fs->read(&buf, 1);
    if(fs->eof()) exit(EXIT_FAILURE); 
    if(buf < 2) exit(EXIT_FAILURE);
    std::cout << buf << "\n";
    int page_k = buf;

    int max_page_size = 4 + 4 + 8;
    int max_entry_size = 0;
    for(const int &id : key_ids){
        max_entry_size += id_to_byte_size(id);
    }
    for(const int &id : data_ids){
        max_entry_size += id_to_byte_size(id);
    }
    max_page_size += max_entry_size * page_k + max_entry_size * (page_k + 1);



    // look for root / create root page

    fs->close();
}
