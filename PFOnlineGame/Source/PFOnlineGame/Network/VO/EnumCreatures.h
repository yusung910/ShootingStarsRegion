#pragma once

#include "CoreMinimal.h"

#include <string>
#include <map>
#include <iostream>

using namespace std;

UENUM(BlueprintType)
enum class ECondition : uint8
{
	IS_DEATH		UMETA(DisplayName = "IS_DEATH"),
	IS_DEATHED		UMETA(DisplayName = "IS_DEATHED"),
	IS_DAMAGED		UMETA(DisplayName = "IS_DAMAGED"),
	IS_ATTACK		UMETA(DisplayName = "IS_ATTACK"),
	IS_MOVE			UMETA(DisplayName = "IS_MOVE"),
	IS_IDLE			UMETA(DisplayName = "IS_IDLE"),
	IS_DASH			UMETA(DisplayName = "IS_DASH"),
};

UENUM(BlueprintType)
enum class EDamagedType : uint8
{
	IS_NORMAL		UMETA(DisplayName = "IS_NORMAL"),
	IS_DOWN			UMETA(DisplayName = "IS_DOWN"),
	IS_LEVITATION	UMETA(DisplayName = "IS_LEVITATION"),
	IS_SLOW			UMETA(DisplayName = "IS_SLOW"),
	IS_PARALYSIS	UMETA(DisplayName = "IS_PARALYSIS"),
	IS_POSION		UMETA(DisplayName = "IS_POSION"),
	IS_FLAME		UMETA(DisplayName = "IS_FLAME"),
};

/**
 * 
 */
class EnumCreatures
{
public:
	EnumCreatures();
	~EnumCreatures();
};
