//#include "DBConnector.h"
//
//DBConnector::DBConnector()
//{
//	driver = nullptr;
//	con = nullptr;
//	Connect();
//}
//
//
//DBConnector::~DBConnector()
//{
//	Disconnect();
//	delete con;
//}
//
//void DBConnector::Connect()
//{
//	driver = get_driver_instance();
//	con = driver->connect(
//		  DB_ADDRESS
//		, DB_ID
//		, DB_PW);
//
//	if (con)
//	{
//		printf_s("INFO::DB 접속 성공\n");
//		con->setSchema(DB_SCHEMA);
//		printf_s("INFO::DB_SCHEMA 세팅 성공\n");
//	}
//	else
//	{
//		printf_s("ERROR::DB 접속 실패\n");
//	}
//}
//
//void DBConnector::Disconnect()
//{
//	con->close();
//}