// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

#include <string>
#include <map>
#include <iostream>

#include "PlayerVO.h"

using namespace std;

class CharacterInfo
{
public:
	CharacterInfo();
	virtual ~CharacterInfo();

	map<int, PlayerVO> Players;

	friend ostream& operator<<(ostream& stream, CharacterInfo& info);
	friend istream& operator>>(istream& stream, CharacterInfo& info);

};
