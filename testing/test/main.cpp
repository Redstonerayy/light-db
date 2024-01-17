#include <iostream>
#include <string>

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

class Test {
   public:
    std::string s;
    Test(std::string s);
};

Test::Test(std::string s){
    this->s = s;
}

int main(int argc, char *argv[]) {
    // stuff
    Test test("asdf");

    std::string query_string = "";
    std::cout << "lightdb>";
    while (true) {
        std::getline(std::cin, query_string, '\n');

        if (check_query_validity(query_string)) {
            // stuff
        } else {
            std::cout << "Error in Query String!\n";
        }
        std::cout << query_string << "\n";
        std::cout << "lightdb>";
    }

    return 0;
}
