// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_EffectAroundPushback.h"
#include "AbilitySystemComponent.h"
#include "VCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UVCAbility_EffectAroundPushback::UVCAbility_EffectAroundPushback()
{
	RelativePushbackMagnitude = 100.0f;
	TagsToPreventPushback.AddTag(FGameplayTag::RequestGameplayTag("Status.Dead"));
	TagsToPreventPushback.AddTag(FGameplayTag::RequestGameplayTag("Status.PushbackImmune"));
	TagsToPreventPushback.AddTag(FGameplayTag::RequestGameplayTag("Status.Immune"));
}

void UVCAbility_EffectAroundPushback::OnEffectApplied(AActor* EffectedActor)
{
	Super::OnEffectApplied(EffectedActor);
	if (AVCCharacter* Character = Cast<AVCCharacter>(EffectedActor))
	{
		// Check if character can be moved
		UAbilitySystemComponent* CharacterAbilityComponent = Character->GetAbilitySystemComponent();
		if (CharacterAbilityComponent != nullptr && CharacterAbilityComponent->HasAnyMatchingGameplayTags(TagsToPreventPushback) == false)
		{
			// Apply pushback to character from it's movement component
			FVector DirectionVector = EffectedActor->GetActorLocation() - GetCurrentActorInfo()->OwnerActor->GetActorLocation();
			DirectionVector.Z = 0;
			DirectionVector.Normalize();
			Character->GetCharacterMovement()->AddImpulse(RelativePushbackMagnitude * DirectionVector + 
														  VerticalPushbackMagnitude * GetCurrentActorInfo()->OwnerActor->GetActorUpVector(),true);
		}
	}

	// TODO Apply Pushback to actors

}
