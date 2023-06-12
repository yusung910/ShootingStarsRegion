// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterInfo.h"

CharacterInfo::CharacterInfo()
{
}

CharacterInfo::~CharacterInfo()
{
}

ostream& operator<<(ostream& stream, CharacterInfo& info)
{
	stream << info.Players.size() << endl;
	for (auto& kvp : info.Players)
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
	PlayerVO LocalPlayerVO;
	info.Players.clear();

	stream >> nPlayers;
	for (int i = 0; i < nPlayers; i++)
	{
		stream >> SessionId;
		stream >> LocalPlayerVO;
		info.Players[SessionId] = LocalPlayerVO;
	}

	return stream;
}