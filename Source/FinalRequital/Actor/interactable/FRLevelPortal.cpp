// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Interactable/FRLevelPortal.h"
#include "Character/FRCharacterBase.h"
#include "Player/FRGASCharacterPlayer.h"
#include "Player/FRPlayerState.h"
#include "System/FRGameInstance.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AFRLevelPortal::AFRLevelPortal()
{
	PortalEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PortalEffect"));
	PortalEffect->SetupAttachment(RootComponent);
}

void AFRLevelPortal::BeginPlay()
{
	Super::BeginPlay();
	if (bIsAutoActivate)
	{
		PortalEffect->Activate();
	}
	else PortalEffect->Deactivate();
}

void AFRLevelPortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AFRCharacterBase* Player = Cast<AFRCharacterBase>(OtherActor))
	{
		if (bIsActivated)
		{
			AFRPlayerState* PS = Player->GetPlayerState<AFRPlayerState>();
			if (PS)
			{
				if (UFRGameInstance* GI = GetGameInstance<UFRGameInstance>())
				{
					GI->PersistentPlayerData.AcquiredWeapons = PS->AcquiredWeapons;
					GI->PersistentPlayerData.SelectedMaskSkill = PS->GetSelectedMaskSkill();
					GI->PersistentPlayerData.Stat_H = PS->Stat_H;
					GI->PersistentPlayerData.Stat_D = PS->Stat_D;
					GI->PersistentPlayerData.Stat_P = PS->Stat_P;
				}
			}
			ShowInteractionPrompt();
			Player->SetInteractiveActor(this);
		}
	}
}

void AFRLevelPortal::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AFRCharacterBase* Player = Cast<AFRCharacterBase>(OtherActor))
	{
		if (bIsActivated)
		{
			HideInteractionPrompt();
			Player->SetInteractiveActor(nullptr);
		}
	}
}

void AFRLevelPortal::Interact_Implementation()
{
	Super::Interact_Implementation();

}

void AFRLevelPortal::ActivatePortal()
{
	bIsActivated = true;

	if (PortalEffect)
	{
		PortalEffect->Activate(true);
	}
}

void AFRLevelPortal::OpenNextLevel()
{
	if (NextLevelName != NAME_None)
	{
		if (UFRGameInstance* GI = GetGameInstance<UFRGameInstance>())
		{
			GI->CurrentLevel = NextLevelName;
		}
		UGameplayStatics::OpenLevel(this, NextLevelName);
	}
}

