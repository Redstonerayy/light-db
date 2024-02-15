#pragma once

class Database;
class TCP_Socket_Server;

void worker_func(TCP_Socket_Server &server, Database &db);
