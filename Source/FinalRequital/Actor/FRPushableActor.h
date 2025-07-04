// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FRPushableActor.generated.h"

UCLASS()
class FINALREQUITAL_API AFRPushableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFRPushableActor();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> StaticMesh;

private:
	void DisablePhysics();
};
