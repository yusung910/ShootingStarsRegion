// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ClientSocket.h"
#include "SktCorresCmn.h"

#include "Runtime/Core/Public/HAL/Runnable.h"

/**
 * 
 */
class PFONLINEGAME_API CorrespondencePacket : public ClientSocket
{
public:
	CorrespondencePacket();
	~CorrespondencePacket();

	//////////////////////////////////////////////////////////////////////////
	// 서버와 통신
	//////////////////////////////////////////////////////////////////////////

	// 서버에 로그인
	EServerStatusCode Login(const FText& Id, const FText& Pw);

	//캐릭터 조회
	EServerStatusCode SearchCharacters();

	//캐릭터 생성
	EServerStatusCode CreateCharacter(PlayerVO vo);

	//캐릭터 제거
	EServerStatusCode DeleteCharacter(PlayerVO vo);

	//월드에 접속한 캐릭터 등록
	EServerStatusCode EnrollPlayer(PlayerVO vo);

	//플레이어 정보 전송
	EServerStatusCode SendPlayer(PlayerVO vo);

	//플레이어 히트 패킷
	EServerStatusCode HitPlayer(PlayerVO vo);

	//몬스터 히트 패킷
	EServerStatusCode HitMonster(MonsterVO monsVO);

	//채팅 메세지 전송
	EServerStatusCode SendChatMessage(const string& msg, const int SessionId);

	// 싱글턴 객체 가져오기
	static CorrespondencePacket* GetSingleton()
	{
		static CorrespondencePacket ins;
		return &ins;
	}
private:

};
