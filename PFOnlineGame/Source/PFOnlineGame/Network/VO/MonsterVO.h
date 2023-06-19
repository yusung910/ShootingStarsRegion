// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "EnumCreatures.h"

class MonsterVO
{
public:
	MonsterVO();
	~MonsterVO();

	float	X, Y, Z;				// X, Y, Z좌표
	float	ORI_X, ORI_Y, ORI_Z;	//원래 자리
	float	DEST_X, DEST_Y, DEST_Z;	//목표 좌표
	float	CUR_HP, MAX_HP, HP_Ratio;			// 체력
	int		Id;				// 고유 id
	float	TraceRange;		// 추격 범위
	float	HitRange;		// 타격 범위
	float	MovePoint;		// 이동 포인트
	float	HitPoint;		// 타격 포인트
	float	Damage;
	float	BeDamageAmount;

	ECondition MonsterCond;

	void SetMonsterPosition(FVector loc);

	friend ostream& operator<<(ostream& stream, MonsterVO& info);
	friend ostream& operator<<(ostream& stream, MonsterVO* info);

	friend istream& operator>>(istream& stream, MonsterVO& info);

	friend MonsterVO operator<<(MonsterVO dest, MonsterVO* target);

	MonsterVO operator=(MonsterVO* target);

};
