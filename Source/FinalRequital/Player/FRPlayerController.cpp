// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRPlayerController.h"
#include "UI/MainMenu/FRMainMenuWidget.h"
#include "EnhancedInputComponent.h"
#include "FRGASCharacterPlayer.h"
#include "FRPlayerState.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/FRGameInstance.h"
#include "UI/HUD/FRHUDWidget.h"

AFRPlayerController::AFRPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bIsInteracting = false;
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

void AFRPlayerController::SwitchToInspectCamera(AActor* TargetActor, float BlendTime, float ForwardOffset, float UpOffset)
{
	if (!TargetActor) return;

	if (UCameraComponent* CamComp = TargetActor->FindComponentByClass<UCameraComponent>())
	{
		if (APawn* PlayerPawn = GetPawn())
		{
			SetViewTargetWithBlend(TargetActor, BlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
			FVector CamLocation = CamComp->GetComponentLocation();
			FRotator CamRotation = CamComp->GetComponentRotation();

			FVector Forward = CamRotation.Vector() * -ForwardOffset;
			FVector Up = FVector::UpVector * UpOffset;
			FVector NewPlayerLocation = CamLocation + Forward + Up;

			PlayerPawn->SetActorLocation(NewPlayerLocation, true);

			FRotator LookAtRotation = (CamLocation - NewPlayerLocation).Rotation();
			FRotator FixedRotation(0.f, LookAtRotation.Yaw, 0.f);
			PlayerPawn->SetActorRotation(FixedRotation);

			if (ACharacter* MyCharacter = Cast<ACharacter>(PlayerPawn))
			{
				if (MyCharacter->GetCharacterMovement())
				{
					MyCharacter->GetCharacterMovement()->DisableMovement();
				}
			}
		}
	}
}

void AFRPlayerController::ReturnToPlayerCamera(float BlendTime)
{
	if (APawn* PlayerPawn = GetPawn())
	{
		SetViewTargetWithBlend(PlayerPawn, BlendTime, EViewTargetBlendFunction::VTBlend_Cubic);

		if (ACharacter* MyCharacter = Cast<ACharacter>(PlayerPawn))
		{
			if (MyCharacter->GetCharacterMovement())
			{
				MyCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			}
		}
	}
}

void AFRPlayerController::HandleESCKeyPressed()
{
	// 상호작용중이라면 return
	if (bIsInteracting) return;

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

void AFRPlayerController::PushUIWidgetToStack(UFRStackBaseWidget* NewWidget)
{
	if (!NewWidget) return;

	if (HUDWidget)
	{
		HUDWidget->SetHUDVisibility(false);
	}

	// 이전 Top 위젯 숨기기
	if (UIWidgetStack.Num() > 0)
	{
		if (UFRStackBaseWidget* TopWidget = UIWidgetStack.Last())
		{
			TopWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	NewWidget->AddToViewport();
	NewWidget->SetVisibility(ESlateVisibility::Visible);

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

	// 이전 위젯 다시 보이게 하기
	if (UIWidgetStack.Num() > 0)
	{
		if (UFRStackBaseWidget* NewTop = UIWidgetStack.Last())
		{
			NewTop->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		// 스택이 완전히 비면 HUD 다시 보이기
		if (HUDWidget)
		{
			HUDWidget->SetHUDVisibility(true);
			HUDWidget->SetStagePlayStatus(false);
		}

		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}
