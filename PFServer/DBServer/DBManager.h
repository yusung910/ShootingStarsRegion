#pragma once
class DBManager : public GSingleton<DBManager>
{
public:
	DBManager();

	~DBManager();

	string SearchAccount(InMemStream& in);

	vector<map< string, string >> SelectCharacterList(const string& user_seq);

	bool InsertCharacter(const PlayerVO& vo);

	bool DeleteCharacter(const PlayerVO& vo);

private:
	Database db;
};


#define _DBManager DBManager::Singleton()