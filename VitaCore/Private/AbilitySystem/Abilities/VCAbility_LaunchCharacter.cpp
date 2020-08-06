// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCAbility_LaunchCharacter.h"

#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Base/VCAreaVolume.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilityTask_WaitGameplayEvent.h"
#include "AbilityTask_PlayMontageAndWait.h"
#include "VCCharacter.h"

void UVCAbility_LaunchCharacter::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::OnCommitAbility(Handle, ActorInfo, ActivationInfo);
	
	OwningCharacter = Cast<AVCCharacter>(ActorInfo->OwnerActor);
	if(OwningCharacter != nullptr)
	{
		OwningCharacter->SetGroundFriction(false);
		
		// Change gravity scale of the character
		if (bShouldChangeGravity != false)
		{
			InitialGravity = OwningCharacter->GetCharacterMovement()->GravityScale;
			SetOwnerGravity(LaunchGravity);
			FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([=]()
			{
				SetOwnerGravity(InitialGravity);
			});
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_LaunchGravity, TimerDelegate, LaunchGravityDuration, false);
		}

		// Convert local launch direction to world according to character orientation and launch character
		LaunchDirection.Normalize();
		const FRotator CharacterRotation = OwningCharacter->GetActorRotation();
		const FVector LaunchVector = CharacterRotation.RotateVector(LaunchDirection);
		OwningCharacter->LaunchCharacter(LaunchVector * LaunchForce, true, true);
	}
}

void UVCAbility_LaunchCharacter::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	UVCAbility_AreaEffect::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if(OwningCharacter != nullptr)
	{	
		OwningCharacter->SetGroundFriction(true);
	}
}

void UVCAbility_LaunchCharacter::OnAnimationEnded()
{
	if (bShouldChangeGravity != false)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_LaunchGravity);
		SetOwnerGravity(InitialGravity);
	}
	UVCAbility_AreaEffect::OnAnimationEnded();
}


void UVCAbility_LaunchCharacter::SetOwnerGravity(float NewGravity)
{
	if(OwningCharacter != nullptr)
	{
		OwningCharacter->GetCharacterMovement()->GravityScale = NewGravity;
	}
}
