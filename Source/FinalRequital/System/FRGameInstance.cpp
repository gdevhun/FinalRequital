// Fill out your copyright notice in the Description page of Project Settings.


#include "System/FRGameInstance.h"

void UFRGameInstance::AcquireWeapon(EWeaponType WeaponType)
{
	AcquiredWeapons.Add(WeaponType);
}

bool UFRGameInstance::HasWeapon(EWeaponType WeaponType) const
{
	return AcquiredWeapons.Contains(WeaponType);
}