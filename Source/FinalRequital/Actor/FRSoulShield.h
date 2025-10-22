// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "FRSoulShield.generated.h"

UCLASS()
class FINALREQUITAL_API AFRSoulShield : public AActor,public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	AFRSoulShield();

protected:
	
	virtual void BeginPlay() override;

    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UAbilitySystemComponent> ASC;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    TSubclassOf<class UGameplayEffect> HpDamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    TSubclassOf<class UGameplayEffect> ShieldDamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "GAS")
    TSubclassOf<class UFRSoulShieldAttributeSet> AttributeSetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UParticleSystem> ParticleEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USoundBase> HitSound;

    UPROPERTY()
    TObjectPtr<UFRSoulShieldAttributeSet> AttributeSet;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class USphereComponent> DamageDetectSphere;

    UFUNCTION(BlueprintNativeEvent)
    void OnOutOfHealth();
    virtual void OnOutOfHealth_Implementation();

    bool IsShieldDestroyed = false;

    UFUNCTION()
	FORCEINLINE void DestroyShield() { IsShieldDestroyed = true; }
};
