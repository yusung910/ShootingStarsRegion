// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterSet.h"

MonsterSet::MonsterSet()
{
}

MonsterSet::~MonsterSet()
{
}

bool MonsterSet::isMonsterAllDeath()
{
	bool bRet = true;
	for (auto mon : monsters)
	{
		if (mon.second.MonsterCond != ECondition::IS_DEATH)
		{
			bRet = false;
			continue;
		}
	}
	return bRet;
}

void MonsterSet::SetMonsterVO(MonsterVO* _mvo)
{
	monsters[_mvo->Id] = _mvo;
}

ostream& operator<<(ostream& stream, MonsterSet& info)
{
	stream << info.monsters.size() << endl;
	for (auto& kvp : info.monsters)
	{
		stream << kvp.first << endl;
		stream << kvp.second << endl;
	}

	return stream;
}

ostream& operator<<(ostream& stream, MonsterSet* info)
{
	stream << info->monsters.size() << endl;
	for (auto& kvp : info->monsters)
	{
		stream << kvp.first << endl;
		stream << kvp.second << endl;
	}

	return stream;
}

istream& operator>>(istream& stream, MonsterSet& info)
{
	int nMonsters = 0;
	int PrimaryId = 0;
	MonsterVO monster;
	info.monsters.clear();

	stream >> nMonsters;
	for (int i = 0; i < nMonsters; i++)
	{
		stream >> PrimaryId;
		stream >> monster;
		info.monsters[PrimaryId] = monster;
	}

	return stream;
}
