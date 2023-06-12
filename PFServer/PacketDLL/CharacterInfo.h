#pragma once

#include "MonsterVO.h"

class CharacterInfo
{
public:
	PACKETDLL_API CharacterInfo();
	PACKETDLL_API ~CharacterInfo();


	PACKETDLL_API friend ostream& operator<<(ostream& stream, CharacterInfo& info);
	PACKETDLL_API friend istream& operator>>(istream& stream, CharacterInfo& info);

	map<int, PlayerVO> players;
};