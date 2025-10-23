// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FRPlayerController.generated.h"

class UFRStackBaseWidget;
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
	virtual void SetupInputComponent() override;

	//HUD SECTION
public:
	void InitializePlayerStateStatus() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	FORCEINLINE class UFRHUDWidget* GetHUDWidget() { return HUDWidget; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UFRHUDWidget> FRHUDWidgetClass; // HUD UI 위젯 BP REF
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UFRHUDWidget> HUDWidget; // HUD UI 포인터

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
	TSubclassOf<class UFRMainMenuWidget> FRMainMenuWidgetClass; // HUD UI 위젯 BP REF
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UFRMainMenuWidget> MainMenuWidget; // MainMenu UI 포인터

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ESCKeyAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainMenu")
	bool bIsInteracting;

	UFUNCTION(BlueprintCallable)
	void SwitchToInspectCamera(AActor* TargetActor, float BlendTime, float ForwardOffset, float UpOffset);
	UFUNCTION(BlueprintCallable)
	void ReturnToPlayerCamera(float BlendTime);

	void HandleESCKeyPressed();

	UFUNCTION(BlueprintCallable)
	void PushUIWidgetToStack(UFRStackBaseWidget* NewWidget);

	UFUNCTION(BlueprintCallable)
	void PopUIWidgetFromStack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UFRStackBaseWidget>> UIWidgetStack;
};
