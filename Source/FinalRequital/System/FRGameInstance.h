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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MasterVolumeValue; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MusicVolumeValue; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SFXVolumeValue; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MouseSensitivityValue; 

	FSettingValues()
		: MasterVolumeValue(0.5f), MusicVolumeValue(0.5f), SFXVolumeValue(0.5f), MouseSensitivityValue(0.5f) {
	}
};

USTRUCT(BlueprintType)
struct FPlayerPersistentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EWeaponType> AcquiredWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMaskSkillType SelectedMaskSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stat_H;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Stat_D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	int32 RemainingStats = 0;

protected:
	virtual void Init() override;

public:
	void ApplyMasterVolume(float Value) const;
	void ApplyMusicVolume(float Value) const;
	void ApplySFXVolume(float Value) const;
	void ApplyMouseSensitivityValue(float Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level")
	FName CurrentLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level")
	int LevelSavePoint = 0;

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
