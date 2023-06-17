// Fill out your copyright notice in the Description page of Project Settings.

#include "PFOnlineGame.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Cmn.h"
#include "Containers/Queue.h"

#include "GInst.h"
#include "GameFramework/PlayerStart.h"

#include "Kismet/GameplayStatics.h"

#include "MainGMB.h"

#include "User.h"
#include "UserPlayMode.h"

#include "Monster.h"
#include "UserNetworkCntrl.h"

AUserNetworkCntrl::AUserNetworkCntrl()
{
	SessionID = FMath::RandRange(0, 10000);

	nPlayers = -1;
	bNewPlayerEntered = false;

	nMonsters = -1;
	isSpawnMonster = false;
	isDestroyMonster = false;

	PrimaryActorTick.bCanEverTick = true;

	isRecvChat = false;
	isShowedAlert = false;
}

void AUserNetworkCntrl::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	IS_NULL(InPawn);
	UserCharacter = Cast<AUser>(InPawn);
}

void AUserNetworkCntrl::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AUserNetworkCntrl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MainGmode->CheckUpServerCorrespondenceStatus(isServerConnected) && isServerDisconnected)
	{
		//서버 상태가 끊겼을 경우 메세지 출력
		gmode->ShowErrorMsg(isServerConnected);
		isServerDisconnected = false;
	}
	else
	{
		//새로운 플레이어가 접속 되었을 때
		if (bNewPlayerEntered)
		{
			//ACharacter 클래스 컴파일에 필요한 지연시간이 필요한것으로 확인
			fNewPlayerEnterTick += DeltaTime;
			if (fNewPlayerEnterTick >= 1.0f)
			{
				fNewPlayerEnterTick = 0.0f;
				UpdateNewPlayer();
			}
		}

		UpdateWorldInfo();

		if (isDestroyMonster)
		{
			DestroyMonster();
		}

		// 몬스터 업데이트
		UpdateMonsterSet();

		//채팅 메세지 업데이트
		if (isRecvChat)
		{
			UpdateToChatMsgWindow();
		}
	}
}

void AUserNetworkCntrl::BeginPlay()
{
	Super::BeginPlay();

	gmode = Cast<AUserPlayMode>(UGameplayStatics::GetGameMode(GetWorld()));

	MainGmode = Cast<AMainGMB>(gmode);
	gi = Cast<UGinst>(UGameplayStatics::GetGameInstance(GetWorld()));
	gi->InitSocket();
	//몬스터 서버 접속
	isServerConnected = gi->Connect("127.0.0.1", 8001);

	if (MainGmode->CheckUpServerCorrespondenceStatus(isServerConnected))
	{
		isServerDisconnected = true;
		//세션 아이디 생성
		gi->SetSessionID(SessionID);
		gmode->SetPlayerHUDWidget();

		//ClientSocket에 컨트롤러 객체를 저장한다.
		gi->SetUserControllerToClientSocket(this);

		// 캐릭터 등록
		AUser* LocalAUser = Cast<AUser>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		IS_NULL(LocalAUser);

		PlayerVO vo = gi->GetGamePlayerCharVO();

		vo.SessionID = SessionID;

		gi->EnrollPlayer(vo);

		//플레이어 정보가 담긴PlayerVO를 User 액터에 저장
		LocalAUser->SetPlayerVO(vo);
		LocalAUser->SetPlayerCond(ECondition::IS_IDLE);

		//생성된 플레이어 정보를 네트워크를 관리하는 클래스에 저장
		pVO = &vo;

		// Recv 스레드 시작
		gi->StartListen();

		GetWorldTimerManager().SetTimer(SendPlayerInfoHandle, this, &AUserNetworkCntrl::SendPlayerInfo, 0.016f, true);
	};
}

void AUserNetworkCntrl::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	gi->Disconnect();
}

void AUserNetworkCntrl::RecvWorldInfo(CharacterInfo* info)
{
	IS_NULL(info);
	OtherInfos = info;
}

void AUserNetworkCntrl::RecvNewPlayer(PlayerVO* vo)
{
	IS_NULL(vo);
	bNewPlayerEntered = true;
	pVO = vo;
}

