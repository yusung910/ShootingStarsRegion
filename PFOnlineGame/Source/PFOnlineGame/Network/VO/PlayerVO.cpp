// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerVO.h"

PlayerVO::PlayerVO()
{
}

PlayerVO::~PlayerVO()
{
}
PlayerVO& PlayerVO::operator=(const map<string, string> m)
{
	infoMap = m;

	CHAR_SEQ = infoMap["CHAR_SEQ"].c_str();
	CLASS_SEQ = infoMap["CLASS_SEQ"].c_str();
	CHAR_LVL = (infoMap["CHAR_LVL"].empty()) ? 0.0f : stof(infoMap["CHAR_LVL"]);
	CHAR_NM = infoMap["CHAR_NM"].c_str();
	CLASS_NM = infoMap["CLASS_NM"].c_str();
	USER_SEQ = infoMap["USER_SEQ"].c_str();

	STR = stof(infoMap["STR"]);
	DEX = stof(infoMap["DEX"]);
	VITAL = stof(infoMap["VITAL"]);
	ENERGY = stof(infoMap["ENERGY"]);

	CUR_EXP = stoi(infoMap["CUR_EXP"]);
	REQ_EXP = stoi(infoMap["REQ_EXP"]);

	return *this;
}

ostream& operator<<(ostream& stream, PlayerVO& vo)
{

	stream << vo.CHAR_SEQ << endl;
	stream << vo.USER_SEQ << endl;
	stream << vo.CLASS_SEQ << endl;
	stream << vo.CHAR_LVL << endl;
	stream << vo.CHAR_NM << endl;
	stream << vo.CLASS_NM << endl;

	stream << vo.SessionID << endl;

	stream << vo.X << endl;
	stream << vo.Y << endl;
	stream << vo.Z << endl;

	stream << vo.VX << endl;
	stream << vo.VY << endl;
	stream << vo.VZ << endl;

	stream << vo.Yaw << endl;
	stream << vo.Pitch << endl;
	stream << vo.Roll << endl;

	stream << (int)vo.PlayerCond << endl;
	stream << (int)vo.CurrentDamagedType << endl;
	stream << (int)vo.AttackDamagedType << endl;

	stream << vo.CUR_EXP << endl;
	stream << vo.REQ_EXP << endl;

	stream << vo.DAMAGE << endl;
	stream << vo.BE_DAMAGE_AMOUNT << endl;

	stream << vo.ACCURACY_RATE << endl;
	stream << vo.DODGE_RATE << endl;

	stream << vo.MAX_HP << endl;
	stream << vo.CUR_HP << endl;
	stream << vo.MAX_MANA << endl;
	stream << vo.CUR_MANA << endl;

	return stream;
}

istream& operator>>(istream& stream, PlayerVO& vo)
{

	stream >> vo.CHAR_SEQ;
	stream >> vo.USER_SEQ;
	stream >> vo.CLASS_SEQ;
	stream >> vo.CHAR_LVL;
	stream >> vo.CHAR_NM;
	stream >> vo.CLASS_NM;

	stream >> vo.SessionID;

	stream >> vo.X;
	stream >> vo.Y;
	stream >> vo.Z;

	stream >> vo.VX;
	stream >> vo.VY;
	stream >> vo.VZ;

	stream >> vo.Yaw;
	stream >> vo.Pitch;
	stream >> vo.Roll;

	int cond;
	int currentDamagedType;
	int AttackDamagedType;
	stream >> cond;
	stream >> currentDamagedType;
	stream >> AttackDamagedType;

	vo.PlayerCond = (ECondition)cond;
	vo.CurrentDamagedType = (EDamagedType)currentDamagedType;
	vo.AttackDamagedType = (EDamagedType)AttackDamagedType;

	stream >> vo.CUR_EXP;
	stream >> vo.REQ_EXP;

	stream >> vo.DAMAGE;
	stream >> vo.BE_DAMAGE_AMOUNT;

	stream >> vo.ACCURACY_RATE;
	stream >> vo.DODGE_RATE;
	stream >> vo.MAX_HP;
	stream >> vo.CUR_HP;
	stream >> vo.MAX_MANA;
	stream >> vo.CUR_MANA;

	

	return stream;
}
