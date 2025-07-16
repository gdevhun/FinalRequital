// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FRWeaponSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFRWeaponSlotWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_WeaponSkillCoolTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_Weapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_WeaponSelected;
};