void AUserNetworkCntrl::RecvMonsterSet(MonsterSet* monsterSet)
{
	IS_NULL(monsterSet);
	MonsterInfos = monsterSet;
}

void AUserNetworkCntrl::RecvSyncMonster(MonsterVO* _MonVO)
{
	IS_NULL(_MonVO);
	MonsterInfos->monsters[_MonVO->Id] = _MonVO;
}

MonsterSet* AUserNetworkCntrl::SendMonsterSet()
{
	return MonsterInfos;
}

void AUserNetworkCntrl::RecvSpawnMonster(MonsterVO* _MonVO)
{
	IS_NULL(_MonVO);
	MonsVO = _MonVO;
	isSpawnMonster = true;
}

void AUserNetworkCntrl::RecvDestroyMonster(MonsterVO* _MonVO)
{
	IS_NULL(_MonVO);
	MonsVO = _MonVO;
	isDestroyMonster = true;
}

void AUserNetworkCntrl::SendPlayerInfo()
{
	auto LocalPlayer = Cast<AUser>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	IS_NULL(LocalPlayer);

	PlayerVO vo;
	gi->SetPlayerVOInfo(vo, LocalPlayer);
	gi->SendPlayer(vo);
}

void AUserNetworkCntrl::UpdateNewPlayer()
{
	UWorld* const world = GetWorld();
	if (pVO->SessionID == SessionID)
	{
		bNewPlayerEntered = false;
		pVO = nullptr;
		return;
	}

	// 새로운 플레이어를 필드에 스폰
	FVector LocalSpawnLocation;
	LocalSpawnLocation.X = pVO->X;
	LocalSpawnLocation.Y = pVO->Y;
	LocalSpawnLocation.Z = pVO->Z;

	FRotator LocalSpawnRotation;
	LocalSpawnRotation.Yaw = pVO->Yaw;
	LocalSpawnRotation.Pitch = pVO->Pitch;
	LocalSpawnRotation.Roll = pVO->Roll;

	FActorSpawnParameters LocalSpawnParams;
	LocalSpawnParams.Owner = this->GetParentActor();
	LocalSpawnParams.Instigator = GetInstigator();
	LocalSpawnParams.Name = FName(*FString(to_string(pVO->SessionID).c_str()));

	//중복으로 생성된 이름의 액터가 있는지 찾는다.
	// 다른 플레이어 업데이트
	TArray<AActor*> LocalSpawnedCharacters;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUser::StaticClass(), LocalSpawnedCharacters);

	bool isActorExist = false;

	for (auto tmpUserActor : LocalSpawnedCharacters)
	{
		if (tmpUserActor->GetName().Equals(LocalSpawnParams.Name.ToString()))
		{
			isActorExist = true;
			continue;
		}
	}
	if (!isActorExist)
	{
		AUser* LocalUser = world->SpawnActor<AUser>(AUser::StaticClass(), LocalSpawnLocation, LocalSpawnRotation, LocalSpawnParams);

		LocalUser->SpawnDefaultController();
		LocalUser->SetPlayerSessionId(pVO->SessionID);
		// 필드의 플레이어 정보에 추가
		if (OtherInfos != nullptr)
		{
			PlayerVO LocalPVO;

			gi->SetPlayerVOInfo(LocalPVO, pVO);

			OtherInfos->Players.insert(pair<int, PlayerVO>(pVO->SessionID, LocalPVO));
		}

	}

	bNewPlayerEntered = false;
	pVO = nullptr;
}

