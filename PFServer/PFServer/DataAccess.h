#pragma once
#include <string>
#include <vector>
#include "DBConnector.h"

class PlayerVO;

class DataAccess : public DBConnector
{

public:
	DataAccess();

	string SearchAccount(const string& Id, const string& Password);

	vector<map< string, string >> SelectCharacterList(const string& user_seq);

	bool InsertCharacter(const PlayerVO& vo);

	bool DeleteCharacter(const PlayerVO& vo);
};

