// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "VCMacros.h"
#include "AbilitySystemComponent.h"
#include "VCTraitAttributeSet.generated.h"

class UCharacterMovementComponent;

UCLASS()
class VITACORE_API UVCTraitAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UVCTraitAttributeSet();

	ATTRIBUTE_ACCESSORS(UVCTraitAttributeSet, Mastery)
	ATTRIBUTE_ACCESSORS(UVCTraitAttributeSet, Fortitude)
	ATTRIBUTE_ACCESSORS(UVCTraitAttributeSet, XPReward)
	ATTRIBUTE_ACCESSORS(UVCTraitAttributeSet, MovementSpeed)

	// --------- MAXIMUM Mastery
	UPROPERTY(ReplicatedUsing = OnRep_Mastery) FGameplayAttributeData Mastery;

	// --------- Fortitude
	UPROPERTY(ReplicatedUsing = OnRep_Fortitude) FGameplayAttributeData Fortitude;

	// --------- XP REWARD
	UPROPERTY(ReplicatedUsing = OnRep_XPReward) FGameplayAttributeData XPReward;

	// --------- MOVEMENT SPEED
	UPROPERTY(ReplicatedUsing = OnRep_MovementSpeed) FGameplayAttributeData MovementSpeed;

protected:
	UFUNCTION()	void OnRep_Mastery(FGameplayAttributeData PrevMastery);
	UFUNCTION()	void OnRep_Fortitude(FGameplayAttributeData PrevFortitude);
	UFUNCTION()	void OnRep_XPReward(FGameplayAttributeData PrevXPReward);
	UFUNCTION()	void OnRep_MovementSpeed(FGameplayAttributeData PrevMovementSpeed);

protected:
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
