// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/FRAT_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UFRAT_JumpAndWaitForLanding::UFRAT_JumpAndWaitForLanding()
{
}

UFRAT_JumpAndWaitForLanding* UFRAT_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UFRAT_JumpAndWaitForLanding* NewTask = NewAbilityTask<UFRAT_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UFRAT_JumpAndWaitForLanding::Activate()
{
	Super::Activate();
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UFRAT_JumpAndWaitForLanding::OnLandedCallback);
	Character->Jump();

	SetWaitingOnAvatar();
}

void UFRAT_JumpAndWaitForLanding::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UFRAT_JumpAndWaitForLanding::OnLandedCallback);
}

void UFRAT_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}
}
