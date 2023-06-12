
#include "PlayerVO.h"

class MonsterVO
{
public:
	PACKETDLL_API MonsterVO();
	PACKETDLL_API virtual ~MonsterVO();
	
	// 플레이어로 이동
	PACKETDLL_API void SetDestLoc(const PlayerVO& target);

	//원래자리로 이동
	PACKETDLL_API void MoveOri();

	PACKETDLL_API bool IsOriginPosition();

	// 피격
	PACKETDLL_API void Damaged(float dmg);
	// 살아있는지 여부
	PACKETDLL_API bool IsAlive();
	// 공격중인지 여부
	PACKETDLL_API ECondition GetMonsterCond();

	// 추격범위에 있는 가장 가까운 플레이어를 확인하고 관련 데이터를 세팅
	PACKETDLL_API void SetPlayerInTrackingInfo(const map<int, PlayerVO> players);

	// 위치 설정
	PACKETDLL_API void SetLocation(float x, float y, float z);

	// 목적지 위치를 현재 위치로 변경
	PACKETDLL_API void ChangeLocToDestLoc();

	PACKETDLL_API friend ostream& operator<<(ostream& stream, MonsterVO& info);

	PACKETDLL_API friend istream& operator>>(istream& stream, MonsterVO& info);

public:
	float	X,Y,Z;					// X, Y, Z 좌표
	float	DEST_X, DEST_Y, DEST_Z;	//목표 좌표
	float	Velocity;
	float	ORI_X, ORI_Y, ORI_Z;	//원래 자리
	float	CUR_HP, MAX_HP;			// 체력
	int		Id;				// 고유 id
	float	TraceRange;		// 추격 범위
	float	HitRange;		// 타격 범위
	float	Damage;


	float	HP_Ratio;

	//피해 입은 데미지
	float BeDamageAmount;

	//
	bool isPlayerInTraceRange;
	//
	double TrackReqSec;
	//
	bool isPlayerInHitRange;

	ECondition MonsterCond;

private:

};


class PACKETDLL_API MonsterSet
{
public:
	map<int, MonsterVO> monsters;

	PACKETDLL_API friend ostream& operator<<(ostream& stream, MonsterSet& info)
	{
		stream << info.monsters.size() << endl;
		for (auto& kvp : info.monsters)
		{
			stream << kvp.first << endl;
			stream << kvp.second << endl;
		}

		return stream;
	}

	PACKETDLL_API friend istream& operator>>(istream& stream, MonsterSet& info)
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
			//info.monsters.insert(pair<int, MonsterVO>(PrimaryId, monster));
			info.monsters[PrimaryId] = monster;
		}

		return stream;
	}
};