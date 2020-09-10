// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbility_MeleeHit.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_WaitGameplayTag.h"
#include "TimerManager.h"
#include "VCGameLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VCCharacter.h"
#include "VCMeleeComponent.h"
#include "AbilityTask_PlayMontageAndWait.h"

UVCAbility_MeleeHit::UVCAbility_MeleeHit():
	CurrentMeleeInfo(FVCAbility_MeleeHitData())
{
	//bShouldEndAbilityWithAnimation = true;
	//bShouldAnimationStopWhenAbilityEnds = true;
	TagsToPreventMeleePushback.AddTag(FGameplayTag::RequestGameplayTag("Status.Dead"));
	TagsToPreventMeleePushback.AddTag(FGameplayTag::RequestGameplayTag("Status.PushbackImmune"));
	TagsToPreventMeleePushback.AddTag(FGameplayTag::RequestGameplayTag("Status.Immune"));

	
	if(MeleeComponentInfo.Num() == 0)
	{
		MeleeComponentInfo.Add(FVCAbility_MeleeHitData());
	}
}

void UVCAbility_MeleeHit::OnCommitAbility(const FGameplayAbilitySpecHandle Handle,
            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UVCGameplayAbility::OnCommitAbility(Handle, ActorInfo, ActivationInfo);

	// Get a random melee component from melee component info
	CurrentMeleeInfo = MeleeComponentInfo.Num() > 0 ?
		MeleeComponentInfo[FMath::RandHelper(MeleeComponentInfo.Num())] : FVCAbility_MeleeHitData();

	if(CurrentMeleeInfo.IsValid() == false)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		// Get a random section if it is not specified
		const int32 SectionCount = CurrentMeleeInfo.Animation->CompositeSections.Num();
		const FName SecionName = CurrentMeleeInfo.MontageSectionName == NAME_None && SectionCount > 0 ?
			CurrentMeleeInfo.Animation->CompositeSections[FMath::RandHelper(SectionCount)].SectionName : CurrentMeleeInfo.MontageSectionName;
		
		Task_PlayMontageAndWaitStart = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
			FName(TEXT("None")), CurrentMeleeInfo.Animation, 1.0f, SecionName, false);
		Task_PlayMontageAndWaitStart->OnInterrupted.AddDynamic(this, &UVCAbility_MeleeHit::EndCurrentAbility);
		Task_PlayMontageAndWaitStart->OnCancelled.AddDynamic(this, &UVCAbility_MeleeHit::EndCurrentAbility);
		Task_PlayMontageAndWaitStart->OnBlendOut.AddDynamic(this, &UVCAbility_MeleeHit::EndCurrentAbility);
		Task_PlayMontageAndWaitStart->OnCompleted.AddDynamic(this, &UVCAbility_MeleeHit::EndCurrentAbility);
		Task_PlayMontageAndWaitStart->ReadyForActivation();
		
	}

	if (HasAuthority(&GetCurrentActivationInfoRef()) == false)
	{
		return;
	}

	if (MeleeHitEffect != nullptr)
	{
		MeleeHitEffectHandle = MakeOutgoingGameplayEffectSpec(MeleeHitEffect, GetOwnerAbilityLevel());
	}
	
	if(bShouldActivateMeleeWithEventTag == true)
	{
		Task_WaitMeleeTagAdded = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, MeleeActiveTag, nullptr, true);
		Task_WaitMeleeTagAdded->Added.AddDynamic(this, &UVCAbility_MeleeHit::OnMeleeStart);
		Task_WaitMeleeTagAdded->ReadyForActivation();
	}
	else
	{
		if(MeleeActivationInitalDelay > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_MeleeActivationInitialDelay,this,  &UVCAbility_MeleeHit::OnMeleeStart, MeleeActivationInitalDelay, false, 0.0f);
		}
		else
		{
			OnMeleeStart();
		}
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_MeleeActivationDuration, this, &UVCAbility_MeleeHit::OnMeleeEnd, MeleeActivationDuration, false, MeleeActivationInitalDelay);
	}
}

