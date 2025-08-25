// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Player/FRMaskSkillComponent.h"
#include "Player/FRWeaponComponent.h"
#include "FRGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSettingValues
{
	GENERATED_BODY()

public:

	UPROPERTY()
	float MasterVolumeValue; 

	UPROPERTY()
	float MusicVolumeValue; 

	UPROPERTY()
	float SFXVolumeValue; 

	UPROPERTY()
	float MouseSensitivityValue; 

	FSettingValues()
		: MasterVolumeValue(0.5f), MusicVolumeValue(0.5f), SFXVolumeValue(0.5f), MouseSensitivityValue(0.5f) {
	}
};

USTRUCT(BlueprintType)
struct FPlayerPersistentData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<EWeaponType> AcquiredWeapons;

	UPROPERTY(BlueprintReadWrite)
	EMaskSkillType SelectedMaskSkill;

	UPROPERTY(BlueprintReadWrite)
	int32 Stat_H;

	UPROPERTY(BlueprintReadWrite)
	int32 Stat_D;

	UPROPERTY(BlueprintReadWrite)
	int32 Stat_P;

	FPlayerPersistentData()
		: SelectedMaskSkill(EMaskSkillType::None), Stat_H(0), Stat_D(0), Stat_P(0)
	{
	}
};
/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFRGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SettingValue")
	FSettingValues CurrentSettingValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerData")
	FPlayerPersistentData PersistentPlayerData;

protected:
	void ApplyMasterVolume(float Value) const;
	void ApplyMusicVolume(float Value) const;
	void ApplySFXVolume(float Value) const;
	void ApplyMouseSensitivityValue(float Value);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<class USoundMix> MasterMix;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<class USoundClass> MasterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<class USoundMix> BGMMix;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<class USoundClass> BGMClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<class USoundMix> SFXMix;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<class USoundClass> SFXClass;
};
