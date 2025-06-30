// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
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

	/*UPROPERTY(EditAnywhere, Category = "Trap")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;*/

};
