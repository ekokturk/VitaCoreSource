// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCHUD_Player.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ProgressBar.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/RetainerBox.h"
#include "UMG/Public/Components/Overlay.h"
#include "GameplayAbilitySpec.h"
#include "Image.h"
#include "VCHeroCharacter.h"
#include "VCPlayerController.h"
#include "VCEnergyAttributeSet.h"
#include "VCHealthAttributeSet.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VCWidget_AbilitySlot.h"
#include "VCGameplayAbility.h"
#include "VCGameState.h"
#include "VCTeam.h"
#include "VCTraitAttributeSet.h"


bool UVCHUD_Player::Initialize()
{
	const bool bResult = Super::Initialize();
	if (bResult == false)
	{
		return false;
	}

	NotifyDuration = 0.1f;
	return true;
}


void UVCHUD_Player::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Initialize HUD
	InitializeHUD();

	UpdateHealth();
	UpdateEnergy();
	UpdateFortitude();
	UpdateMastery();
	UpdateMovementSpeed();
	UpdateTeamLogo();
	
	// TODO Automate this after UI asset is generalized
	InitializeAbility(0, Ability1_SlotWidget);
	InitializeAbility(1, Ability2_SlotWidget);
	InitializeAbility(2, Ability3_SlotWidget);
	InitializeAbility(3, Ability4_SlotWidget);
	InitializeAbility(4, Ability5_SlotWidget);
}

void UVCHUD_Player::RemoveFromParent()
{
	// ------------ Delegate cleanup
	
	if(MyCharacter)
	{
		MyCharacter->OnTeamChanged.RemoveAll(this);
		MyCharacter->GetXPChangedDelegate().RemoveAll(this);
		MyCharacter->GetOnLevelChangedDelegate().RemoveAll(this);
		MyCharacter->GetOnEntityDeathDelegate().RemoveAll(this);
		MyCharacter->OnInCombatStateChanged.RemoveAll(this);
	}
	if(AbilitySystemComponent)
	{
		if (HealthAttributeSet)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCHealthAttributeSet::GetMaxHealthAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCHealthAttributeSet::GetHealthAttribute()).RemoveAll(this);
		}
		if (TraitAttributeSet)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCTraitAttributeSet::GetFortitudeAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCTraitAttributeSet::GetMasteryAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCTraitAttributeSet::GetMovementSpeedAttribute()).RemoveAll(this);
		}

		if (EnergyAttributeSet)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCEnergyAttributeSet::GetMaxEnergyAttribute()).RemoveAll(this);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCEnergyAttributeSet::GetEnergyAttribute()).RemoveAll(this);
		}
	}

	Super::RemoveFromParent();
}

