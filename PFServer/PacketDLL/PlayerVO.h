#include "stdafx.h"

class PlayerVO
{
public:
	PACKETDLL_API PlayerVO();
	PACKETDLL_API virtual ~PlayerVO();


	string USER_SEQ;	//소유 플레이어 시퀀스
	string CHAR_SEQ;		//캐릭터 시퀀스
	string CLASS_SEQ;	//클래스 시퀀스
	float CHAR_LVL;		//캐릭터 레벨
	string CHAR_NM;		//캐릭터명
	string CLASS_NM;	//캐릭터 클래스 명

	int SessionID; //세션

	// 위치
	float X, Y, Z;

	// 회전값
	float Yaw, Pitch, Roll;

	// 속도
	float VX, VY, VZ;

	//스텟
	float STR, DEX, VITAL, ENERGY;

	//데미지, 명중률, 회피율, 
	float DAMAGE, ACCURACY_RATE, DODGE_RATE;
		
	//최대체력, 최대 마나
	float CUR_HP, CUR_MANA, MAX_HP, MAX_MANA;

	//현재 경험치
	float CUR_EXP, REQ_EXP;

	//피해 입은 데미지
	float BE_DAMAGE_AMOUNT;

	ECondition PlayerCond;
	EDamagedType CurrentDamagedType;//받은 데미지 유형
	EDamagedType AttackDamagedType; //공격 데미지 유형

	map<string, string> infoMap;

	PACKETDLL_API void SetCharacterListToStream(ostream& stream, PlayerVO& vo);

	PACKETDLL_API void Damaged(float dmg);

	PACKETDLL_API PlayerVO& operator=(const map<string, string> m);
	PACKETDLL_API friend PlayerVO* operator<<(PlayerVO* pVO, PlayerVO vo);

	PACKETDLL_API friend ostream& operator<<(ostream& stream, PlayerVO& vo);
	PACKETDLL_API friend istream& operator>>(istream& stream, PlayerVO& vo);

};

