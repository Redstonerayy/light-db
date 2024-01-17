#include <string>
#include <iostream>

#include "client_funcs.hpp"

bool check_query_validity(std::string query_string) {
    bool double_quote = false;
    bool single_quote = false;
    for (int i = 0; i < query_string.size(); ++i) {
        if (query_string.at(i) == (char)34)  // "
            double_quote = single_quote ? double_quote : !double_quote;

        if (query_string.at(i) == (char)39)  // '
            single_quote = double_quote ? single_quote : !single_quote;

        if (query_string.at(i) == ';' && !single_quote && !double_quote) {
            return true;
        }
    }
    return false;
}