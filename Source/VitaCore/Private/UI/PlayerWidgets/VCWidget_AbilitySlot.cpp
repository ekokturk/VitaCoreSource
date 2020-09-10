// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCWidget_AbilitySlot.h"

#include "Image.h"
#include "Overlay.h"
#include "ProgressBar.h"
#include "TextBlock.h"
#include "VCGameplayAbility.h"

bool UVCWidget_AbilitySlot::Initialize()
{
	const bool bResult = Super::Initialize();
	if (bResult == false)
	{
		return false;
	}

	if(Icon_Image)
	{
		Icon_Image->SetVisibility(ESlateVisibility::Hidden);
	}

	if(Cost_TextBlock)
	{
		Cost_TextBlock->SetVisibility(ESlateVisibility::Hidden);
	}

	return true;
}


void UVCWidget_AbilitySlot::InitAbilitySlot(UVCGameplayAbility* Ability)
{
	SlottedAbility = Ability;
	if(SlottedAbility)
	{
		SetAbilityCost();
		if(Ability->GetIconTexture())
		{
			Icon_Image->SetVisibility(ESlateVisibility::Visible);
			Icon_Image->SetBrushFromTexture(Ability->GetIconTexture());
		}
		AbilityInvalid_Overlay->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		AbilityInvalid_Overlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void UVCWidget_AbilitySlot::SetAbilityCost() const
{
	const UGameplayEffect* CostEffect = SlottedAbility->GetCostGameplayEffect();
	if (CostEffect != nullptr)
	{
		float Cost = 0;
		if (CostEffect->Modifiers.Num() > 0)
		{
			const FGameplayModifierInfo EffectInfo = CostEffect->Modifiers[0];
			EffectInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);	
		}
		
		Cost_TextBlock->SetVisibility(ESlateVisibility::Visible);
		(Cost != 0) ?	Cost_TextBlock->SetText(FText::AsNumber(static_cast<int32>(-Cost))) :
						Cost_TextBlock->SetText(FText::GetEmpty());
	}
}

void UVCWidget_AbilitySlot::SetAbilityCooldownPercent()
{
	if (SlottedAbility && SlottedAbility->GetCooldownGameplayEffect())
	{
		float Duration = 1.f;
		float Remaining = 0.f;
		SlottedAbility->GetCooldownTimeRemainingAndDuration(FGameplayAbilitySpecHandle(), SlottedAbility->GetCurrentActorInfo(), Remaining, Duration);
		Cooldown_ProgressBar->SetPercent((Remaining / Duration));
	}
}

void UVCWidget_AbilitySlot::TriggerCooldown()
{
	if(SlottedAbility )
	{
		if (UWorld* World = GetWorld())
		{
			FTimerManager& TimerManager = World->GetTimerManager();
			Cooldown_ProgressBar->SetPercent(1.f);
			TimerManager.ClearTimer(TimerHandle_Cooldown);
			TimerManager.SetTimer(TimerHandle_Cooldown, this, &UVCWidget_AbilitySlot::SetAbilityCooldownPercent, CooldownStep, true);
		}
	}

}

void UVCWidget_AbilitySlot::ClearCooldown()
{
	if(UWorld * World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_Cooldown);
	}
	Cooldown_ProgressBar->SetPercent(0.f);
	
}
