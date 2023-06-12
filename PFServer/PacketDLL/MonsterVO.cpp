#include "MonsterVO.h"
#include <thread>
MonsterVO::MonsterVO()
	:X(0), Y(0), Z(0),
	ORI_X(0), ORI_Y(0), ORI_Z(0),
	DEST_X(0), DEST_Y(0), DEST_Z(0),
	CUR_HP(0), MAX_HP(0),
	Id(-1),
	Velocity(600), isPlayerInTraceRange(false),
	TraceRange(700), HitRange(200), Damage(5), HP_Ratio(1)
{
	MonsterCond = ECondition::IS_IDLE;
}

MonsterVO::~MonsterVO()
{
}

void MonsterVO::SetDestLoc(const PlayerVO& target)
{
	DEST_X = target.X;
	DEST_Y = target.Y;
	DEST_Z = target.Z;
}

void MonsterVO::MoveOri()
{
	X = ORI_X;
	Y = ORI_Y;
	Z = ORI_Z;
}

bool MonsterVO::IsOriginPosition()
{
	return (X == ORI_X && Y == ORI_Y && Z == ORI_Z);
}
void MonsterVO::Damaged(float dmg)
{
	CUR_HP -= dmg;
	HP_Ratio = CUR_HP / MAX_HP;
	printf_s("[Battle Info] Hit! 남은 체력 : %f\n", CUR_HP);
}

bool MonsterVO::IsAlive()
{
	bool bIsAlive = (CUR_HP > 0);

	if (!bIsAlive)
	{
		MonsterCond = ECondition::IS_DEATH;
	}
	return bIsAlive;
}

ECondition MonsterVO::GetMonsterCond()
{
	return MonsterCond;
}

void MonsterVO::SetPlayerInTrackingInfo(const map<int, PlayerVO> players)
{
	map<double, PlayerVO> TargetDists;
	
	for (auto p : players)
	{
		PlayerVO tmpPvo = p.second;
		double tmpDist = sqrt(pow(tmpPvo.X - X, 2) + pow(tmpPvo.Y - Y, 2) + pow(tmpPvo.Z - Z, 2));
		TargetDists.insert(pair<double, PlayerVO>(tmpDist, tmpPvo));
	}

	map< double, PlayerVO >::iterator it = TargetDists.begin();
	//목표 좌표
	SetDestLoc(it->second);

	isPlayerInTraceRange = (it->first <= TraceRange && it->second.PlayerCond != ECondition::IS_DEATHED);

	isPlayerInHitRange = (it->first <= HitRange && it->second.PlayerCond != ECondition::IS_DEATHED);

	TrackReqSec = (it->first / Velocity) * 1000;
}


void MonsterVO::SetLocation(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}


void MonsterVO::ChangeLocToDestLoc()
{
	X = DEST_X;
	Y = DEST_Y;
	Z = DEST_Z;
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

	stream << info.CUR_HP << endl;
	stream << info.MAX_HP << endl;
	stream << info.HP_Ratio << endl;

	stream << info.Id << endl;

	stream << info.Damage << endl;
	stream << info.BeDamageAmount << endl;

	stream << info.HitRange << endl;
	stream << (float)info.MonsterCond << endl;


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

	stream >> info.CUR_HP;
	stream >> info.MAX_HP;
	stream >> info.HP_Ratio;

	stream >> info.Id;
	stream >> info.Damage;
	stream >> info.BeDamageAmount;

	stream >> info.HitRange;
	int cond;

	stream >> cond;
	info.MonsterCond = (ECondition)cond;

	return stream;
}