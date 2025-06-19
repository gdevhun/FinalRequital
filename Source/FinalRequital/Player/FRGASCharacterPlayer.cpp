// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FRGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/FRPlayerState.h"
#include "EnhancedInputComponent.h"
#include "FRDebugHelper.h"
#include "FRGameplayTag.h"
#include "FRWeaponComponent.h"
#include "UI/FRWidgetComponent.h"
#include "FRMaskSkillComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AFRGASCharacterPlayer::AFRGASCharacterPlayer()
{
	ASC = nullptr;

	HpBar = CreateDefaultSubobject<UFRWidgetComponent>(TEXT("Widget"));
	WeaponComponent = CreateDefaultSubobject<UFRWeaponComponent>(TEXT("WeaponComponent"));
	MaskSkillComponent = CreateDefaultSubobject<UFRMaskSkillComponent>(TEXT("MaskSkillComponent"));
}

void AFRGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AFRPlayerState* GASPS = GetPlayerState<AFRPlayerState>();

	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);

		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			ASC->GiveAbility(StartSpec);
		}
		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}

		SetupGASInputComponent();
	}
}

void AFRGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetupGASInputComponent();
}

void AFRGASCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(FRTAG_CHARACTER_STUNNED, EGameplayTagEventType::NewOrRemoved)
			.AddUObject(this, &AFRGASCharacterPlayer::OnStunTagChanged);
	}
}

void AFRGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFRGASCharacterPlayer::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::GASInputPressed, 1);
		EnhancedInputComponent->BindAction(SpecialAttackAction, ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::GASInputPressed, 2);
		EnhancedInputComponent->BindAction(QuickSlotActions[0], ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::QuickSlot1);
		EnhancedInputComponent->BindAction(QuickSlotActions[1], ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::QuickSlot2);
		EnhancedInputComponent->BindAction(QuickSlotActions[2], ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::QuickSlot3);
		EnhancedInputComponent->BindAction(QuickSlotActions[3], ETriggerEvent::Triggered, this, &AFRGASCharacterPlayer::QuickSlot4);
	}
}

void AFRGASCharacterPlayer::HandleQuickSlot(EWeaponType WeaponType)
{
	if (ASC && ASC->HasMatchingGameplayTag(FRTAG_CHARACTER_STUNNED))
	{
		return;
	}

	if (WeaponComponent)
	{
		WeaponComponent->EquipWeapon(WeaponType);
	}
}

void AFRGASCharacterPlayer::QuickSlot1()
{
	HandleQuickSlot(EWeaponType::Sword);
}

void AFRGASCharacterPlayer::QuickSlot2()
{
	HandleQuickSlot(EWeaponType::Bow);
}

void AFRGASCharacterPlayer::QuickSlot3()
{
	HandleQuickSlot(EWeaponType::IronMace);
}

void AFRGASCharacterPlayer::QuickSlot4()
{
	HandleQuickSlot(EWeaponType::BronzeBell);
}

void AFRGASCharacterPlayer::OnStunTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		GetCharacterMovement()->DisableMovement();
		//bUseControllerRotationYaw = false;
		bIsStunned = true;

		if (StunMontage)
		{
			PlayAnimMontage(StunMontage, 1.0f);
		}
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bIsStunned = false;
	}
}

void AFRGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	if (ASC && ASC->HasMatchingGameplayTag(FRTAG_CHARACTER_STUNNED))
	{
		return;
	}

	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AFRGASCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;

		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

UAbilitySystemComponent* AFRGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AFRGASCharacterPlayer::AdjustBronzeBellLayerAnim_Implementation() {}
void AFRGASCharacterPlayer::AdjustBowLayerAnim_Implementation() {}
void AFRGASCharacterPlayer::AdjustUnarmedLayerAnim_Implementation() {}
void AFRGASCharacterPlayer::AdjustMeleeLayerAnim_Implementation() {}