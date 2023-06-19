#include "MonsterVO.h"
#include "PlayerVO.h"

#include <thread>
MonsterVO::MonsterVO()
	:X(0), Y(0), Z(0),
	ORI_X(0), ORI_Y(0), ORI_Z(0),
	DEST_X(0), DEST_Y(0), DEST_Z(0),
	CUR_HP(0), MAX_HP(0),
	Id(-1), Velocity(600),
	TraceRange(700), HitRange(200), Damage(5), HP_Ratio(1)
{
	MonsterCond = ECondition::IS_IDLE;
}

MonsterVO::~MonsterVO()
{
}

void MonsterVO::SetDestLoc(PlayerVO vo)
{
	DEST_X = vo.X;
	DEST_Y = vo.Y;
	DEST_Z = vo.Z;
}

void MonsterVO::MoveOri()
{
	DEST_X = ORI_X;
	DEST_Y = ORI_Y;
	DEST_Z = ORI_Z;
}

bool MonsterVO::IsOriginPosition()
{
	return (X == ORI_X && Y == ORI_Y && Z == ORI_Z);
}
void MonsterVO::Damaged(float dmg)
{
	CUR_HP -= dmg;
	HP_Ratio = CUR_HP / MAX_HP;

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

		//printf_s("Monster[%d] to Player[%d] Dist : %f\n", Id, tmpPvo.SessionID,tmpDist);
	}

	map< double, PlayerVO >::iterator it = TargetDists.begin();
	SetDestLoc(it->second);
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

	stream >> info.TraceRange;

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


/*
* monster set
*/
ostream& operator<<(ostream& stream, MonsterSet& info)
{
	stream << info.monsters.size() << endl;
	for (auto& kvp : info.monsters)
	{
		stream << kvp.first << endl;
		stream << kvp.second << endl;
	}

	return stream;
}


istream& operator>>(istream& stream, MonsterSet& info)
{
	int nMonsters = 0;
	int PrimaryId = 0;
	MonsterVO monster;
	info.monsters.clear();

	stream >> nMonsters;
	for (int i = 0; i < nMonsters; i++)
	{
		stream >> PrimaryId;
		stream >> monster;
		info.monsters[PrimaryId] = monster;
	}

	return stream;
}



void MonsterSet::InitializeMonsterSet()
{
	printf_s("info::몬스터 생성\n");
	srand((unsigned int)time(NULL));

	// 몬스터 초기화	
	MonsterVO mFields;

	mFields.X = -2654;
	mFields.Y = 3629;
	mFields.Z = -461;

	mFields.ORI_X = -2654;
	mFields.ORI_Y = 3629;
	mFields.ORI_Z = -461;

	mFields.MAX_HP = 100.0f;
	mFields.CUR_HP = 100.0f;
	mFields.Id = rand();
	mFields.Damage = 1.0f;

	monsters[mFields.Id] = mFields;

	mFields.X = -1374;
	mFields.Y = 3629;
	mFields.Z = -520;

	mFields.ORI_X = -1374;
	mFields.ORI_Y = 3629;
	mFields.ORI_Z = -520;

	mFields.Id = rand();
	monsters[mFields.Id] = mFields;

	mFields.X = -724;
	mFields.Y = 3629;
	mFields.Z = -520;

	mFields.ORI_X = -724;
	mFields.ORI_Y = 3629;
	mFields.ORI_Z = -520;
	mFields.Id = rand();
	monsters[mFields.Id] = mFields;

	mFields.X = -830;
	mFields.Y = 1710;
	mFields.Z = -494;

	mFields.ORI_X = -1160;
	mFields.ORI_Y = 1709;
	mFields.ORI_Z = -410;
	mFields.Id = rand();
	monsters[mFields.Id] = mFields;
}

void MonsterSet::SetMonstersCondition(map<int, PlayerVO> players)
{
	//몬스터가 사망이였을 경우

	//몬스터 근처에 유저가 있는지 판별
	for (auto& kvp : monsters)
	{
		auto& monMap = kvp.second;
		MonsterVO* monster = &monsters[monMap.Id];

		//접속한 플레이어가 없거나 몬스터가 죽었을 경우 실행하지 않는다.
		if (players.size() <= 0 || !monster->IsAlive())
			continue;

		monster->SetPlayerInTrackingInfo(players);
	}

	//for (auto& monMap : monsters)
	//{
	//	auto& mon = monMap.second;
	//	MonsterVO* monster = &monsters[mon.Id];

	//	if (monster->isPlayerInTraceRange)
	//	{
	//		if (monster->isPlayerInHitRange)
	//		{
	//			//if (monster->MonsterCond == ECondition::IS_ATTACK)
	//			//{
	//			//	monster->MonsterCond = ECondition::IS_IDLE;
	//			//}
	//			//else
	//			//{
	//				monster->MonsterCond = ECondition::IS_ATTACK;
	//			//}
	//		}

	//		if (monster->MonsterCond == ECondition::IS_IDLE)
	//		{
	//			monster->MonsterCond = ECondition::IS_MOVE;
	//			monster->ChangeLocToDestLoc();
	//		}
	//	}
	//	else if (!monster->isPlayerInTraceRange &&
	//		!monster->IsOriginPosition())
	//	{
	//		monster->MoveOri();
	//		monster->MonsterCond = ECondition::IS_MOVE;
	//	}
	//	else
	//	{
	//		monster->MonsterCond = ECondition::IS_IDLE;
	//	}
	//}
}