// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FRGameInstance.generated.h"

enum class EWeaponType : uint8;
/**
 * 
 */
UCLASS()
class FINALREQUITAL_API UFRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 무기 획득 상태를 기록하는 함수
	UFUNCTION(BlueprintCallable)
	void AcquireWeapon(EWeaponType WeaponType);

	// 무기 보유 여부 체크
	bool HasWeapon(EWeaponType WeaponType) const;

private:
	// 획득한 무기 목록
	UPROPERTY(EditAnywhere)
	TSet<EWeaponType> AcquiredWeapons;
};
