#pragma once

struct Association {
    int first;
    int second;
};

struct Row {
    int id;
    std::string name;
    std::vector<Association> friendships;
    std::vector<Association> hostilities;
};
