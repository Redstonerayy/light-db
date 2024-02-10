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
    ... zeros for everything not filled
*/

void open_b_tree(BTree& btree, std::string filepath) {
    btree.filepath = filepath;
    
    if(!std::filesystem::exists(filepath)){
        std::ofstream of(filepath);
        of.close();
    }

    btree.fs = std::fstream(filepath, std::ios::in | std::ios::out | std::ios::binary);
    std::fstream* fs = &btree.fs;

    const std::string a = "asdfasdf";
    *fs << a << std::endl;
    // fs->write(a.c_str(), 7);
    // std::cout << fs->eof();
    fs->close();
    // look for root / create root page
}
