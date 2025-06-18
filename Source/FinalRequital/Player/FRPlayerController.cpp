// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRPlayerController.h"
#include "UI/FRHUDWidget.h"

AFRPlayerController::AFRPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}
void AFRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	this->ConsoleCommand(TEXT("showdebug abilitysystem"));

	// HUD 위젯을 화면에 추가 -> 에셋정보는 블루프린트에서 캐시
	if (FRHUDWidgetClass) // RXHUDWidgetClass가 유효한지 확인
	{
		AFRHUDWidget = CreateWidget<UFRHUDWidget>(GetWorld(), FRHUDWidgetClass);
		if (AFRHUDWidget)
		{
			// HUD를 화면에 추가
			AFRHUDWidget->AddToViewport();
		}
	}
	//SetInputMode(FInputModeGameAndUI()); // UI와 게임 모드 둘 다 활성화
	//bShowMouseCursor = true;              // 마우스 커서 표시

}
