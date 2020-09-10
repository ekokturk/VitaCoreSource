// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "VCAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class VITACORE_API UVCAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UVCAbilitySystemComponent();

	// This method is an overloaded version of "BindAbilityActivationToInputComponent(UInputComponent* InputComponent, FGameplayAbilityInputBinds BindInfo)"
	// It only binds ability enum and does not process target and blocked input
	// Additionally it binds value of "Input" meta data of the enum instead therefore abilities can be referenced with different name
	virtual void BindVCAbilityActivationToInputComponent(UInputComponent* PlayerInputComponent, UEnum* EnumBinds);

	UFUNCTION() void ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters) const;

	UFUNCTION() void AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters) const;

	UFUNCTION() void RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters) const;

};
