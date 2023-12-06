#include "client_config.hpp"
#include "client_class.hpp"

int main(int argc, char *argv[])
{
    /*------------ CLI args ------------*/
    /*------------ authenticate ------------*/ //skip for now, TODO openssl, secret
    Client client("127.0.0.1", PORT);
    client.Query("SELECT * FROM users; SELECT name FROM cars;");

    return 0;
}
