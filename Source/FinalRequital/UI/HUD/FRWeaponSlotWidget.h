// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FRHUDWidget.h"
#include "UI/FRUserWidget.h"
#include "FRWeaponSlotWidget.generated.h"

enum class EWeaponType : uint8;
/**
 * 
 */


UCLASS()
class FINALREQUITAL_API UFRWeaponSlotWidget : public UFRUserWidget
{
	GENERATED_BODY()

public:
	UFRWeaponSlotWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateSlotVisibilityByAcquisition(EWeaponType NewlyAcquiredWeapon);
	void UpdateSlotVisibilityBySwap();
	void StartSlotCooldown(float Remaining, float TotalDuration);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PB_WeaponSkillCoolTime;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_Weapon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_WeaponSelected;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_WeaponDeactivated;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponType")
	EWeaponType ThisSlotWeaponType;

private:
	void UpdateCooldownProgress();
	float CooldownStartTime = 0.f;
	float MaxCooldownTime = 1.f;
	FTimerHandle CooldownUpdateTimerHandle;

};
