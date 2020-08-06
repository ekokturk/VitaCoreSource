// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCAIController_CoreTitan.h"

#include "VCHealthAttributeSet.h"
#include "VCTraitAttributeSet.h"
#include "VCCharacter.h"
#include "VCCoreGameState.h"
#include "VCCorePlayerState.h"
#include "VCTitanCharacter.h"
#include "Components/CapsuleComponent.h"

AVCAIController_CoreTitan::AVCAIController_CoreTitan()
{
}


void AVCAIController_CoreTitan::BeginPlay()
{
	Super::BeginPlay();
}

void AVCAIController_CoreTitan::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(MyCharacter && MyCharacter->GetHealthAttributeSet())
	{
		MyCharacter->GetHealthAttributeSet()->OnModifyDamage.Unbind();
		MyCharacter->GetHealthAttributeSet()->OnModifyDamage.BindUObject(this, &AVCAIController_CoreTitan::ProcessDamage);
		MyCharacter->GetHealthAttributeSet()->OnDamage.AddUObject(this, &AVCAIController_CoreTitan::OnDamage);
	}

	if(AVCCoreGameState * CoreGameState = GetWorld()->GetGameState<AVCCoreGameState>())
	{
		if (AVCTitanCharacter* TitanCharacter = Cast<AVCTitanCharacter>(MyCharacter))
		{
			if (TitanCharacter->TitanType == ETitanTypes::Fire)
			{
				CoreGameState->FireTitan = TitanCharacter;
			}
			else if(TitanCharacter->TitanType == ETitanTypes::Ice)
			{
				CoreGameState->IceTitan = TitanCharacter;
			}
		}
	}
}

void AVCAIController_CoreTitan::OnUnPossess()
{
	if (MyCharacter && MyCharacter->GetHealthAttributeSet())
	{
		MyCharacter->GetHealthAttributeSet()->OnModifyDamage.Unbind();
		MyCharacter->GetHealthAttributeSet()->OnDamage.RemoveAll(this);
	}
	Super::OnUnPossess();
}


void AVCAIController_CoreTitan::ProcessDamage(AActor* Damager, float& Damage) const
{
	if (Damager)
	{
		Damage *= GetDistanceMultiplier(Cast<AVCCharacter>(Damager));
	}
}

void AVCAIController_CoreTitan::OnDamage(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth)
{
	if (MyCharacter == nullptr || MyCharacter->GetHealthAttributeSet() == nullptr)
	{
		return;
	}

	AVCCharacter* DamagerCharacter = Cast<AVCCharacter>(Damager);
	{
		if (AVCCorePlayerState* DamagerPlayerState = Cast<AVCCorePlayerState>(DamagerCharacter->GetPlayerState()))
		{
			DamagerPlayerState->Stats.XPContribution += Damage * MyCharacter->GetTraitAttributeSet()->GetXPReward();
		}
	}
}

float AVCAIController_CoreTitan::GetDistanceMultiplier(AVCCharacter* Other) const
{
	if (Other == nullptr || MyCharacter == nullptr)
	{
		return 0.f;
	}

	const float Distance = FVector::Dist(Other->GetActorLocation(), MyCharacter->GetActorLocation() - FVector(0.0f, 0.0f, Other->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	const float CapsuleOffset = MyCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	
	// TODO : Change to (Distance - CapsuleOffset - DistanceMultiplierOffset)
	const float DistanceMultiplier = FMath::Clamp(1 - (Distance - CapsuleOffset) / EnemyCharactersCheckRadius, 0.0f, 1.0f);
	
	return DistanceMultiplier;
}