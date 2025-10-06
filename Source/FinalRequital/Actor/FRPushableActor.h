// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/FRHighlightInterface.h"
#include "FRPushableActor.generated.h"

UCLASS()
class FINALREQUITAL_API AFRPushableActor : public AActor, public IFRHighlightInterface
{
	GENERATED_BODY()
	
public:	
	AFRPushableActor();
	virtual void Highlight() override;
	virtual void UnHighlight() override;

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNavModifierComponent> NavModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTutorialObj = false;

private:
	void DisablePhysics();
};
