// Fill out your copyright notice in the Description page of Project Settings.
#include "MonsterVO.h"

MonsterVO::MonsterVO()
{
}

MonsterVO::~MonsterVO()
{
}

void MonsterVO::SetMonsterPosition(FVector loc)
{
	X = loc.X;
	Y = loc.Y;
	Z = loc.Z;
}

ostream& operator<<(ostream& stream, MonsterVO& info)
{
	stream << info.X << endl;
	stream << info.Y << endl;
	stream << info.Z << endl;

	stream << info.ORI_X << endl;
	stream << info.ORI_Y << endl;
	stream << info.ORI_Z << endl;

	stream << info.DEST_X << endl;
	stream << info.DEST_Y << endl;
	stream << info.DEST_Z << endl;

	stream << info.TraceRange << endl;

	stream << info.CUR_HP << endl;
	stream << info.MAX_HP << endl;
	stream << info.HP_Ratio << endl;

	stream << info.Id << endl;

	stream << info.Damage << endl;
	stream << info.BeDamageAmount << endl;

	stream << info.HitRange << endl;

	stream << (int)info.MonsterCond << endl;

	return stream;
}

ostream& operator<<(ostream& stream, MonsterVO* info)
{
	stream << info->X << endl;
	stream << info->Y << endl;
	stream << info->Z << endl;

	stream << info->ORI_X << endl;
	stream << info->ORI_Y << endl;
	stream << info->ORI_Z << endl;

	stream << info->DEST_X << endl;
	stream << info->DEST_Y << endl;
	stream << info->DEST_Z << endl;

	stream << info->TraceRange << endl;

	stream << info->CUR_HP << endl;
	stream << info->MAX_HP << endl;
	stream << info->HP_Ratio << endl;

	stream << info->Id << endl;

	stream << info->Damage << endl;
	stream << info->BeDamageAmount << endl;

	stream << info->HitRange << endl;

	stream << (int)info->MonsterCond << endl;

	return stream;
}

istream& operator>>(istream& stream, MonsterVO& info)
{
	stream >> info.X;
	stream >> info.Y;
	stream >> info.Z;

	stream >> info.ORI_X;
	stream >> info.ORI_Y;
	stream >> info.ORI_Z;

	stream >> info.DEST_X;
	stream >> info.DEST_Y;
	stream >> info.DEST_Z;

	stream >> info.TraceRange;

	stream >> info.CUR_HP;
	stream >> info.MAX_HP;
	stream >> info.HP_Ratio;

	stream >> info.Id;
	stream >> info.Damage;
	stream >> info.BeDamageAmount;
	stream >> info.HitRange;

	float cond;
	stream >> cond;
	info.MonsterCond = (ECondition)cond;

	return stream;
}

MonsterVO operator<<(MonsterVO dest, MonsterVO* target)
{
	dest.X = target->X;
	dest.Y = target->Y;
	dest.Z = target->Z;

	dest.ORI_X = target->ORI_X;
	dest.ORI_Y = target->ORI_Y;
	dest.ORI_Z = target->ORI_Z;

	dest.DEST_X = target->DEST_X;
	dest.DEST_Y = target->DEST_Y;
	dest.DEST_Z = target->DEST_Z;

	dest.Id = target->Id;

	dest.CUR_HP = target->CUR_HP;
	dest.MAX_HP = target->MAX_HP;

	dest.MonsterCond = target->MonsterCond;

	dest.Damage = target->Damage;

	return dest;
}

MonsterVO MonsterVO::operator=(MonsterVO* target)
{
	MonsterVO ret;
	ret.X = target->X;
	ret.Y = target->Y;
	ret.Z = target->Z;

	ret.ORI_X = target->ORI_X;
	ret.ORI_Y = target->ORI_Y;
	ret.ORI_Z = target->ORI_Z;

	ret.DEST_X = target->DEST_X;
	ret.DEST_Y = target->DEST_Y;
	ret.DEST_Z = target->DEST_Z;

	ret.Id = target->Id;

	ret.CUR_HP = target->CUR_HP;
	ret.MAX_HP = target->MAX_HP;

	ret.MonsterCond = target->MonsterCond;

	ret.Damage = target->Damage;
	return ret;
}