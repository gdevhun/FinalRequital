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

	void UpdateSkillImage(EMaskSkillType SelectedSkillType);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_MaskSkillCoolTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_MaskSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage>IMG_MaskSkillDeactivated;

	// 미리 할당된 스킬별 이미지
	UPROPERTY(EditAnywhere, Category = "Skill")
	TMap<EMaskSkillType, UTexture2D*> SkillImages;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TObjectPtr<class UTexture2D> ActivatedSkillFrame;

private:

};
