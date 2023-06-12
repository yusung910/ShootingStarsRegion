// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "User.h"

#include "Ginst.h"


UGinst::UGinst()
{

}

void UGinst::Init()
{
	Super::Init();
}


EServerStatusCode UGinst::InitSocket()
{
	Socket = CorrespondencePacket::GetSingleton();
	return Socket->InitSocket(); 
}
EServerStatusCode UGinst::Connect()
{
	return Socket->Connect("127.0.0.1", 8000);
}

EServerStatusCode UGinst::Login(const FText& Id, const FText& Pw)
{
	return Socket->Login(Id, Pw);
}

EServerStatusCode UGinst::SearchCharacters()
{
	return Socket->SearchCharacters();
}

EServerStatusCode UGinst::CreateCharacter(PlayerVO vo)
{
	return Socket->CreateCharacter(vo);
}

EServerStatusCode UGinst::DeleteCharacter(PlayerVO vo)
{
	return Socket->DeleteCharacter(vo);
}

EServerStatusCode UGinst::EnrollPlayer(PlayerVO vo)
{
	vo.CHAR_LVL = gamePlayChar.CHAR_LVL;
	vo.CHAR_NM = gamePlayChar.CHAR_NM;
	vo.CHAR_SEQ = gamePlayChar.CHAR_SEQ;
	vo.CLASS_NM = gamePlayChar.CLASS_NM;
	vo.CLASS_SEQ = gamePlayChar.CLASS_SEQ;
	vo.USER_SEQ = gamePlayChar.USER_SEQ;

	vo.STR = gamePlayChar.STR;
	vo.DEX = gamePlayChar.DEX;
	vo.VITAL = gamePlayChar.VITAL;
	vo.ENERGY = gamePlayChar.ENERGY;

	return Socket->EnrollPlayer(vo);
}

EServerStatusCode UGinst::SendPlayer(PlayerVO vo)
{
	return Socket->SendPlayer(vo);
}

EServerStatusCode UGinst::HitPlayer(PlayerVO vo)
{
	return Socket->HitPlayer(vo);
}

EServerStatusCode UGinst::HitMonster(MonsterVO monsVO)
{
	return Socket->HitMonster(monsVO);
}

EServerStatusCode UGinst::StartListen()
{
	return Socket->StartListen();
}

EServerStatusCode UGinst::SendChatMessage(const string& msg)
{
	return Socket->SendChatMessage(msg, SessionID);
}

void UGinst::Disconnect()
{
	Socket->Logout(SessionID);
	Socket->CloseSocket();
	Socket->StopListen();
}

void UGinst::SetGameModeToClientSocket(AGameModeBase* gm)
{
	Socket->SetGameMode(gm);
}

void UGinst::SetUserControllerToClientSocket(AUserNetworkCntrl* cntrl)
{
	Socket->SetPlayerController(cntrl);
}


void UGinst::SetPlayerVOInfo(PlayerVO& dest, AUser* depart)
{
	dest.SessionID = SessionID;

	const auto MyLocation = depart->GetActorLocation();
	const auto MyRotation = depart->GetActorRotation();
	const auto MyVelocity = depart->GetVelocity();

	// 위치
	dest.X = MyLocation.X;
	dest.Y = MyLocation.Y;
	dest.Z = MyLocation.Z;
	// 회전
	dest.Yaw = MyRotation.Yaw;
	dest.Pitch = MyRotation.Pitch;
	dest.Roll = MyRotation.Roll;

	// 속도
	dest.VX = MyVelocity.X;
	dest.VY = MyVelocity.Y;
	dest.VZ = MyVelocity.Z;

	// 속성
	dest.PlayerCond = depart->GetPlayerVO().PlayerCond;
	dest.CurrentDamagedType = depart->GetPlayerVO().CurrentDamagedType;
	dest.AttackDamagedType = depart->GetPlayerVO().AttackDamagedType;
	dest.CUR_HP = depart->GetPlayerVO().CUR_HP;
	dest.MAX_HP = depart->GetPlayerVO().MAX_HP;
	dest.CUR_MANA = depart->GetPlayerVO().CUR_MANA;
	dest.MAX_MANA = depart->GetPlayerVO().MAX_MANA;

	dest.DAMAGE = depart->GetPlayerVO().DAMAGE;
	dest.ACCURACY_RATE = depart->GetPlayerVO().ACCURACY_RATE;
	dest.DODGE_RATE = depart->GetPlayerVO().DODGE_RATE;

	dest.USER_SEQ = depart->GetPlayerVO().USER_SEQ;
	dest.CHAR_SEQ = depart->GetPlayerVO().CHAR_SEQ;
	dest.CLASS_SEQ = depart->GetPlayerVO().CLASS_SEQ;
	dest.CHAR_LVL = depart->GetPlayerVO().CHAR_LVL;
	dest.CHAR_NM = depart->GetPlayerVO().CHAR_NM;
	dest.CLASS_NM = depart->GetPlayerVO().CLASS_NM;
	

	dest.CUR_EXP = gamePlayChar.CUR_EXP;
	dest.REQ_EXP = gamePlayChar.REQ_EXP;
	dest.STR = gamePlayChar.STR;
	dest.DEX = gamePlayChar.DEX;
	dest.VITAL = gamePlayChar.VITAL;
	dest.ENERGY = gamePlayChar.ENERGY;

}

