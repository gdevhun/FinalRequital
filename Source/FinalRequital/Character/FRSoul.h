// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/FRHighlightInterface.h"
#include "FRSoul.generated.h"

UCLASS()
class FINALREQUITAL_API AFRSoul : public ACharacter, public IFRHighlightInterface
{
	GENERATED_BODY()

public:
	AFRSoul();
	virtual void Highlight() override;
	virtual void UnHighlight() override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> VisualMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> VisualMesh2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> VisualMesh3;

	UFUNCTION(BlueprintCallable)
	void ActiveSecondVisualMesh();

	UFUNCTION(BlueprintCallable)
	void ActiveThirdVisualMesh();

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	bool bIsPushing = false;

private:
	bool bHasBouncedOnce = false;
	bool bIsActivatedSecondMesh = false;
	bool bIsActivatedThirdMesh = false;

};
