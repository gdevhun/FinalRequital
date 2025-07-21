// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRPlayerController.h"
#include "FRGASCharacterPlayer.h"
#include "UI/HUD/FRHUDWidget.h"

AFRPlayerController::AFRPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}
void AFRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//this->ConsoleCommand(TEXT("showdebug abilitysystem"));

	if (FRHUDWidgetClass) 
	{
		HUDWidget = CreateWidget<UFRHUDWidget>(GetWorld(), FRHUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();

			if (AFRGASCharacterPlayer* PlayerChar = Cast<AFRGASCharacterPlayer>(GetCharacter()))
			{
				HUDWidget->InitHUDWidget(PlayerChar->GetAbilitySystemComponent());
			}
		}
	}
	//SetInputMode(FInputModeGameAndUI()); // UI와 게임 모드 둘 다 활성화
	//bShowMouseCursor = true;              // 마우스 커서 표시

}

void AFRPlayerController::PushUIWidgetToStack(UUserWidget* NewWidget)
{
	if (!NewWidget) return;
	UIWidgetStack.Add(NewWidget);

}

void AFRPlayerController::PopUIWidgetFromStack(UUserWidget* NewWidget)
{
}
