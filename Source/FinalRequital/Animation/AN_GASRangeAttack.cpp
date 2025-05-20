// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN_GASRangeAttack.h"
#include "AbilitySystemBlueprintLibrary.h"

UAN_GASRangeAttack::UAN_GASRangeAttack()
{
}

FString UAN_GASRangeAttack::GetNotifyName_Implementation() const
{
	return TEXT("GASRangeAttackHit");
}

void UAN_GASRangeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AActor* OwnerActor = MeshComp->GetOwner();
		if (OwnerActor)
		{
			// ���̷ε嵥���Ϳ� �޺����÷��뿡 ���� ������ �߰�
			FGameplayEventData PayloadData;

			// ���Գ�Ƽ���̿��� ������ Ʈ���Ű����÷����±׸� ���ؼ� �ڵ����� Activate�� ȣ���ϰԵ�. MeleeAttackHitCheck
			// ���̷ε嵥���͸� �Բ� ����.
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, TriggerGameplayTag, PayloadData);
		}
	}
}

