// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "FRSoulPiece.generated.h"

UCLASS()
class FINALREQUITAL_API AFRSoulPiece : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AFRSoulPiece();

	UFUNCTION()
	void OnDetectedOnce();

protected:
	virtual void BeginPlay() override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> RootSceneComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystemComponent> FXComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> AudioComponent;

	bool bIsDetected;
};
