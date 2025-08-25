// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FRLevelOpenHelper.generated.h"

UCLASS()
class FINALREQUITAL_API AFRLevelOpenHelper : public AActor
{
	GENERATED_BODY()
	
public:	
	AFRLevelOpenHelper();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CollisionBox;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OpenNextLevel();

protected:	
	UPROPERTY(EditAnywhere, Category="Level")
	FName NextLevelName;

};
