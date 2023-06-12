// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "EnumCreatures.h"
/**
 * 
 */
class PlayerVO
{
public:
	PlayerVO();
	~PlayerVO();

	// 세션 아이디
	int SessionID;

	// 위치
	float X, Y, Z;

	// 회전값
	float Yaw, Pitch, Roll;

	// 속도
	float VX, VY, VZ;

	//
	map<string, string> infoMap;

	string USER_SEQ;		//소유 플레이어 시퀀스
	string CHAR_SEQ;		//캐릭터 시퀀스
	string CLASS_SEQ;		//클래스 시퀀스
	float CHAR_LVL;			//캐릭터 레벨
	string CHAR_NM;			//캐릭터명
	string CLASS_NM;		//캐릭터 클래스 이름

	float STR, DEX, VITAL, ENERGY;

	float DAMAGE, ACCURACY_RATE, DODGE_RATE;
	float CUR_HP, MAX_HP, CUR_MANA, MAX_MANA;

	float SINGLE_TRACE_RAD, MULTI_TRACE_RAD;

	float CUR_EXP, REQ_EXP;

	//피해 입은 데미지
	float BE_DAMAGE_AMOUNT;


	ECondition PlayerCond;
	EDamagedType CurrentDamagedType;//받은 데미지 유형
	EDamagedType AttackDamagedType; //공격 데미지 유형

	PlayerVO& operator=(const map<string, string> m);

	friend ostream& operator<<(ostream& stream, PlayerVO& vo);
	friend istream& operator>>(istream& stream, PlayerVO& vo);

};
