﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FRMonsterBase.h"
#include "GAS/Attribute/FRCharacterAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "FRDebugHelper.h"
#include "Physics/FRCollision.h"
#include "UI/FRWidgetComponent.h"
#include "UI/FRUserWidget.h"

AFRMonsterBase::AFRMonsterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_FRCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkMaxSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// ASC
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UFRCharacterAttributeSet>(TEXT("AttributeSet"));

	// HP UI
	HpBar = CreateDefaultSubobject<UFRWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (HpWidgetRef.Succeeded())
	{
		HpBar->SetWidgetClass(HpWidgetRef.Class); 
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(160.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Level = 1;
}
void AFRMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	/*if (HpWidgetClassRef)
	{
		HpBar->SetWidgetClass(HpWidgetClassRef);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		HpBar->InitWidget();
	}*/
	
}
class UAbilitySystemComponent* AFRMonsterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	ASC->InitAbilityActorInfo(this, this);
	AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);

	// [게임플레이 이펙트 생성 과정]
	// 게임플레이 이펙트 컨텍스트와 게임플레이 이펙트 스펙을 통해 생성 가능
	// 게임플레이 이펙트컨텍스트: GE에서 계산에 필요한 데이터를 담은 객체(가해자, 가해수단, 판정정보등)
	// 게임플레이 이벤트스펙: GE관련 정보를 담는 객체(레벨, 모데파이어, 태그 정보, 게임플레이 이펙트 컨텍스트 핸들)
	// ASC는 각 데이터 핸들 객체를 통해 간접 관리
	// -> 따라서 이펙트 컨텍스트 핸들을 만들고 이펙스 스펙 핸들을 생성하는 순서로 진행되어야함.

	// ASC가 초기화될 때 이펙트를 발생시키는 코드로직섹션
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		// GA발동하지 않고 이펙트를 스스로 발동시켜 스탯을 변경함.
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}

}

void AFRMonsterBase::OnOutOfHealth()
{
	SetDead();
}

void AFRMonsterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AFRMonsterBase::PlayDeadAnimation() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