void UVCHUD_Player::InitializeHUD()
{
	MyPlayerController = GetOwningPlayer<AVCPlayerController>();
	if(MyPlayerController == nullptr)
	{
		return;
	}
	
	MyCharacter = MyPlayerController->GetControlledCharacter();
	if(MyCharacter)
	{
		AbilitySystemComponent = MyCharacter->GetAbilitySystemComponent();
		// Cache attribute set references that will be used
		for (auto& AttributeSet : AbilitySystemComponent->SpawnedAttributes)
		{
			if (AttributeSet->IsA(UVCHealthAttributeSet::StaticClass()))
			{
				HealthAttributeSet = Cast<UVCHealthAttributeSet>(AttributeSet);
			}
			else if (AttributeSet->IsA(UVCEnergyAttributeSet::StaticClass()))
			{
				EnergyAttributeSet = Cast<UVCEnergyAttributeSet>(AttributeSet);
			}
			else if (AttributeSet->IsA(UVCTraitAttributeSet::StaticClass()))
			{
				TraitAttributeSet = Cast<UVCTraitAttributeSet>(AttributeSet);
			}
		}

		UpdateLevel(MyCharacter->GetCharacterLevel());
		UpdateXP(MyCharacter->GetCurrentXPRatio());

		MyCharacter->OnTeamChanged.AddWeakLambda(this, [=](ETeamType PrevTeam, ETeamType CurrentTeam)
		{
			UpdateTeamLogo();
		});
		
		MyCharacter->GetXPChangedDelegate().AddWeakLambda(this, [=](int DeltaXP, int TotalXP, int ProgressedBracketXP, int RequiredBracketXP)
		{
			UpdateXP(static_cast<float>(ProgressedBracketXP) / RequiredBracketXP);
		});

		MyCharacter->GetOnLevelChangedDelegate().AddWeakLambda(this, [=](int PrevLevel, int CurrentLevel)
		{
			UpdateLevel(CurrentLevel);
			if(CurrentLevel > PrevLevel)
			{
				BP_OnLevelUp();
			}
		});

		MyCharacter->GetOnEntityDeathDelegate().AddWeakLambda(this, [=](FDeathInfo DeathInfo)
		{
			if(DeathInfo.IsDead)
			{
				BP_NotifyDeath();
			}
		});
		
		// Notify Combat status change
		InCombatOverlay->SetVisibility(ESlateVisibility::Hidden);
		MyCharacter->OnInCombatStateChanged.AddWeakLambda(this, [=](bool IsInCombat)
		{
			if(InCombatOverlay)
			{
				IsInCombat ? InCombatOverlay->SetVisibility(ESlateVisibility::Visible) :
							 InCombatOverlay->SetVisibility(ESlateVisibility::Hidden);
			}
		});

		if(HealthAttributeSet)
		{
			// Notify Max Health ChangeVCHealthAttributeSet
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCHealthAttributeSet::GetMaxHealthAttribute())
			.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData){ UpdateHealth(); });

			// Notify Health ChangeVCHealthAttributeSet
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCHealthAttributeSet::GetHealthAttribute())
			.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData){ UpdateHealth(); });

		}

		if(TraitAttributeSet)
		{
			// Notify Fortitude ChangeVCHealthAttributeSet
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCTraitAttributeSet::GetFortitudeAttribute())
			.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData){ UpdateFortitude(); });

			// Notify Mastery ChangeVCHealthAttributeSet
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCTraitAttributeSet::GetMasteryAttribute())
			.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData){ UpdateMastery(); });

			// Notify Movement Speed ChangeVCHealthAttributeSet
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCTraitAttributeSet::GetMovementSpeedAttribute())
				.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData){ UpdateMovementSpeed(); });
		}

		if(EnergyAttributeSet)
		{
			// Notify Max Energy Change
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCEnergyAttributeSet::GetMaxEnergyAttribute())
			.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData){ UpdateEnergy(); });

			// Notify Energy Change
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVCEnergyAttributeSet::GetEnergyAttribute())
			.AddWeakLambda(this, [=](const FOnAttributeChangeData& AttributeChangeData){ UpdateEnergy(); });
		}
	}
}

void UVCHUD_Player::InitializeAbility(int AbilityInputID, UVCWidget_AbilitySlot* AbilitySlotWidget)
{
	if (AbilitySystemComponent == nullptr) {
		return;
	}
	const auto AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromInputID(AbilityInputID);
	if(AbilitySpec == nullptr){
		return;
	}
	const auto AbilityInstance = Cast<UVCGameplayAbility>(AbilitySpec->GetPrimaryInstance());
	if(AbilityInstance == nullptr){
		return;
	}
	
	// Initialize ability slot widget
	AbilitySlotWidget->InitAbilitySlot(AbilityInstance);

	// Set ability cooldown events
	UGameplayEffect* CooldownEffect = AbilityInstance->GetCooldownGameplayEffect();
	if(CooldownEffect)
	{
		// Apply cooldown based on cooldown tag
		FGameplayTagContainer CooldownTagContainer;
		CooldownEffect->GetOwnedGameplayTags(CooldownTagContainer);
		for (const FGameplayTag & CooldownTag : CooldownTagContainer)
		{
			FDelegateHandle DelegateHangle = AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
				.AddWeakLambda(this, [AbilitySlotWidget](const FGameplayTag Tag, int32 Count)
				{
					Count == 0 ? AbilitySlotWidget->ClearCooldown() : AbilitySlotWidget->TriggerCooldown();
				});

			OnRemovedFromViewport.AddWeakLambda(this, [&, DelegateHangle]
			{
				if(AbilitySystemComponent)
				{
					AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).Remove(DelegateHangle);
				}
			});
		}
	}
}
void UVCHUD_Player::UpdateHealth() const
{
	if (HealthAttributeSet)
	{
		const float CurrentHealth = FMath::Clamp(HealthAttributeSet->GetHealth(), 0.f, HealthAttributeSet->GetMaxHealth());
		const float MaxHealth = HealthAttributeSet->GetMaxHealth();
		const float HealthRatio = CurrentHealth / MaxHealth;
		const FString HealthText = FString::Printf(TEXT(" %i / %i"),
			static_cast<int>(CurrentHealth), static_cast<int>(MaxHealth));

		Health_TextBlock->SetText(FText::FromString(HealthText));
		Health_ProgressBar->SetPercent(HealthRatio);
	}
	else
	{
		Health_TextBlock->SetText(FText::GetEmpty());
		Health_ProgressBar->SetPercent(0.f);
	}
}

