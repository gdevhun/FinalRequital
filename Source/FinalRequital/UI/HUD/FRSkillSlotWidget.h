// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FRSkillSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRSkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFRSkillSlotWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_SkillCoolTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_Skill;
};
