// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAbilitySystemComponent.h"

#include "Components/InputComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "GameFramework/PlayerController.h"

UVCAbilitySystemComponent::UVCAbilitySystemComponent()
{
}

void UVCAbilitySystemComponent::BindVCAbilityActivationToInputComponent(UInputComponent* PlayerInputComponent,
	UEnum* EnumBinds)
{
	
	if(PlayerInputComponent == nullptr || EnumBinds == nullptr )
	{
		return;
	}
	bIsNetDirty = true;
	BlockedAbilityBindings.SetNumZeroed(EnumBinds->NumEnums());
	
	for (int32 idx = 0; idx < EnumBinds->NumEnums(); ++idx)
	{
		const FString FullStr = EnumBinds->GetMetaData(TEXT("Input"), idx);
		// Pressed event
		{
			FInputActionBinding AB(FName(*FullStr), IE_Pressed);
			AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::AbilityLocalInputPressed, idx);
			PlayerInputComponent->AddActionBinding(AB);
		}
		// Released event
		{
			FInputActionBinding AB(FName(*FullStr), IE_Released);
			AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::AbilityLocalInputReleased, idx);
			PlayerInputComponent->AddActionBinding(AB);
		}
	}
	
	{
		FInputActionBinding AB(FName(*FString("ConfirmInput")), IE_Pressed);
		AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::LocalInputConfirm);
		PlayerInputComponent->AddActionBinding(AB);
	}

	{
		FInputActionBinding AB(FName(*FString("CancelInput")), IE_Pressed);
		AB.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &UAbilitySystemComponent::LocalInputCancel);
		PlayerInputComponent->AddActionBinding(AB);
	}

	GenericCancelInputID = 0;
	GenericConfirmInputID = 0;
}

void UVCAbilitySystemComponent::ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag,
	const FGameplayCueParameters& GameplayCueParameters) const
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Executed, GameplayCueParameters);
}	

void UVCAbilitySystemComponent::AddGameplayCueLocal(const FGameplayTag GameplayCueTag,
	const FGameplayCueParameters& GameplayCueParameters) const
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::OnActive, GameplayCueParameters);
}	

void UVCAbilitySystemComponent::RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag,
	const FGameplayCueParameters& GameplayCueParameters) const
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Removed, GameplayCueParameters);

}

