
#include "CharacterInfo.h"

CharacterInfo::CharacterInfo()
{

}

CharacterInfo::~CharacterInfo()
{
}

ostream& operator<<(ostream& stream, CharacterInfo& info)
{
	stream << info.players.size() << endl;
	for (auto& kvp : info.players)
	{
		stream << kvp.first << endl;
		stream << kvp.second << endl;
	}

	return stream;
}
istream& operator>>(istream& stream, CharacterInfo& info)
{
	int nPlayers = 0;
	int SessionId = 0;
	PlayerVO vo;
	info.players.clear();

	stream >> nPlayers;

	for (int i = 0; i < nPlayers; i++)
	{
		stream >> SessionId;
		stream >> vo;
		info.players[SessionId] = vo;
		//info.players.insert(pair<int, PlayerVO>(SessionId, vo));
	}

	return stream;
}