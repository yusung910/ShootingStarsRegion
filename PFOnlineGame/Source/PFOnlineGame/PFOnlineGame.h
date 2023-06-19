// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"


DECLARE_LOG_CATEGORY_EXTERN(DevLog, Log, All);

// 이 함수가 호출된 함수의 이름과 라인을 매크로로 String화 시킴 - 출처 : 이득우의 언리얼 C++ 게임 개발의 정석
#define LOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// 위의 CALLINFO와 함께 아무 내용이 없는 로그를 찍는 매크로 - 출처 : 이득우의 언리얼 C++ 게임 개발의 정석
#define LOG_S(Verbosity) UE_LOG(DevLog, Verbosity, TEXT("%s"), *LOG_CALLINFO)

// CALLINFO와 함께 텍스트로 로그를 찍는 매크로
#define LOG(Verbosity, Format, ...) UE_LOG(DevLog, Verbosity, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

// Screen에다가 LOG를 직접 찍는 매크로
#define	LOG_SCREEN(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

#define	LOG_SCREEN_FSTR(FSTR) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(FSTR.c_str()))

// 위의 함수에 시간을 넣는 매크로. Tick에서 DeltaTime을 인자로 넣으면 보기 편함.
#define	LOG_SCREEN_T(Format, ...) GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__))

// LOG에 Verbosity를 인자에서 빼고 매크로화 시킨것. 
#define LOG_WARNING(Format, ...) UE_LOG(DevLog, Warning, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define LOG_ERROR(Format, ...) UE_LOG(DevLog, Error, TEXT("%s %s"), *LOG_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

// assert()와 거의 비슷한 용도로 사용가능한 매크로. ...에는 retval이 들어감.
#define CHECK(Expr, ...) {if(!(Expr)) {LOG(Error, "ASSERTION : %s", "'"#Expr"'"); return __VA_ARGS__;}}

// null체크
#define IS_NULL(p) { if(p == nullptr) {LOG(Error, "Object is Null : %s", "'"#p"'"); return ; } }

#define IS_NULL_FALSE(p) { if(p == nullptr) {LOG(Error, "Object is Null : %s", "'"#p"'"); return false; } }

/*    Object Asset을 받습니다.
*    생성자 타임에만 작동합니다.
*    RETURN_POINTER : 받을 포인터
*    CLASS_TYPE : 받을 오브젝트의 클래스
*    PATH : 경로 (TEXT를 붙이지 않습니다.)*/
#define GetObjectAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FObjectFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) {\
    RETURN_POINTER = __##RETURN_POINTER.Object;\
}\
else RETURN_POINTER = nullptr;\
CHECK(RETURN_POINTER)


/*    Class Asset을 받습니다.
*    생성자 타임에만 작동합니다.
*    RETURN_POINTER : 받을 포인터
*    CLASS_TYPE : 받을 클래스 타입
*    PATH : 경로 (TEXT를 붙이지 않습니다.)*/
#define GetClassAsset(RETURN_POINTER, CLASS_TYPE, PATH)\
static ConstructorHelpers::FClassFinder<CLASS_TYPE> __##RETURN_POINTER(TEXT(PATH));\
if (__##RETURN_POINTER.Succeeded()) {\
    RETURN_POINTER = __##RETURN_POINTER.Class;\
}\
else RETURN_POINTER = nullptr;\
CHECK(RETURN_POINTER)

/*
* 머티리얼을 호출한다.
*/
#define GetMaterialInst(RET_PTR, PATH)\
UMaterialInstanceConstant* mat = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), NULL, TEXT(PATH)));\
RET_PTR = UMaterialInstanceDynamic::Create(mat, this);\
CHECK(mat)

/*
* 나이아가라 컴포넌트를 호출한다
* RET_PTR : 리턴 포인터
* ATTACH_PTR : 나이아가라 컴포넌트를 상속시킬 상위 포인터(getMesh());
* COMP_NM : 컴포넌트 명
* PATH: 나이아가라 컴포넌트 경로
*/
#define GetNiagaraComp(RET_PTR, SYS_PTR, ATTACH_PTR, COMP_NM, PATH)\
RET_PTR = CreateDefaultSubobject<UNiagaraComponent>(TEXT(COMP_NM));\
GetObjectAsset(SYS_PTR, UNiagaraSystem, PATH);\
RET_PTR->SetAsset(SYS_PTR);\
RET_PTR->SetupAttachment(ATTACH_PTR);\
RET_PTR->SetVisibility(false);\
CHECK(SYS_PTR)

//일반 함수에서 사용하는 두 벡터간의 선을 그리는 함수
//Tick함수에서는 사용하지 말아야한다.
#define DrawDebugingLine(START, END, COLOR) DrawDebugLine(GetWorld(), START, END, COLOR, true, -1.0F, 0U, 5.F);

//틱함수에서는 사용할 수 있다.
#define DrawDebugingLineT(START, END) DrawDebugLine(GetWorld(), START, END, FColor::Yellow, false, -1.0F, 0U, 5.F);

#define AddFVectorX(V, f) FVector(V.X + f, V.Y, V.Z);

#define ExceptZFVector(V) FVector(V.X, V.Y, 0.f);