void UVCAbility_MeleeHit::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if(HasAuthority(&GetCurrentActivationInfoRef()) == true)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			World->GetTimerManager().ClearTimer(TimerHandle_MeleeActivationDuration);
			World->GetTimerManager().ClearTimer(TimerHandle_MeleeActivationInitialDelay);
		}
		// Check if animation has
		if(CurrentMeleeInfo.IsValid() && 
			(CurrentMeleeInfo.Animation->SequenceLength < MeleeActivationInitalDelay + MeleeActivationDuration
			|| MeleeActivationDuration <= 0.0f))
		{
			OnMeleeEnd();
		}
	}

	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UVCAbility_MeleeHit::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	// Cache Melee Components of the owner

	const TArray<UVCMeleeComponent*> OwnerMeleeComponents = GetAbilityActorComponents<UVCMeleeComponent>(ActorInfo);
	for(auto& ComponentInfo : MeleeComponentInfo)
	{
		for(auto & MeleeComponent : OwnerMeleeComponents)
		{
			if (MeleeComponent->ComponentHasTag(ComponentInfo.ComponentTag) ||	// Check is component is tagged
				(ComponentInfo.ComponentTag.IsNone() && MeleeComponent->ComponentTags.Num() == 0)) // Check if tag is not defined
			{
				MeleeComponent->OnMeleeHit.AddUObject(this, &UVCAbility_MeleeHit::OnMeleeHit);
				ComponentInfo.SetMeleeComponentRef(MeleeComponent);
				break;
			}
		}
		
	}
}


void UVCAbility_MeleeHit::OnMeleeHit(const FHitInfo HitInfo)
{
	if(HitInfo.TargetActor)
	{
		if(AVCCharacter * Character = Cast<AVCCharacter>(HitInfo.TargetActor))
		{
			// Check if character can be moved
			UAbilitySystemComponent * CharacterAbilityComponent = Character->GetAbilitySystemComponent();
			if(CharacterAbilityComponent != nullptr && CharacterAbilityComponent->HasAnyMatchingGameplayTags(TagsToPreventMeleePushback) == false)
			{
				// Apply pushback to character from it's movement component
				const FVector DirectionVector = GetMeleeHitImpulseVector(Character->GetActorTransform());
				Character->GetCharacterMovement()->AddImpulse(HitForceMagnitude * DirectionVector,true);
			}
		}
		else
		{
			// TODO Change this with Hitable Interface
			UPrimitiveComponent* PrimitiveComponent = HitInfo.TargetActor->FindComponentByClass<UPrimitiveComponent>();
			if (PrimitiveComponent != nullptr && PrimitiveComponent->IsSimulatingPhysics())
			{
				const FVector DirectionVector = GetMeleeHitImpulseVector(PrimitiveComponent->GetComponentTransform());
				PrimitiveComponent->AddImpulse(HitForceMagnitude * DirectionVector);
			}
		}
	}

	if (IAbilitySystemInterface* TargetAbilitySystem = Cast<IAbilitySystemInterface>(HitInfo.TargetActor))
	{
		FGameplayEffectContextHandle Context = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
		Context.AddHitResult(HitInfo.ConvertToHitResult());
		const FGameplayEffectSpecHandle Handle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(MeleeHitEffect, GetOwnerAbilityLevel(), FGameplayEffectContextHandle(Context));
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*Handle.Data.Get(), TargetAbilitySystem->GetAbilitySystemComponent());
	}
}

void UVCAbility_MeleeHit::OnMeleeStart()
{

	if (CurrentMeleeInfo.IsValid())
	{
		CurrentMeleeInfo.MeleeComponentRef->SetOverlapActive(true);
		if (bShouldActivateMeleeWithEventTag == true)
		{
			Task_WaitMeleeTagRemoved = UAbilityTask_WaitGameplayTagRemoved::WaitGameplayTagRemove(this, MeleeActiveTag, nullptr, true);
			Task_WaitMeleeTagRemoved->Removed.AddDynamic(this, &UVCAbility_MeleeHit::OnMeleeEnd);
			Task_WaitMeleeTagRemoved->ReadyForActivation();
		}
	}
}

void UVCAbility_MeleeHit::OnMeleeEnd()
{
	if (CurrentMeleeInfo.IsValid())
	{
		CurrentMeleeInfo.MeleeComponentRef->SetOverlapActive(false);
		CurrentMeleeInfo.MeleeComponentRef->ClearOverlappedActors();
		CurrentMeleeInfo = FVCAbility_MeleeHitData();
	}
}

FVector UVCAbility_MeleeHit::GetMeleeHitImpulseVector(const FTransform& TargetTransform) const
{
	FVector DirectionVector = IsMeleePushbackRelative ?  
		TargetTransform.GetLocation() - GetCurrentActorInfo()->OwnerActor.Get()->GetActorLocation()	:
		GetCurrentActorInfo()->OwnerActor.Get()->GetActorForwardVector();
	DirectionVector.Normalize();
	DirectionVector.Z = VerticalPushbackOverride < -1 ? DirectionVector.Z : VerticalPushbackOverride;
	return DirectionVector;
}
