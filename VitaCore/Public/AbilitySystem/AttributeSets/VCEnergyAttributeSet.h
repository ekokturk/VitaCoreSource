// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "VCMacros.h"

#include "VCEnergyAttributeSet.generated.h"

UCLASS()
class VITACORE_API UVCEnergyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UVCEnergyAttributeSet();

	UPROPERTY(BlueprintReadWrite, Category = "AttributeSet")
		FGameplayTag Tag_FullEnergy;
	
	//================================= ATTRIBUTES START =================================

	// ENERGY
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Energy, Category = "Attributes")
		FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UVCEnergyAttributeSet, Energy)

	// MAX ENERGY
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MaxEnergy, Category = "Attributes")
		FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UVCEnergyAttributeSet, MaxEnergy)

	// ENERGY REGEN
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_EnergyRegen, Category = "Attributes")
		FGameplayAttributeData EnergyRegen;
	ATTRIBUTE_ACCESSORS(UVCEnergyAttributeSet, EnergyRegen)

	//================================= ATTRIBUTES END =================================
	// Attribute set methods
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	// Replication Notify
	UFUNCTION()	void OnRep_Energy(FGameplayAttributeData PrevEnergy);
	UFUNCTION()	void OnRep_MaxEnergy(FGameplayAttributeData PrevMaxEnergy);
	UFUNCTION()	void OnRep_EnergyRegen(FGameplayAttributeData PrevEnergyRegen);

	// Property Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
