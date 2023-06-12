// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"

#include <string>
#include <map>
#include <iostream>
#include "MonsterVO.h"

using namespace std;

class MonsterSet
{
public:
	MonsterSet();
	~MonsterSet();
	map<int, MonsterVO> monsters;

	bool isMonsterAllDeath();

	void SetMonsterVO(MonsterVO* _mvo);

	friend ostream& operator<<(ostream& stream, MonsterSet& info);
	friend istream& operator>>(istream& stream, MonsterSet& info);


	friend ostream& operator<<(ostream& stream, MonsterSet* info);

};
