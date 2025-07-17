// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FRUserWidget.h"
#include "FRSkillSlotWidget.generated.h"

enum class EMaskSkillType : uint8;
/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRSkillSlotWidget : public UFRUserWidget
{
	GENERATED_BODY()

public:
	UFRSkillSlotWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_SkillCoolTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_Skill;

	// 미리 할당된 스킬별 이미지
	UPROPERTY(EditAnywhere, Category = "Skill")
	TMap<EMaskSkillType, UTexture2D*> SkillImages;

private:
	void InitializeImage();

	// 현재 선택된 스킬
	EMaskSkillType CurrentSkill;

	// 쿨타임 업데이트용 타이머
	FTimerHandle CooldownTimerHandle;

	
};
