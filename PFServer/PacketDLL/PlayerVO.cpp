#include "PlayerVO.h"

PlayerVO::PlayerVO()
{

}

PlayerVO::~PlayerVO()
{

}

void PlayerVO::SetCharacterListToStream(ostream& stream, PlayerVO& vo)
{
	for (const auto& m : vo.infoMap)
	{
		stream << m.first << "=" << m.second << "&";
	}

	stream << ",";
}

void PlayerVO::Damaged(float dmg)
{
	CUR_HP -= dmg;
}

PlayerVO& PlayerVO::operator=(const map<string, string> m)
{
	infoMap = m;

	CHAR_SEQ = infoMap["CHAR_SEQ"];
	CLASS_SEQ = infoMap["CLASS_SEQ"];
	CHAR_LVL = stof(infoMap["CHAR_LVL"]);
	CHAR_NM = infoMap["CHAR_NM"];
	CLASS_NM = infoMap["CLASS_NM"];
	USER_SEQ = infoMap["USER_SEQ"];

	STR = stof(infoMap["STR"]);
	DEX = stof(infoMap["DEX"]);
	VITAL = stof(infoMap["VITAL"]);
	ENERGY = stof(infoMap["ENERGY"]);

	CUR_EXP = stoi(infoMap["CUR_EXP"]);
	REQ_EXP = stoi(infoMap["REQ_EXP"]);

	return *this;
}

PlayerVO* operator<<(PlayerVO* pVO, PlayerVO vo)
{
	// 캐릭터의 위치를 저장						
	pVO->SessionID = vo.SessionID;
	pVO->X = vo.X;
	pVO->Y = vo.Y;
	pVO->Z = vo.Z;

	pVO->CHAR_NM = vo.CHAR_NM;
	pVO->USER_SEQ = vo.USER_SEQ;
	pVO->CHAR_SEQ = vo.CHAR_SEQ;
	pVO->CLASS_NM = vo.CLASS_NM;
	pVO->CLASS_SEQ = vo.CLASS_SEQ;
	pVO->CHAR_LVL = vo.CHAR_LVL;

	// 캐릭터의 회전값을 저장
	pVO->Yaw = vo.Yaw;
	pVO->Pitch = vo.Pitch;
	pVO->Roll = vo.Roll;

	// 캐릭터의 속도를 저장
	pVO->VX = vo.VX;
	pVO->VY = vo.VY;
	pVO->VZ = vo.VZ;

	// 캐릭터 속성
	pVO->STR = vo.STR;
	pVO->DEX = vo.DEX;
	pVO->VITAL = vo.VITAL;
	pVO->ENERGY = vo.ENERGY;

	pVO->DAMAGE = vo.DAMAGE;

	pVO->PlayerCond = vo.PlayerCond;
	pVO->CurrentDamagedType = vo.CurrentDamagedType;
	pVO->AttackDamagedType = vo.AttackDamagedType;


	pVO->CUR_EXP = vo.CUR_EXP;
	pVO->REQ_EXP = vo.REQ_EXP;
	pVO->ACCURACY_RATE = vo.ACCURACY_RATE;
	pVO->DODGE_RATE = vo.DODGE_RATE;
	pVO->MAX_HP = vo.MAX_HP;
	pVO->CUR_HP = vo.CUR_HP;
	pVO->MAX_MANA = vo.MAX_MANA;
	pVO->CUR_MANA = vo.CUR_MANA;

	return pVO;
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
	int c_damagedType;
	int a_damagedType;
	stream >> cond;
	stream >> c_damagedType;
	stream >> a_damagedType;
	vo.PlayerCond = (ECondition) cond;
	vo.CurrentDamagedType = (EDamagedType) cond;
	vo.AttackDamagedType = (EDamagedType) cond;
	
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
		info.players.insert(pair<int, PlayerVO>(SessionId, vo));

		vector<float> loc = { vo.X, vo.Y, vo.Z };
		info.PlayerLocs.insert(pair<int, vector<float>> (SessionId, loc));
	}

	return stream;
}