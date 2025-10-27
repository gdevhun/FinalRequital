// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "Actor/Trap/FRTrapIntervalHelper.h"
#include "FRFloorTrap.generated.h"

UCLASS()
class FINALREQUITAL_API AFRFloorTrap : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AFRFloorTrap();

protected:
	virtual void BeginPlay() override;

protected:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap")
	TObjectPtr<class UStaticMeshComponent> TrapMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap")
	TObjectPtr<class UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap")
	TObjectPtr<class UParticleSystemComponent> ParticleSystem;

	UPROPERTY(BlueprintReadOnly, Category = "Trap")
	TArray<TObjectPtr<class ACharacter>> OverlappingCharacters;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> PlayerDamageEffectClass;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> MonsterDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayAbility> PlayerHitAbilityClass;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<class UGameplayAbility> MonsterHitAbilityClass;
public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	float TrapActiveInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	ETrapIntervalType TrapIntervalType = ETrapIntervalType::Interval_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float StartDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USoundBase> HitSound;

private:
	void ToggleTrapActive();

private:
	bool bIsTrapActive = false;
	FTimerHandle ToggleActiveTimer;

};