void UVCHUD_Player::UpdateEnergy() const
{
	if (EnergyAttributeSet)
	{
		const float CurrentEnergy = EnergyAttributeSet->GetEnergy();
		const float MaxEnergy = EnergyAttributeSet->GetMaxEnergy();
		const FString EnergyText = FString::Printf(TEXT(" %i / %i"),
			static_cast<int>(CurrentEnergy), static_cast<int>(MaxEnergy));

		Energy_TextBlock->SetText(FText::FromString(EnergyText));
		Energy_ProgressBar->SetPercent(CurrentEnergy / MaxEnergy);
	}
	else
	{
		Energy_TextBlock->SetText(FText::GetEmpty());
		Energy_ProgressBar->SetPercent(0.f);
	}
}

void UVCHUD_Player::UpdateFortitude() const
{
	TraitAttributeSet ? Fortitude_TextBlock->SetText(FText::AsNumber(TraitAttributeSet->GetFortitude())) :
					    Fortitude_TextBlock->SetText(FText::AsNumber(0));
}

void UVCHUD_Player::UpdateMastery() const
{
	TraitAttributeSet ? Mastery_TextBlock->SetText(FText::AsNumber(TraitAttributeSet->GetMastery())) :
						Mastery_TextBlock->SetText(FText::AsNumber(0));
}

void UVCHUD_Player::UpdateMovementSpeed() const
{
	TraitAttributeSet ? Movement_TextBlock->SetText(FText::AsNumber(TraitAttributeSet->GetMovementSpeed())) :
						Movement_TextBlock->SetText(FText::AsNumber(0));
}

void UVCHUD_Player::UpdateLevel(int CurrentLevel) const
{
	Level_TextBlock->SetText(FText::AsNumber(CurrentLevel + 1));
}

void UVCHUD_Player::UpdateXP(float XPRatio) const
{
	XP_ProgressBar->SetPercent(XPRatio);
}

void UVCHUD_Player::TriggerDamageMask(float MaskRatio)
{
	if (DamageMaskRetainerBox)
	{
		UMaterialInstanceDynamic* MaterialInstance = DamageMaskRetainerBox->GetEffectMaterial();
		if (MaterialInstance)
		{
			float ScalarMaterialValue = 0;
			MaterialInstance->GetScalarParameterValue(FMaterialParameterInfo("Radius"), ScalarMaterialValue);
			const float DamageMaskRadiusValue = FMath::Lerp(ScalarMaterialValue, MaskRatio, 0.1 * DamageMaskLerpMagnitude);
			const float DamageMaskHealthRatio = FMath::Clamp(DamageMaskRadiusValue, DamageMaskMinimumRadius, 1.0f);

			MaterialInstance->SetScalarParameterValue("Radius", DamageMaskHealthRatio);
		}
	}
}

void UVCHUD_Player::UpdateTeamLogo() const
{
	const AVCTeam* MyTeam = MyCharacter->GetTeam();
	if (MyTeam)
	{
		if(UMaterialInstance* TeamLogoMaterial = MyTeam->GetTeamLogoMaterial())
		{
			TeamLogo_Image->SetBrushFromMaterial(TeamLogoMaterial);
			return;
		}
		if(UTexture2D* TeamLogoTexture = MyTeam->GetTeamLogoImage())
		{
			TeamLogo_Image->SetBrushFromTexture(TeamLogoTexture);
		}
	}
	TeamLogo_Image->SetBrushFromTexture(nullptr);
}


void UVCHUD_Player::NotifySpawn()
{
	BP_NotifySpawn();
}


void UVCHUD_Player::NotifyDamage(float AppliedDamage)
{
	BP_NotifyDamage(AppliedDamage);
}