bool AUserNetworkCntrl::UpdateWorldInfo()
{
	if (OtherInfos == nullptr)
		return false;

	// 플레이어 업데이트
	UpdatePlayerInfo(OtherInfos->Players[SessionID]);

	for (auto info : OtherInfos->Players)
	{
		LOG_SCREEN_T("USER cond : %d", info.second.PlayerCond);
	}

	// 다른 플레이어 업데이트
	TArray<AActor*> LocalSpawnedCharacters;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUser::StaticClass(), LocalSpawnedCharacters);

	// 새로운 플레이어가 자기 자신이면 무시
	if (nPlayers == -1)
	{
		for (auto& player : OtherInfos->Players)
		{
			

			if (player.second.SessionID == SessionID ||
				player.second.PlayerCond == ECondition::IS_DEATH)
				continue;

			FVector LocalSpawnLocation;
			LocalSpawnLocation.X = player.second.X;
			LocalSpawnLocation.Y = player.second.Y;
			LocalSpawnLocation.Z = player.second.Z;

			FRotator LocalSpawnRotation;
			LocalSpawnRotation.Yaw = player.second.Yaw;
			LocalSpawnRotation.Pitch = player.second.Pitch;
			LocalSpawnRotation.Roll = player.second.Roll;

			FActorSpawnParameters LocalSpawnParams;
			LocalSpawnParams.Owner = this->GetParentActor();
			LocalSpawnParams.Instigator = GetInstigator();
			LocalSpawnParams.Name = FName(*FString(to_string(player.second.SessionID).c_str()));

			AUser* LocalSpawnCharacter = GetWorld()->SpawnActor<AUser>(AUser::StaticClass(), LocalSpawnLocation, LocalSpawnRotation, LocalSpawnParams);

			LocalSpawnCharacter->SpawnDefaultController();

			LocalSpawnCharacter->SetPlayerSessionId(player.second.SessionID);

			LocalSpawnCharacter->SetPlayerCond(player.second.PlayerCond);

		}
		nPlayers = OtherInfos->Players.size();
	}
	else
	{
		gmode->SetPlayerStatusInfo(&OtherInfos->Players[SessionID]);

		for (auto& LocalPlayer : LocalSpawnedCharacters)
		{

			AUser* OtherPlayer = Cast<AUser>(LocalPlayer);
			int nPlayerSessionId = OtherPlayer->GetPlayerSessionId();

			

			if (!OtherPlayer ||
				nPlayerSessionId == -1 ||
				OtherPlayer->GetPlayerSessionId() == SessionID)
			{
				continue;
			}

			PlayerVO* info = &OtherInfos->Players[nPlayerSessionId];

			if (info->PlayerCond != ECondition::IS_DEATH)
			{
				FVector CharacterLocation;
				CharacterLocation.X = info->X;
				CharacterLocation.Y = info->Y;
				CharacterLocation.Z = info->Z;

				FRotator CharacterRotation;
				CharacterRotation.Yaw = info->Yaw;
				CharacterRotation.Pitch = info->Pitch;
				CharacterRotation.Roll = info->Roll;

				FVector CharacterVelocity;
				CharacterVelocity.X = info->VX;
				CharacterVelocity.Y = info->VY;
				CharacterVelocity.Z = info->VZ;


				switch (info->PlayerCond)
				{
				case ECondition::IS_ATTACK:
					OtherPlayer->Attack();
					break;
				}

				OtherPlayer->AddMovementInput(CharacterVelocity);
				OtherPlayer->SetActorRotation(CharacterRotation);
				OtherPlayer->SetActorLocation(CharacterLocation);

			}
		}
	}

	return true;
}

void AUserNetworkCntrl::UpdatePlayerInfo(const PlayerVO& vo)
{
	AUser* LocalPlayer = Cast<AUser>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (vo.PlayerCond != ECondition::IS_DEATHED && vo.CUR_HP <= 0.0f)
	{
		if (!isShowedAlert)
		{
			isShowedAlert = true;
			LocalPlayer->SetPlayerCond(ECondition::IS_DEATHED);
			//플레이어 게임 시작 액터
			AActor* LocalPlayerStart;
			LocalPlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
			FVector StartLoc = LocalPlayerStart->GetActorLocation();

			gmode->ShowAlert(UTF8_TO_TCHAR("캐릭터가 사망하였습니다,\n처음 위치에서 부활합니다."));

			gmode->deleAlertBtn.AddLambda([LocalPlayer, StartLoc, this] {
				//플레이어를 처음 위치로 이동한다.
				LocalPlayer->SetActorLocation(StartLoc);
				LocalPlayer->RecoveryHealth(0.f, 0.f);
				LocalPlayer->SetPlayerCond(ECondition::IS_IDLE);
				isShowedAlert = false;
			});
		}

	} else {
		// 캐릭터 속성 업데이트
		if (LocalPlayer->GetPlayerVO().PlayerCond == ECondition::IS_DAMAGED)
		{
			UE_LOG(LogClass, Log, TEXT("Player damaged"));
		}
	}
}

