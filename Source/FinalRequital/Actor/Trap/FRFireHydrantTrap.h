// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "FRFireHydrantTrap.generated.h"

UCLASS()
class FINALREQUITAL_API AFRFireHydrantTrap : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AFRFireHydrantTrap();

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

	UPROPERTY(EditAnywhere, Category = "Trap")
	TSubclassOf<class UGameplayEffect> PlayerDamageEffectClass;

	UPROPERTY(EditAnywhere, Category = "Trap")
	TSubclassOf<class UGameplayEffect> MonsterDamageEffectClass;

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bIsTrapActive = false;

	void ToggleTrapActive();
	void DeactivateTrap();
	void TriggerTrap();
	void ApplyDamageEffect();

private:
	FTimerHandle ToggleActiveTimer;
	FTimerHandle DamageTickTimer;

	UPROPERTY(EditAnywhere, Category = "Trap")
	float TrapCooldownTime = 3.0f;

};
