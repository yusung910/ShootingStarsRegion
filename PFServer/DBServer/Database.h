#pragma once
#include <string>
#include <stdlib.h>
#include <iostream>
#include <mutex>

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

using namespace std;
using namespace sql;

// DB 정보
#define DB_ADDRESS		"localhost"
#define	DB_PORT			3306
#define DB_ID			"PFOnlineDB"
#define DB_PW			"1111"
#define DB_SCHEMA		"pfonlinegame_db"

class PlayerVO;

class Database
{
public:
	Database();
	~Database();

	// DB 에 연결
	void Connect();
	void Disconnect();

	recursive_mutex databaseLock;

	Driver* driver;
	Connection* con;
};

