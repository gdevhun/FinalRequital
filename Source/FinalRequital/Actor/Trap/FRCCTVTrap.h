// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "FRTrapIntervalHelper.h"
#include "GameFramework/Actor.h"
#include "FRCCTVTrap.generated.h"

UCLASS()
class FINALREQUITAL_API AFRCCTVTrap : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AFRCCTVTrap();

protected:
	virtual void BeginPlay() override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USceneComponent> RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> CCTVMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USpotLightComponent> SpotLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> StunEffectClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float StartDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DetectionAngleDegrees = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float DetectionRayLength = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	int32 NumDetectionRays = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	ETrapIntervalType TrapIntervalType = ETrapIntervalType::Interval_3;

	// Pitch rotation 각도는 -90이 아래를 보는 기준각임.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float MinPitch = -60.0f;
	// Pitch rotation 각도는 -90이 아래를 보는 기준각임.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float MaxPitch = -120.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TObjectPtr<USoundBase> HitSound;
private:
	void DetectByLineTrace();
	FTimerHandle TraceTimerHandle;

};
