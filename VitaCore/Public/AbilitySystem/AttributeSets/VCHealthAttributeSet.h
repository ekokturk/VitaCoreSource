// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "VCEntityInterface.h"
#include "VCMacros.h"


#include "VCHealthAttributeSet.generated.h"

// This delegate takes "Damager Actor", "Damage", "CurrentHealth", "MaxHealth" as parameters
DECLARE_MULTICAST_DELEGATE_FourParams(FOnDamage, AActor*, float, float, float);

// This delegate takes "Damager Actor" and "Damage" as parameters if it is required to update damage or extend functionality
// For ex. Titan damage magnitude overrides damage value before it is applied
DECLARE_DELEGATE_TwoParams(FOnModifyDamage,AActor*,float&)


class UGameplayEffect;

UCLASS()
class VITACORE_API UVCHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UVCHealthAttributeSet();

	UPROPERTY()
		FGameplayTag Tag_FullHealth;
	
	//================================= ATTRIBUTES START =================================

	// --------- HEALTH
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_Health, Category = "Attributes")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UVCHealthAttributeSet, Health)

	// --------- MAXIMUM HEALTH
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UVCHealthAttributeSet, MaxHealth)

	// --------- HEALTH REGENERATION
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing = OnRep_HealthRegen, Category = "Attributes")
		FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UVCHealthAttributeSet, HealthRegen)

	// Replication Notify
	UFUNCTION()	void OnRep_Health(FGameplayAttributeData PrevHealth);
	UFUNCTION()	void OnRep_MaxHealth(FGameplayAttributeData PrevMaxHealth);
	UFUNCTION()	void OnRep_HealthRegen(FGameplayAttributeData PrevHealthRegen);


	// This delegate is called whenever negative effect is applied to health attribute (Damager is only available on server)
	FOnDamage OnDamage;
	
	// This delegate is uVCHealthAttributeSete damage before it is applied to health (Server Only)
	FOnModifyDamage OnModifyDamage;
	
protected:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