void AUserNetworkCntrl::DestroyMonster()
{
	UWorld* const world = GetWorld();
	if (world)
	{
		// 스폰된 몬스터에서 찾아 파괴
		//TArray<AActor*> SpawnedMonsters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), SpawnedMonsters);

		for (auto Actor : SpawnedMonsters)
		{
			AMonster* Monster = Cast<AMonster>(Actor);
			if (Monster && Monster->GetMonsterId() == MonsVO->Id)
			{
				gmode->SetMonsterHPRatio(0.f);
				Monster->Dead();
				break;
			}
		}

		// 업데이트 후 초기화
		MonsVO = nullptr;
		isDestroyMonster = false;
	}
}

void AUserNetworkCntrl::UpdateMonsterSet()
{
	if (MonsterInfos == nullptr)
		return;

	UWorld* const world = GetWorld();
	
	if (world)
	{
		if (nMonsters == -1)
		{
			nMonsters = MonsterInfos->monsters.size();

			for (auto& kvp : MonsterInfos->monsters)
			{
				MonsterVO* monsVO = &kvp.second;
				FVector LocalSpawnLocation;
				LocalSpawnLocation.X = monsVO->X;
				LocalSpawnLocation.Y = monsVO->Y;
				LocalSpawnLocation.Z = monsVO->Z;

				FRotator LocalSpawnRotation(0, 0, 0);

				FActorSpawnParameters LocalSpawnParams;
				LocalSpawnParams.Owner = this;
				LocalSpawnParams.Instigator = GetInstigator();
				LocalSpawnParams.Name = FName(*FString(to_string(monsVO->Id).c_str()));
				
				//액터를 스폰할 때 발생하는 충돌을 해결하는 방법을 제시
				//이 구문 때문에 일부 액터가 스폰 안되던 현상 해결
				LocalSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				AMonster* LocalSpawnMonster = world->SpawnActor<AMonster>(AMonster::StaticClass(), LocalSpawnLocation, LocalSpawnRotation, LocalSpawnParams);
				if (LocalSpawnMonster)
				{
					LocalSpawnMonster->SpawnDefaultController();
					LocalSpawnMonster->SetMonsterVO(monsVO);
				}
			}
		}
		else
		{
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), SpawnedMonsters);
			if (SpawnedMonsters.Num() > 0)
			{

				for (auto actor : SpawnedMonsters)
				{
					AMonster* monster = Cast<AMonster>(actor);

					if (monster)
					{
						MonsterVO* MonsterInfo = &MonsterInfos->monsters[monster->GetMonsterId()];

						monster->SetMonsterVO(MonsterInfo);

						if (MonsterInfo->MonsterCond == ECondition::IS_MOVE)
						{
							FVector LocalLocation;
							LocalLocation.X = MonsterInfo->X;
							LocalLocation.Y = MonsterInfo->Y;
							LocalLocation.Z = MonsterInfo->Z;

							monster->MoveToLocation(LocalLocation);
							monster->SetAttackTargetLoc(LocalLocation);

						}
						else if (MonsterInfo->MonsterCond == ECondition::IS_ATTACK)
						{
							monster->Attack();
						}

						else if (MonsterInfo->MonsterCond == ECondition::IS_DEATH)
						{
							gmode->ShowMonsterHPBar(false);
						}
					}
				}
			}
			else
			{
				nMonsters = -1;
				MonsterInfos->monsters.clear();
			}
		}
	}
}

void AUserNetworkCntrl::SetServerConnected(EServerStatusCode code)
{
	isServerConnected = code;
}

void AUserNetworkCntrl::RecvChatMsg(const string* chat)
{
	if (chat != nullptr)
	{
		isRecvChat = true;
		sRecvChatMsg = chat;
	}
}

void AUserNetworkCntrl::UpdateToChatMsgWindow()
{
	isRecvChat = false;
	gmode->SetChatMsgAtWindow(sRecvChatMsg);
}

void AUserNetworkCntrl::StartMainGamePlay(PlayerVO vo)
{
	MainGmode->StartMainGamePlay(vo);
}
