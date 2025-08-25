// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRPlayerController.h"
#include "FRGASCharacterPlayer.h"
#include "FRPlayerState.h"
#include "System/FRGameInstance.h"
#include "UI/HUD/FRHUDWidget.h"

AFRPlayerController::AFRPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}
void AFRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//this->ConsoleCommand(TEXT("showdebug abilitysystem"));

	InitializePlayerStateStatus();
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

void AFRPlayerController::InitializePlayerStateStatus() const
{
	AFRPlayerState* PS = GetPlayerState<AFRPlayerState>();
	if (PS)
	{
		if (UFRGameInstance* GI = GetGameInstance<UFRGameInstance>())
		{
			PS->AcquiredWeapons = GI->PersistentPlayerData.AcquiredWeapons;
			PS->AcquireMaskSkill(GI->PersistentPlayerData.SelectedMaskSkill);
			PS->Stat_H = GI->PersistentPlayerData.Stat_H;
			PS->Stat_D = GI->PersistentPlayerData.Stat_D;
			PS->Stat_P = GI->PersistentPlayerData.Stat_P;

			PS->ApplyStatsToAttributes();
		}
	}
}

void AFRPlayerController::PushUIWidgetToStack(UUserWidget* NewWidget)
{
	if (!NewWidget) return;
	UIWidgetStack.Add(NewWidget);

}

void AFRPlayerController::PopUIWidgetFromStack(UUserWidget* NewWidget)
{
}
