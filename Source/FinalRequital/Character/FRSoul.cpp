// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FRSoul.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AFRSoul::AFRSoul()
{
	PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->BodyInstance.bNotifyRigidBodyCollision = true;
    GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(GetCapsuleComponent());
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    VisualMesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh2"));
    VisualMesh2->SetupAttachment(GetCapsuleComponent());
    VisualMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    VisualMesh3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh3"));
    VisualMesh3->SetupAttachment(GetCapsuleComponent());
    VisualMesh3->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    VisualMesh->SetRenderCustomDepth(false);
    GetMesh()->SetCustomDepthStencilValue(250);
    VisualMesh2->SetRenderCustomDepth(false);
    GetMesh()->SetCustomDepthStencilValue(250);
    VisualMesh3->SetRenderCustomDepth(false);
    GetMesh()->SetCustomDepthStencilValue(250);
}

void AFRSoul::Highlight()
{
    VisualMesh->SetRenderCustomDepth(true);
    if (bIsActivatedSecondMesh)  VisualMesh2->SetRenderCustomDepth(true);
	if (bIsActivatedThirdMesh)  VisualMesh3->SetRenderCustomDepth(true);
}

void AFRSoul::UnHighlight()
{
    VisualMesh->SetRenderCustomDepth(false);
    if (bIsActivatedSecondMesh)  VisualMesh2->SetRenderCustomDepth(false);
    if (bIsActivatedThirdMesh)  VisualMesh3->SetRenderCustomDepth(false);
}

void AFRSoul::BeginPlay()
{
	Super::BeginPlay();

    VisualMesh->SetVisibility(true);
    VisualMesh2->SetVisibility(false);
    VisualMesh3->SetVisibility(false);
}

void AFRSoul::ActiveSecondVisualMesh()
{
    VisualMesh2->SetVisibility(true);
    bIsActivatedSecondMesh = true;
}

void AFRSoul::ActiveThirdVisualMesh()
{
    VisualMesh3->SetVisibility(true);
    bIsActivatedThirdMesh = true;
}

void AFRSoul::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
    if (!bIsPushing || bHasBouncedOnce)
        return;

    bHasBouncedOnce = true;

    FVector Velocity = GetCharacterMovement()->Velocity;
    FVector Reflected = FMath::GetReflectionVector(Velocity, HitNormal);

    float Speed = Velocity.Size();
    FVector BounceDir = Reflected.GetSafeNormal();

    LaunchCharacter(BounceDir * Speed * 2.0f, true, true);

    // 반사 1회 후 리셋
    GetWorldTimerManager().SetTimerForNextTick([this]()
        {
            bIsPushing = false;
            bHasBouncedOnce = false;
        });
}


