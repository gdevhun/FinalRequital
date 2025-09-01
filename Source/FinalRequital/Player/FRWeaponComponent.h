// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ActorComponent.h"
#include "FRWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None			UMETA(DisplayName = "맨손"),
	Sword		    UMETA(DisplayName = "검"),
	Bow             UMETA(DisplayName = "활"),
	IronMace	    UMETA(DisplayName = "철퇴"),
	BronzeBell      UMETA(DisplayName = "청동방울")
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AFRWeaponBase> WeaponActorClass;

	UPROPERTY(EditAnywhere)
	FName AttachSocketName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> AttackAbility;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayAbility> SpecialAttackAbility;
};

UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class FINALREQUITAL_API UFRWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFRWeaponComponent();

	void EquipWeapon(EWeaponType WeaponType);
	void ClearWeapon();
	EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }

protected:
	virtual void BeginPlay() override;

	void GiveAbility(TSubclassOf<class UGameplayAbility> AbilityClass, int32 InputID, FGameplayAbilitySpecHandle& OutHandle);
	void ClearAbility(FGameplayAbilitySpecHandle& Handle);

protected:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TMap<EWeaponType, FWeaponData> WeaponSlots;

	UPROPERTY()
	EWeaponType CurrentWeaponType = EWeaponType::None;

	UPROPERTY()
	TMap<EWeaponType, AFRWeaponBase*> WeaponActorCache;

	UPROPERTY()
	TObjectPtr<AFRWeaponBase> CurrentWeaponActor;

	UPROPERTY()
	FGameplayAbilitySpecHandle AttackAbilityHandle;

	UPROPERTY()
	FGameplayAbilitySpecHandle SpecialAttackAbilityHandle;

	UPROPERTY()
	TObjectPtr<class AFRGASCharacterPlayer> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UFRHUDWidget> HUD;

private:
	FTimerHandle CrossHairCheckTimer;
	void CheckPushableTarget();

	UPROPERTY()
	TObjectPtr<AActor> LastHighlightedActor;
};