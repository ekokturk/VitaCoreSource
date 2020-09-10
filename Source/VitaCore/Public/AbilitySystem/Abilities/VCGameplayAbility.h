// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "VCGameTypes.h"
#include "VCAbilitySystemInterface.h"
#include "VCGameplayAbility.generated.h"


class UTexture2D;
class AVCCharacter;
class IVCEntityInterface;

/*
 * This is the base class for VitaCore Abilities that each ability must inherit from
 */

UCLASS()
class VITACORE_API UVCGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UVCGameplayAbility();

	// TODO Remove 
	UFUNCTION(BlueprintCallable)
		AVCCharacter * GetOwnerCharacter();

	// Get Level of the VitaCore Ability Actor
	UFUNCTION(BlueprintCallable)
		int GetOwnerAbilityLevel() const;

	UFUNCTION()
		void EndCurrentAbility();

	UFUNCTION() UTexture2D* GetIconTexture() const { return IconTexture; }
	
protected:
	UPROPERTY(EditAnywhere, Category = "VCGameplayAbility")
		FGameplayTagContainer ExecutableCuesOnCommit;

	UPROPERTY(EditAnywhere, Category = "VCGameplayAbility")
		FGameplayTagContainer ActivatableCuesOnCommit;
	
	// VitaCore Ability system reference
	UPROPERTY()
		TScriptInterface<IVCAbilitySystemInterface> OwnerAbilitySystem;

	// VitaCore Entity reference
	UPROPERTY()
		TScriptInterface<IVCEntityInterface> OwnerEntity;

	// Get affiliation of the owner against another actor
	UFUNCTION()
		EAffiliationType GetAffiliation(AActor * OtherActor) const;

	UPROPERTY(EditAnywhere, Category = "VCGameplayAbility")
		UTexture2D* IconTexture;

	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	// Called when ability is successfully committed 
	virtual void OnCommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
	
	// Get Actor component of an ability actor either by tag or class
	template<typename T>
	TArray<T*> GetAbilityActorComponents(const FGameplayAbilityActorInfo* ActorInfo, FName ComponentTag = NAME_None)
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UActorComponent>::Value, "'T' template parameter to GetAbilityActorComponents must be derived from UActorComponent");

		TArray<T*> Components;
		if (ActorInfo->OwnerActor != nullptr)
		{
			if (ComponentTag != NAME_None)
			{
				TArray<T*> CachedComponents;
				ActorInfo->OwnerActor->GetComponents<T>(CachedComponents);
				for (T* TaggedComponent : CachedComponents)
				{
					if(TaggedComponent->ComponentHasTag(ComponentTag) == true)
					{
						Components.Emplace(TaggedComponent);
					}
				}
			}
			else
			{
				ActorInfo->OwnerActor->GetComponents<T>(Components);
			}
		}
		return Components;
	}
	
};
