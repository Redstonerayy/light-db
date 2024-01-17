#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "header.hpp"

std::vector<std::string> split_string(std::string s, std::string delimiter) {
    std::vector<std::string> splitted;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        splitted.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }

    splitted.push_back(s);

    return splitted;
}

std::vector<Row> read_csv(std::string filepath) {
    std::vector<Row> csv_rows;
    std::ifstream file(filepath);
    std::string line;
    while (std::getline(file, line, '\n')) {
        std::vector<std::string> splitted = split_string(line, ",");
        Row new_row;
        new_row.id = std::stoi(splitted.at(0));
        new_row.name = splitted.at(1);
        
        std::vector<std::string> fs = split_string(splitted.at(2), "-");
        for(int i = 0; i < fs.size(); ++i){
            new_row.friendships.emplace_back(Association{new_row.id, std::stoi(fs.at(i))});
        }

        std::vector<std::string> hs = split_string(splitted.at(3), "-");
        for(int i = 0; i < hs.size(); ++i){
            new_row.hostilities.emplace_back(Association{new_row.id, std::stoi(hs.at(i))});
        }
        csv_rows.push_back(new_row);
    }
    return csv_rows;
}
