// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include <sstream>
#include <algorithm>
#include <string>
#include <cstdlib> //rand(), srand()
#include <ctime> //time()

#include "Runtime/Core/Public/GenericPlatform/GenericPlatformAffinity.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"

#include "Runtime/Engine/Classes/Engine/Engine.h"

#include "MainGMB.h"
#include "UserNetworkCntrl.h"
#include "CharManagementGMB.h"

#include "ClientSocket.h"

#include "CorrespondencePacket.h"

CorrespondencePacket::CorrespondencePacket()
{
}

CorrespondencePacket::~CorrespondencePacket()
{
}

EServerStatusCode CorrespondencePacket::Login(const FText& Id, const FText& Pw)
{
	stringstream SendStream;

	// 로그인 정보를 서버에 보낸다
	SendStream << EPacketType::LOGIN << endl;
	SendStream << TCHAR_TO_UTF8(*Id.ToString()) << endl;
	SendStream << TCHAR_TO_UTF8(*Pw.ToString()) << endl;

	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
		return EServerStatusCode::PACKET_SEND_FAIL;

	// 서버로부터 응답 대기
	int nRecvLen = recv(
		ServerSocket, (CHAR*)&recvBuffer, MAX_BUFFER, 0
	);

	if (nRecvLen <= 0)
		return EServerStatusCode::PACKET_RECEIVE_FAIL;

	stringstream RecvStream;
	RecvStream << recvBuffer;

	int PacketType;
	RecvStream >> PacketType;

	string seq;
	RecvStream >> seq;
	UserSeq = seq;

	if (PacketType != EPacketType::LOGIN)
		return EServerStatusCode::PACKET_RECEIVE_FAIL;

	if (seq == "error")
	{
		return EServerStatusCode::AUTHORIZED_FAIL;
	}

	// 로그인 성공 유무를 반환
	return EServerStatusCode::SUCCESS;
}

EServerStatusCode CorrespondencePacket::SearchCharacters()
{
	stringstream SendStream;
	SendStream << EPacketType::SEARCH_CHARACTER << endl;
	SendStream << UserSeq << endl;

	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
		return EServerStatusCode::PACKET_SEND_FAIL;

	
	// 서버로부터 응답 대기
	int nRecvLen = recv(
		ServerSocket, (CHAR*)&recvBuffer, MAX_BUFFER, 0
	);

	if (nRecvLen <= 0)
		return EServerStatusCode::PACKET_RECEIVE_FAIL;

	stringstream RecvStream;
	RecvStream << recvBuffer;

	int PacketType;
	RecvStream >> PacketType;

	string charParam;
	RecvStream >> charParam;

	//캐릭터 목록
	vector<map<string, string>> chars = SetParseParam(charParam);
	vector<PlayerVO> charVec;

	for (auto d : chars)
	{
		if (!d.empty())
		{
			cinfo = d;
			charVec.push_back(cinfo);
		}
	}
	Cast<ACharManagementGMB>(gmode)->SetCharacters(charVec);

	charVec.clear();
	chars.clear();
	return EServerStatusCode::SUCCESS;
}

EServerStatusCode CorrespondencePacket::CreateCharacter(PlayerVO vo)
{
	stringstream SendStream;
	SendStream << EPacketType::CREATE_CHARACTER << endl;
	SendStream << vo.CLASS_SEQ << endl;
	SendStream << vo.CHAR_NM << endl;

	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
		return EServerStatusCode::PACKET_SEND_FAIL;

	// 서버로부터 응답 대기
	int nRecvLen = recv(
		ServerSocket, (CHAR*)&recvBuffer, MAX_BUFFER, 0
	);

	if (nRecvLen <= 0)
		return EServerStatusCode::PACKET_RECEIVE_FAIL;

	stringstream RecvStream;
	RecvStream << recvBuffer;

	return EServerStatusCode::SUCCESS;
}

EServerStatusCode CorrespondencePacket::DeleteCharacter(PlayerVO vo)
{
	stringstream SendStream;
	SendStream << EPacketType::DELETE_CHARACTER << endl;
	SendStream << vo.CHAR_SEQ << endl;
	

	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
		return EServerStatusCode::PACKET_SEND_FAIL;

	memset(recvBuffer, 0, MAX_BUFFER);

	// 서버로부터 응답 대기
	int nRecvLen = recv(
		ServerSocket, (CHAR*)&recvBuffer, MAX_BUFFER, 0
	);

	if (nRecvLen <= 0)
		return EServerStatusCode::PACKET_RECEIVE_FAIL;

	stringstream RecvStream;
	RecvStream << recvBuffer;

	return EServerStatusCode::SUCCESS;
}

EServerStatusCode CorrespondencePacket::EnrollPlayer(PlayerVO vo)
{
	// 
	stringstream SendStream;
	// 요청 종류
	SendStream << EPacketType::ENROLL_PLAYER << endl;
	SendStream << vo;

	// 캐릭터 정보 전송
	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
	{
		return EServerStatusCode::PACKET_SEND_FAIL;
	}

	return EServerStatusCode::SUCCESS;
}


EServerStatusCode CorrespondencePacket::SendPlayer(PlayerVO vo)
{
	// 
	stringstream SendStream;
	// 요청 종류
	SendStream << EPacketType::SEND_PLAYER << endl;
	SendStream << vo;

	// 캐릭터 정보 전송
	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
	{
		return EServerStatusCode::PACKET_SEND_FAIL;
	}
	return EServerStatusCode::PACKET_SEND_SUCCESS;
}

EServerStatusCode CorrespondencePacket::HitPlayer(PlayerVO vo)
{
	// 
	stringstream SendStream;
	// 요청 종류
	SendStream << EPacketType::HIT_PLAYER << endl;
	SendStream << vo;

	// 캐릭터 정보 전송
	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
	{
		return EServerStatusCode::PACKET_SEND_FAIL;
	}
	return EServerStatusCode::PACKET_SEND_SUCCESS;
}

EServerStatusCode CorrespondencePacket::HitMonster(MonsterVO monsVO)
{
	// 
	stringstream SendStream;
	// 요청 종류
	SendStream << EPacketType::HIT_MONSTER << endl;
	SendStream << monsVO;

	// 피격 정보 전송
	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
		return EServerStatusCode::PACKET_SEND_FAIL;


	return EServerStatusCode::PACKET_SEND_SUCCESS;
}

EServerStatusCode CorrespondencePacket::SendChatMessage(const string& msg, const int SessionId)
{
	// 
	stringstream SendStream;
	// 요청 종류
	SendStream << EPacketType::CHAT << endl;
	SendStream << SessionId << endl;
	SendStream << msg << endl;

	// 메세지 패킷 전송
	int nSendLen = send(
		ServerSocket, (CHAR*)SendStream.str().c_str(), SendStream.str().length(), 0
	);

	if (nSendLen == -1)
		return EServerStatusCode::PACKET_SEND_FAIL;


	return EServerStatusCode();
}
