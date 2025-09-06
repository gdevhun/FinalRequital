// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRPlayerState.h"
#include "AbilitySystemComponent.h"
#include "FRMaskSkillComponent.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "System/FRGameInstance.h"

AFRPlayerState::AFRPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UFRCharacterAttributeSet>(TEXT("AttributeSet"));

	Stat_H = 0;
	Stat_D = 0;
	Stat_P = 0;
}

class UAbilitySystemComponent* AFRPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRPlayerState::AcquireWeapon(EWeaponType WeaponType)
{
	AcquiredWeapons.Add(WeaponType);
	OnWeaponAcquired.Broadcast(WeaponType);
}

bool AFRPlayerState::HasWeapon(EWeaponType WeaponType) const
{
	return AcquiredWeapons.Contains(WeaponType);
}


void AFRPlayerState::AcquireMaskSkill(EMaskSkillType SkillType)
{
	SelectedMaskSkill = SkillType;
}

bool AFRPlayerState::HasMaskSkill() const
{
	return SelectedMaskSkill != EMaskSkillType::None; 
}

EMaskSkillType AFRPlayerState::GetSelectedMaskSkill() const
{
	return SelectedMaskSkill;
}

void AFRPlayerState::IncreaseStat(EFRCharacterStatType StatType, int32 Amount)
{
	UFRGameInstance* GI = GetGameInstance<UFRGameInstance>();
	if (GI->RemainingStats==0) return;

	switch (StatType)
	{
	case EFRCharacterStatType::H:
		Stat_H += Amount;
		break;
	case EFRCharacterStatType::D:
		Stat_D += Amount;
		break;
	case EFRCharacterStatType::P:
		Stat_P += Amount;
		break;
	default:
		break;
	}

	ApplyStatsToAttributes();


	GI->PersistentPlayerData.Stat_H = Stat_H;
	GI->PersistentPlayerData.Stat_D = Stat_D;
	GI->PersistentPlayerData.Stat_P = Stat_P;
	GI->RemainingStats--;
	
}

void AFRPlayerState::ApplyStatsToAttributes()
{
	if (!AttributeSet || !ASC) return;

	const float CurrentAttackRate = AttributeSet->GetAttackRate();
	const float CurrentMaxHealth = AttributeSet->GetMaxHealth();
	const float CurrentMana = AttributeSet->GetMana();

	const float NewAttackRate = CurrentAttackRate + Stat_D * 5.0f;
	const float NewMaxHealth = CurrentMaxHealth + Stat_H * 25.0F;
	const float NewMana = CurrentMana + Stat_P;

	AttributeSet->SetAttackRate(NewAttackRate);
	AttributeSet->SetMaxHealth(NewMaxHealth);
	AttributeSet->SetMana(NewMana);
	
}

