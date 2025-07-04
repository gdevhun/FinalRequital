// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FRPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FINALREQUITAL_API AFRPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFRPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	//virtual void SetupInputComponent() override;

	//HUD SECTION
public:
	FORCEINLINE class UFRHUDWidget* GetHUDWidget() { return AFRHUDWidget; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UFRHUDWidget> FRHUDWidgetClass; // 플레이어에게 보일 HUD UI 위젯 BP

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UFRHUDWidget> AFRHUDWidget; // HUD UI 포인터
};
