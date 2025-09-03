// Fill out your copyright notice in the Description page of Project Settings.


#include "System/FRGameInstance.h"
#include "kismet/GameplayStatics.h"

UFRGameInstance::UFRGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UFRGameInstance::Init()
{
	Super::Init();
	ApplyMasterVolume(CurrentSettingValue.MasterVolumeValue);
	ApplyMusicVolume(CurrentSettingValue.MusicVolumeValue);
	ApplySFXVolume(CurrentSettingValue.SFXVolumeValue);
}

void UFRGameInstance::ApplyMasterVolume(float Value) const
{
	if (MasterMix && MasterClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(this, MasterMix, MasterClass, Value, 1.0f, 0.0f);
		UGameplayStatics::PushSoundMixModifier(this, MasterMix);
	}
}

void UFRGameInstance::ApplyMusicVolume(float Value) const
{
	if (BGMMix && BGMClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(this, BGMMix, BGMClass, Value, 1.0f, 0.0f);
		UGameplayStatics::PushSoundMixModifier(this, BGMMix);
	}
}


void UFRGameInstance::ApplySFXVolume(float Value) const
{
	if (SFXMix && SFXClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(this, SFXMix, SFXClass, Value, 1.0f, 0.0f);
		UGameplayStatics::PushSoundMixModifier(this, SFXMix);
	}
}

void UFRGameInstance::ApplyMouseSensitivityValue(float Value)
{
	CurrentSettingValue.MouseSensitivityValue = Value;
}
