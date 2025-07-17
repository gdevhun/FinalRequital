// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRPlayerState.h"
#include "AbilitySystemComponent.h"
#include "FRMaskSkillComponent.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExtension.h"
#include "UI/HUD/FRWeaponSlotWidget.h"

AFRPlayerState::AFRPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UFRCharacterAttributeSet>(TEXT("AttributeSet"));

	Stat_H = 0;
	Stat_D = 0;
	Stat_P = 0;
}

class UAbilitySystemComponent* AFRPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFRPlayerState, AcquiredWeapons);
	DOREPLIFETIME(AFRPlayerState, SelectedMaskSkill);
	DOREPLIFETIME(AFRPlayerState, Stat_H);
	DOREPLIFETIME(AFRPlayerState, Stat_D);
	DOREPLIFETIME(AFRPlayerState, Stat_P);
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

