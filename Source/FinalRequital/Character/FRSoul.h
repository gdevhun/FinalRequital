// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FRSoul.generated.h"

UCLASS()
class FINALREQUITAL_API AFRSoul : public ACharacter
{
	GENERATED_BODY()

public:
	AFRSoul();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> VisualMesh;
};
