// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRPlayerState.h"
#include "AbilitySystemComponent.h"
#include "FRMaskSkillComponent.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "Net/UnrealNetwork.h"

AFRPlayerState::AFRPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UFRCharacterAttributeSet>(TEXT("AttributeSet"));
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
}

void AFRPlayerState::AcquireWeapon(EWeaponType WeaponType)
{
	AcquiredWeapons.Add(WeaponType);
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

