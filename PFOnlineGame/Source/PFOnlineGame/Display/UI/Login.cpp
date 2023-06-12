// Fill out your copyright notice in the Description page of Project Settings.
#include "PFOnlineGame.h"
#include "Components/EditableText.h"

#include "Components/Button.h"
#include "CharManagementGMB.h"

#include "Login.h"

void ULogin::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	gmode = Cast<ACharManagementGMB>(UGameplayStatics::GetGameMode(GetWorld()));
	//패스워드 처리
	UserPW->SetIsPassword(true);
	btnLogin->OnClicked.AddDynamic(this, &ULogin::DoLogin);
	btnExit->OnClicked.AddDynamic(this, &ULogin::DoGameExit);

	delPwInput.BindUFunction(this, "PwCommitted");
	UserPW->OnTextCommitted.Add(delPwInput);

	UserPW->SetKeyboardFocus();

}

void ULogin::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

void ULogin::DoLogin()
{
	gmode->Login(UserID->GetText(), UserPW->GetText());
}

void ULogin::DoGameExit()
{
	gmode->ShowConfirm(UTF8_TO_TCHAR("게임을 종료하시겠습니까?"));
	gmode->deleConfirmBtn.AddLambda([this] {
		gmode->GameEnd();
	});
}

void ULogin::PwCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		DoLogin();
	}
}
