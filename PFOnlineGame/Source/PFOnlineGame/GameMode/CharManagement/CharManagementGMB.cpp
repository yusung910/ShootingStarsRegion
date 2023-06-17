// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "CharSelectCamSplineA.h"
#include "CharacterSelect.h"

#include "GInst.h"
#include "Kismet/GameplayStatics.h"

#include "User.h"
#include "CharManagementGMB.h"

ACharManagementGMB::ACharManagementGMB()
{
	DefaultPawnClass = ACharSelectCamSplineA::StaticClass();

	//로그인 위젯 호출
	GetClassAsset(LoginWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Display/UI/BPW_Login.BPW_Login_C'");

	//캐릭터 목록 위젯
	GetClassAsset(CharListWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Display/UI/BPW_CharSelect.BPW_CharSelect_C'");

	//캐릭터 생성 위젯
	GetClassAsset(CharCreateWidgetClass, UUserWidget, "WidgetBlueprint'/Game/Blueprints/Display/UI/BPW_CharCreate.BPW_CharCreate_C'");
}
void ACharManagementGMB::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	ShowErrorMsg(gi->InitSocket());

	EServerStatusCode connectStatus = gi->Connect("127.0.0.1", 8000);
	ShowErrorMsg(connectStatus);

	if (connectStatus == EServerStatusCode::CONNECTED)
	{
		IS_NULL(LoginWidgetClass);
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), LoginWidgetClass); //위젯 생성
		CurrentWidget->AddToViewport(); //뷰포트에 추가
		//게임모드 세팅
		gi->SetGameModeToClientSocket(UGameplayStatics::GetGameMode(GetWorld()));
	}

	CamSplineActor = Cast<ACharSelectCamSplineA>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharSelectCamSplineA::StaticClass()));

	TArray<AActor*> pos;

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), "CharLoc", pos);

	for (auto posActor : pos)
	{
		if (posActor->ActorHasTag("UserCharLoc"))
		{
			UserCharLoc = posActor->GetActorLocation();
		}
		else if (posActor->ActorHasTag("CreateCharLoc"))
		{
			CreateCharLoc = posActor->GetActorLocation();
		}
	}
}

void ACharManagementGMB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ACharManagementGMB::Login(const FText& Id, const FText& Pw)
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	if (Id.IsEmpty() || Pw.IsEmpty())
	{
		ShowAlert(UTF8_TO_TCHAR("ID와 비밀번호를 입력하여주세요."));
	}
	else
	{
		EServerStatusCode loginRes = gi->Login(Id, Pw);
		ShowErrorMsg(loginRes);
		if (loginRes == EServerStatusCode::SUCCESS)
		{
			EServerStatusCode searchCharRes = gi->SearchCharacters();
			ShowErrorMsg(searchCharRes);

			//캐릭터 선택 화면으로 이동한다.
			CurrentWidget->RemoveFromViewport();
			CamSplineActor->MoveToSplinePointAt(0, 1);

			deleCameraMoveDone.Clear();
			deleCameraMoveDone.AddLambda([this] {
				ShowCharacterSelectWidget();
			});
		}
	}
}

void ACharManagementGMB::CreateCharacter(PlayerVO vo)
{
	gi->CreateCharacter(vo);
}

void ACharManagementGMB::DeleteCharacter(PlayerVO vo)
{
	EServerStatusCode rslt = gi->DeleteCharacter(vo);
	ShowErrorMsg(rslt);

	if (rslt == EServerStatusCode::SUCCESS)
	{
		gi->SearchCharacters();
		ShowCharacterSelectWidget();
		if (userActor != nullptr)
		{
			userActor->Destroy();
		}
	}
}


void ACharManagementGMB::SetCharacters(vector<PlayerVO> chars)
{
	UserCharList = chars;
}

void ACharManagementGMB::ShowCharacterSelectWidget()
{
	if (CharListWidget != nullptr)
	{
		CharListWidget->RemoveFromViewport();
	}

	CharListWidget = CreateWidget<UUserWidget>(GetWorld(), CharListWidgetClass); //위젯 생성
	CharListWidget->AddToViewport();
	Cast<UCharacterSelect>(CharListWidget)->InitCharacterList(UserCharList);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}


void ACharManagementGMB::InitCharSelectBtnColor()
{
	IS_NULL(CharListWidget);
	Cast<UCharacterSelect>(CharListWidget)->InitCharSelectBtnColor();
}


void ACharManagementGMB::SpawnUserActorInCharacterSelect(PlayerVO vo)
{
	if (userActor != nullptr)
	{
		userActor->Destroy();
	}

	FTransform transform;
	transform.SetLocation(UserCharLoc);
	transform.SetRotation(FQuat(FRotator(0, 61, 0)));

	//캐릭터를 소환한다.
	userActor = GetWorld()->SpawnActor<AUser>(AUser::StaticClass(), transform);
}


void ACharManagementGMB::SpawnCharActorInCreateCharacter(FString classSeq)
{
	if (CreateActor != nullptr)
	{
		CreateActor->Destroy();
	}

	FTransform transform;
	transform.SetLocation(CreateCharLoc);
	transform.SetRotation(FQuat(FRotator(0, -240, 0)));

	//캐릭터를 소환한다.
	CreateActor = GetWorld()->SpawnActor<AUser>(AUser::StaticClass(), transform);
}


void ACharManagementGMB::ShowCreateCharacterWidget()
{
	if (userActor != nullptr)
	{
		userActor->Destroy();
	}

	CharListWidget->RemoveFromViewport();

	CamSplineActor->MoveToSplinePointAt(1, 2);
	deleCameraMoveDone.Clear();
	deleCameraMoveDone.AddLambda([this] {
		CharCreateWidget = CreateWidget<UUserWidget>(GetWorld(), CharCreateWidgetClass); //위젯 생성
		CharCreateWidget->AddToViewport();
	});
}

void ACharManagementGMB::MoveCamCharSelectFromCreate()
{
	gi->SearchCharacters();

	CharCreateWidget->RemoveFromViewport();
	CamSplineActor->MoveToSplinePointAt(2, 1);
	deleCameraMoveDone.Clear();
	deleCameraMoveDone.AddLambda([this] {
		ShowCharacterSelectWidget();
		});
}


void ACharManagementGMB::SetCharListWidgetEnable(bool b)
{
	if (CharListWidget != nullptr)
	{
		CharListWidget->SetIsEnabled(b);
	}
}