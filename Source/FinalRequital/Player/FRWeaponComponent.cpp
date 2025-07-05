// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/FRWeaponComponent.h"
#include "Player/FRGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/FRPlayerController.h"
#include "UI/FRHUDWidget.h"
#include "Actor/FRWeaponBase.h"
#include "FRPlayerState.h"
#include "Actor/FRPushableActor.h"
#include "Camera/CameraComponent.h"
#include "Character/FRMonsterBase.h"
#include "Character/FRSoul.h"
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

	// 획득 여부 검사 PlayerState 기반
	AFRPlayerState* FRPlayerState = OwnerCharacter->GetPlayerState<AFRPlayerState>();
	if (!FRPlayerState || !FRPlayerState->HasWeapon(WeaponType))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon type %d has not been acquired yet."), static_cast<int32>(WeaponType));
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
		GiveAbility(WeaponData->SpecialAttackAbility, 2, SpecialAttackAbilityHandle);

	CurrentWeaponType = WeaponType;

	switch (WeaponType)
	{
	case EWeaponType::Sword:
		OwnerCharacter->AdjustSwordLayerAnim();
		break;
	case EWeaponType::IronMace:
		OwnerCharacter->AdjustIronMaceLayerAnim();
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
		const bool bShowCrosshair = (WeaponType == EWeaponType::Bow) || (WeaponType == EWeaponType::BronzeBell);
		HUD->ShowCrossHair(bShowCrosshair);
	}
	
	if (WeaponType == EWeaponType::BronzeBell)
	{
		
		GetWorld()->GetTimerManager().SetTimer(
			CrossHairCheckTimer,
			this,
			&UFRWeaponComponent::CheckPushableTarget,
			0.25f,
			true
		);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(CrossHairCheckTimer);
		HUD->ResetCrossHairColor(); 
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
	ClearAbility(SpecialAttackAbilityHandle);

	CurrentWeaponType = EWeaponType::None;

	if (OwnerCharacter)
	{
		OwnerCharacter->AdjustUnarmedLayerAnim();
	}

	if (HUD)
	{
		HUD->ShowCrossHair(false);
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

void UFRWeaponComponent::CheckPushableTarget()
{
	if (!OwnerCharacter || !HUD) return;

	UCameraComponent* Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	FVector Start = Camera->GetComponentLocation();
	FVector Direction = Camera->GetForwardVector();
	FVector End = Start + Direction * 700.0f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (bHit && Hit.GetActor())
	{
		if (Hit.GetActor()->IsA<AFRPushableActor>() || Hit.GetActor()->IsA<AFRMonsterBase>()
			|| Hit.GetActor()->IsA<AFRSoul>())
		{
			HUD->ChangeCrossHairColor(); 
			return;
		}
	}

	HUD->ResetCrossHairColor(); 
}
