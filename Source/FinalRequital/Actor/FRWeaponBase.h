// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FRWeaponBase.generated.h"


enum class EWeaponType : uint8;

UCLASS()
class FINALREQUITAL_API AFRWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
	AFRWeaponBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EWeaponType WeaponType;

	void SetVisible(bool bVisible);

};
