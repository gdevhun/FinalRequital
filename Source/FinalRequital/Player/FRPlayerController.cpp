// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRPlayerController.h"
#include "UI/MainMenu/FRMainMenuWidget.h"
#include "EnhancedInputComponent.h"
#include "FRGASCharacterPlayer.h"
#include "FRPlayerState.h"
#include "Kismet/GameplayStatics.h"
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

void AFRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//UI Callback Function Bind
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(ESCKeyAction, ETriggerEvent::Started, this, &AFRPlayerController::HandleESCKeyPressed);
		EnhancedInput->BindAction(TabKeyAction, ETriggerEvent::Started, this, &AFRPlayerController::TabKeyPressedCallback);
	}
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

void AFRPlayerController::HandleESCKeyPressed()
{

	// 스택이 비어있다면 → 메인메뉴 열기
	if (UIWidgetStack.Num() == 0)
	{
		if (FRMainMenuWidgetClass)
		{
			MainMenuWidget = CreateWidget<UFRMainMenuWidget>(this, FRMainMenuWidgetClass);
			if (MainMenuWidget)
			{
				PushUIWidgetToStack(MainMenuWidget);
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				bShowMouseCursor = true;
				SetInputMode(FInputModeGameAndUI());
			}
		}
	}
	else
	{
		// 스택에 뭔가 있으면 하나 닫기
		PopUIWidgetFromStack();
	}
}

void AFRPlayerController::TabKeyPressedCallback()
{

}

void AFRPlayerController::PushUIWidgetToStack(UFRStackBaseWidget* NewWidget)
{
	if (!NewWidget) return;
	NewWidget->AddToViewport();
	UIWidgetStack.Add(NewWidget);
}

void AFRPlayerController::PopUIWidgetFromStack()
{
	if (UIWidgetStack.Num() == 0) return;

	// 제일 위에 있는 위젯 제거
	if (UFRStackBaseWidget* TopWidget = UIWidgetStack.Last())
	{
		TopWidget->RemoveFromParent();
	}
	UIWidgetStack.Pop();

	// 다 닫혔다면 → 게임 재개
	if (UIWidgetStack.Num() == 0)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}
