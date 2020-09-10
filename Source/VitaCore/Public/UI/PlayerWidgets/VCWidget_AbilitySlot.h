// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "VCWidget_AbilitySlot.generated.h"

class UVCGameplayAbility;
class UImage;
class UProgressBar;
class UTextBlock;
class UOverlay;

UCLASS()
class VITACORE_API UVCWidget_AbilitySlot : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitAbilitySlot(UVCGameplayAbility* Ability);
	void TriggerCooldown();
	void ClearCooldown();

protected:
	// Ability Icon that is captured from ability instance
	UPROPERTY(meta = (BindWidget)) UImage* Icon_Image;

	// Ability cooldown bar
	UPROPERTY(meta = (BindWidget)) UProgressBar* Cooldown_ProgressBar;

	// Ability cost text
	UPROPERTY(meta = (BindWidget)) UTextBlock* Cost_TextBlock;

	
	UPROPERTY(meta = (BindWidget)) UOverlay* AbilityInvalid_Overlay;


	UPROPERTY(BlueprintReadWrite,EditAnywhere)
		float CooldownStep;
	
	// Initialize ability cost
	void SetAbilityCost() const;

	void SetAbilityCooldownPercent();

	FTimerHandle TimerHandle_Cooldown;
	
	// Cached gameplay ability
	UPROPERTY() UVCGameplayAbility* SlottedAbility;

protected:
	virtual bool Initialize() override;


};
