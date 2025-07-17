// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FRHUDWidget.generated.h"

enum class EWeaponType : uint8;
/**
 * 
 */

UCLASS()
class FINALREQUITAL_API UFRHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFRHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void ShowCrossHair(bool bShow);
	void ChangeCrossHairColor();
	void ResetCrossHairColor();
	void UpdateWeaponSlotBySwap(EWeaponType CurrentEquippedWeaponType);
	void BindActivatedWeaponSlotsToPlayerState();

	UFUNCTION()
	void OnWeaponAcquiredFromState(EWeaponType WeaponType);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> IMG_CrossHair;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UFRSkillSlotWidget> WBP_SkillSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UFRWeaponSlotWidget> WBP_WeaponSlot_1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UFRWeaponSlotWidget> WBP_WeaponSlot_2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UFRWeaponSlotWidget> WBP_WeaponSlot_3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UFRWeaponSlotWidget> WBP_WeaponSlot_4;

};
