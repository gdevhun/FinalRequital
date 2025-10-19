// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FRMonsterBase.h"
#include "GAS/Attribute/FRMonsterAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "FRDebugHelper.h"
#include "Physics/FRCollision.h"
#include "UI/FRWidgetComponent.h"
#include "UI/FRUserWidget.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/SphereComponent.h"
#include "Player/FRGASCharacterPlayer.h"

AFRMonsterBase::AFRMonsterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Level = 1;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(28.f, 60.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_FRMONSTER);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(CCHANNEL_FRACTION, ECR_Block);

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
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -55.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(250);

	// HpBar UI Detect Sphere
	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetupAttachment(RootComponent);
	DetectSphere->InitSphereRadius(400.0f);
	DetectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	

	// ASC
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UFRMonsterAttributeSet>(TEXT("MonsterAttributeSet"));

	// HPBar UI
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
}
void AFRMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	HpBar->SetVisibility(false);

	if (DetectSphere)
	{
		DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AFRMonsterBase::OnSphereBeginOverlap);
		DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AFRMonsterBase::OnSphereEndOverlap);
	}

}
class UAbilitySystemComponent* AFRMonsterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);

	for (int32 i = 0; i < StartAbilities.Num(); ++i)
	{
		const TSubclassOf<UGameplayAbility>& StartAbility = StartAbilities[i];
		FGameplayAbilitySpec StartSpec(StartAbility);

		FGameplayAbilitySpecHandle GivenHandle = ASC->GiveAbility(StartSpec);

		// 0번 인덱스는 HitReact 따로 저장
		if (i == 0)
		{
			HitReactAbilityHandle = GivenHandle;
		}
	}

	// 몬스터 체력 관련 DELEGATE 연결 처리
	if (AttributeSet)
	{
		AttributeSet->OnMonsterOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
		AttributeSet->OnMonsterTakeDamage.AddDynamic(this, &ThisClass::HitReact);
	}
	
	//AttributeSet->OnMonsterOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
	//AttributeSet->OnMonsterTakeDamage.AddDynamic(this, &ThisClass::HitReact);

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

void AFRMonsterBase::Highlight()
{
	GetMesh()->SetRenderCustomDepth(true);
}

void AFRMonsterBase::UnHighlight()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void AFRMonsterBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AFRGASCharacterPlayer::StaticClass()))
	{
		if (HpBar)
		{
			GetWorld()->GetTimerManager().ClearTimer(HpBarTimerHandle);
			HpBar->SetVisibility(true);
		}
	}
}

void AFRMonsterBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(AFRGASCharacterPlayer::StaticClass()))
	{
		if (HpBar)
		{
			HpBar->SetVisibility(false);
		}
	}
}

void AFRMonsterBase::OnOutOfHealth()
{
	SetDead();
}

void AFRMonsterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayAnimMontage(DeadMontage);
	SetActorEnableCollision(false);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (AIController->BrainComponent)
		{
			AIController->BrainComponent->StopLogic(TEXT("Monster Died"));
		}
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AFRMonsterBase::HitReact()
{
	if (ASC && HitReactAbilityHandle.IsValid())
	{
		ASC->TryActivateAbility(HitReactAbilityHandle);

		if (HpBar)
		{
			HpBar->SetVisibility(true);
			GetWorld()->GetTimerManager().ClearTimer(HpBarTimerHandle);

			// 플레이어가 범위 밖에 있으면 1.5초 후 숨김
			TArray<AActor*> OverlappingActors;
			DetectSphere->GetOverlappingActors(OverlappingActors, AFRGASCharacterPlayer::StaticClass());

			if (OverlappingActors.Num() == 0)
			{
				GetWorld()->GetTimerManager().SetTimer(
					HpBarTimerHandle,
					FTimerDelegate::CreateLambda([this]()
						{
							if (HpBar)
							{
								HpBar->SetVisibility(false);
							}
						}),
					1.5f,
					false
				);
			}
		}
	}
}

void AFRMonsterBase::ApplyLevelEffect(int InLevel)
{
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, InLevel, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AFRMonsterBase::PlayAnimMontage(UAnimMontage* Montage) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(Montage, 1.0f);
}

