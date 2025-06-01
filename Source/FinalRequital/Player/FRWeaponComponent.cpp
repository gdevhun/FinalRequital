// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FRWeaponComponent.h"
#include "Player/FRGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/FRPlayerController.h"
#include "UI/FRHUDWidget.h"
#include "Actor/FRWeaponBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"

UFRWeaponComponent::UFRWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFRWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AFRGASCharacterPlayer>(GetOwner());
	if (OwnerCharacter)
	{
		ASC = OwnerCharacter->GetAbilitySystemComponent();

		if (AFRPlayerController* PC = Cast<AFRPlayerController>(OwnerCharacter->GetController()))
		{
			HUD = PC->GetHUDWidget();
		}
	}
}

void UFRWeaponComponent::EquipWeapon(EWeaponType WeaponType)
{
	if (!ASC || !OwnerCharacter || WeaponType == CurrentWeaponType)
	{
		ClearWeapon();
		return;
	}

	const FWeaponData* WeaponData = WeaponSlots.Find(WeaponType);
	if (!WeaponData || !WeaponData->WeaponActorClass)
		return;

	ClearWeapon();

	AFRWeaponBase* Weapon = nullptr;

	if (WeaponActorCache.Contains(WeaponType))
	{
		Weapon = WeaponActorCache[WeaponType];
	}
	else
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Weapon = GetWorld()->SpawnActor<AFRWeaponBase>(WeaponData->WeaponActorClass, SpawnParams);
		if (Weapon)
		{
			WeaponActorCache.Add(WeaponType, Weapon);
		}
	}

	if (Weapon)
	{
		Weapon->AttachToComponent(
			OwnerCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			WeaponData->AttachSocketName);

		Weapon->SetVisible(true);
		CurrentWeaponActor = Weapon;
	}

	if (WeaponData->AttackAbility)
		GiveAbility(WeaponData->AttackAbility, 1, AttackAbilityHandle);

	if (WeaponData->SpecialAttackAbility)
		GiveAbility(WeaponData->SpecialAttackAbility, 2, SubAttackAbilityHandle);

	CurrentWeaponType = WeaponType;

	switch (WeaponType)
	{
	case EWeaponType::Sword:
	case EWeaponType::IronMace:
		OwnerCharacter->AdjustMeleeLayerAnim();
		break;
	case EWeaponType::Bow:
		OwnerCharacter->AdjustBowLayerAnim();
		break;
	case EWeaponType::BronzeBell:
		OwnerCharacter->AdjustBronzeBellLayerAnim();
		break;
	default:
		break;
	}

	if (HUD)
	{
		const bool bShowCrosshair = (WeaponType == EWeaponType::Bow);
		HUD->ShowCrosshair(bShowCrosshair);
	}
}

void UFRWeaponComponent::ClearWeapon()
{
	if (CurrentWeaponActor)
	{
		CurrentWeaponActor->SetVisible(false);
		CurrentWeaponActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeaponActor = nullptr;
	}

	ClearAbility(AttackAbilityHandle);
	ClearAbility(SubAttackAbilityHandle);

	CurrentWeaponType = EWeaponType::None;

	if (OwnerCharacter)
	{
		OwnerCharacter->AdjustUnarmedLayerAnim();
	}

	if (HUD)
	{
		HUD->ShowCrosshair(false);
	}
}

void UFRWeaponComponent::GiveAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 InputID, FGameplayAbilitySpecHandle& OutHandle)
{
	if (!ASC || !AbilityClass)
		return;

	FGameplayAbilitySpec Spec(AbilityClass, 1, InputID);
	OutHandle = ASC->GiveAbility(Spec);
}

void UFRWeaponComponent::ClearAbility(FGameplayAbilitySpecHandle& Handle)
{
	if (ASC && Handle.IsValid())
	{
		ASC->ClearAbility(Handle);
		Handle = FGameplayAbilitySpecHandle();
	}
}
