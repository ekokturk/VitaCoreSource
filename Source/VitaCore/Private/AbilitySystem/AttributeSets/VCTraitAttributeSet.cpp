// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCTraitAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "UnrealNetwork.h"
#include "VCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UVCTraitAttributeSet::UVCTraitAttributeSet()
	:Mastery(0.f),
	Fortitude(0.f),
	XPReward(0.f),
	MovementSpeed(0.f)
{}

bool UVCTraitAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return true;
}

void UVCTraitAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

}

void UVCTraitAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	if (Data.EvaluatedData.Attribute.GetUProperty() == GetMasteryAttribute().GetUProperty()) {}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == GetFortitudeAttribute().GetUProperty()) {}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == GetXPRewardAttribute().GetUProperty()) {}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == GetMovementSpeedAttribute().GetUProperty())
	{
		if (AVCCharacter* MyCharacter = Cast<AVCCharacter>(GetOuter()))
		{
			MyCharacter->GetCharacterMovement()->MaxWalkSpeed = MovementSpeed.GetCurrentValue();
		}
	}
}

void UVCTraitAttributeSet::OnRep_Mastery(FGameplayAttributeData PrevMastery)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCTraitAttributeSet, Mastery, PrevMastery);
}
void UVCTraitAttributeSet::OnRep_Fortitude(FGameplayAttributeData PrevFortitude)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCTraitAttributeSet, Fortitude, PrevFortitude);
}
void UVCTraitAttributeSet::OnRep_XPReward(FGameplayAttributeData PrevXPReward)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCTraitAttributeSet, XPReward, PrevXPReward);
}
void UVCTraitAttributeSet::OnRep_MovementSpeed(FGameplayAttributeData PrevMovementSpeed)
{
	if (AVCCharacter* MyCharacter = Cast<AVCCharacter>(GetOuter()))
	{
		MyCharacter->GetCharacterMovement()->MaxWalkSpeed = MovementSpeed.GetCurrentValue();
	}
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVCTraitAttributeSet, MovementSpeed, PrevMovementSpeed);
}

void UVCTraitAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UVCTraitAttributeSet, Mastery, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVCTraitAttributeSet, Fortitude, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVCTraitAttributeSet, XPReward, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVCTraitAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
}
