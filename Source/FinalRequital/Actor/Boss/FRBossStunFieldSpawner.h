// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FRBossStunFieldSpawner.generated.h"

UCLASS()
class FINALREQUITAL_API AFRBossStunFieldSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	AFRBossStunFieldSpawner();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void SpawnStunFields(AActor* TargetPlayer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|StunField")
	int32 FieldCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|StunField")
	float MinDistanceFromPlayer = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|StunField")
	float MaxDistanceFromPlayer = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|StunField")
	float FieldLifetime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|StunField")
	class UParticleSystem* WarningEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss|StunField")
	TSubclassOf<AActor> StunFieldActorClass;

protected:
	FVector GetRandomLocationAroundPlayer(const FVector& PlayerLocation);

	void SpawnSingleField(const FVector& Location);

	UPROPERTY()
	TArray<AActor*> ActiveFields;
};