void UGinst::SetPlayerVOInfo(PlayerVO& dest, PlayerVO* depart)
{
	dest.SessionID = depart->SessionID;

	dest.X = depart->X;
	dest.Y = depart->Y;
	dest.Z = depart->Z;

	dest.Yaw = depart->Yaw;
	dest.Pitch = depart->Pitch;
	dest.Roll = depart->Roll;

	dest.VX = depart->VX;
	dest.VY = depart->VY;
	dest.VZ = depart->VZ;

	// 속성
	dest.PlayerCond = depart->PlayerCond;
	dest.CurrentDamagedType = depart->CurrentDamagedType;
	dest.AttackDamagedType = depart->AttackDamagedType;
	dest.DAMAGE = depart->DAMAGE;

	dest.DAMAGE = gamePlayChar.DAMAGE;
	dest.ACCURACY_RATE = gamePlayChar.ACCURACY_RATE;
	dest.DODGE_RATE = gamePlayChar.DODGE_RATE;

	dest.USER_SEQ = gamePlayChar.USER_SEQ;
	dest.CHAR_SEQ = gamePlayChar.CHAR_SEQ;
	dest.CLASS_SEQ = gamePlayChar.CLASS_SEQ;
	dest.CHAR_LVL = gamePlayChar.CHAR_LVL;
	dest.CHAR_NM = gamePlayChar.CHAR_NM;
	dest.CLASS_NM = gamePlayChar.CLASS_NM;

	dest.CUR_HP = gamePlayChar.CUR_HP;
	dest.MAX_HP = gamePlayChar.MAX_HP;
	dest.CUR_MANA = gamePlayChar.CUR_MANA;
	dest.MAX_MANA = gamePlayChar.MAX_MANA;

	dest.CUR_EXP = gamePlayChar.CUR_EXP;
	dest.REQ_EXP = gamePlayChar.REQ_EXP;
	dest.STR = gamePlayChar.STR;
	dest.DEX = gamePlayChar.DEX;
	dest.VITAL = gamePlayChar.VITAL;
	dest.ENERGY = gamePlayChar.ENERGY;
}


bool UGinst::GetThreadStatus()
{
	return Socket->GetThreadStatus();
}

void UGinst::SetGamePlayerCharVO(PlayerVO vo)
{
	gamePlayChar = vo;
	int classSeq = stoi(gamePlayChar.CLASS_SEQ);
	float LocalDmg, LocalDodgeRate, LocalAccuracyRate, LocalMaxHp, LocalMaxMana, Local_SINGLE_TRACE_RAD;
	switch (classSeq)
	{
	case 1:
		//검사
		LocalDmg = (gamePlayChar.STR * 5);
		LocalDodgeRate = (gamePlayChar.DEX * 0.9);
		LocalAccuracyRate = (gamePlayChar.DEX * 0.09);
		LocalMaxHp = 100 + (gamePlayChar.VITAL * 5);
		LocalMaxMana = 50 + (gamePlayChar.VITAL * 5);
		Local_SINGLE_TRACE_RAD = 100.f;
		break;
	case 2:
		//거너
		LocalDmg = (gamePlayChar.STR * 3);
		LocalDodgeRate = (gamePlayChar.DEX * 0.09);
		LocalAccuracyRate = (gamePlayChar.DEX * 1.1);
		LocalMaxHp = 90 + (gamePlayChar.VITAL * 5);
		LocalMaxMana = 60 + (gamePlayChar.VITAL * 5);
		break;

	}

	gamePlayChar.DAMAGE = LocalDmg;
	gamePlayChar.DODGE_RATE = LocalDodgeRate;
	gamePlayChar.ACCURACY_RATE = LocalAccuracyRate;
	gamePlayChar.ACCURACY_RATE = LocalAccuracyRate;
	gamePlayChar.MAX_HP = LocalMaxHp;
	gamePlayChar.MAX_MANA = LocalMaxMana;
	gamePlayChar.CUR_HP = LocalMaxHp;
	gamePlayChar.CUR_MANA = LocalMaxMana;
	gamePlayChar.SINGLE_TRACE_RAD = Local_SINGLE_TRACE_RAD;

	gamePlayChar.PlayerCond = vo.PlayerCond;
	gamePlayChar.CurrentDamagedType = vo.CurrentDamagedType;
	gamePlayChar.AttackDamagedType = vo.AttackDamagedType;

}
