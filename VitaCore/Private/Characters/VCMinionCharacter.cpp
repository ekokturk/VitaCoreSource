// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCMinionCharacter.h"

#include "GameplayAbilitySpec.h"
#include "VCAbilitySystemComponent.h"
#include "VCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/VCMeleeComponent.h"
#include "VCSpawner.h"
#include "VCHeroCharacter.h"
#include "TimerManager.h"

#include "Components/VCShooterComponent.h"

AVCMinionCharacter::AVCMinionCharacter()
	:ShooterComponentSocketName(TEXT("MuzzleSocket")),
	LeftMeleeSocketName(TEXT("LeftMeleeSocket")),
	RightMeleeSocketName(TEXT("RightMeleeSocket"))
{
	ShooterComponent = CreateDefaultSubobject<UVCShooterComponent>(TEXT("ShooterComponent"));
	ShooterComponent->SetupAttachment(GetMesh(), ShooterComponentSocketName);

	LeftMeleeComponent = CreateDefaultSubobject<UVCMeleeComponent>(TEXT("LeftMeleeComponent"));
	LeftMeleeComponent->ComponentTags.Add(TEXT("LeftMeleeComponent"));
	LeftMeleeComponent->SetOverlapActive(false);
	LeftMeleeComponent->SetupAttachment(GetMesh(), LeftMeleeSocketName);

	RightMeleeComponent = CreateDefaultSubobject<UVCMeleeComponent>(TEXT("RightMeleeComponent"));
	RightMeleeComponent->ComponentTags.Add(TEXT("RightMeleeComponent"));
	RightMeleeComponent->SetOverlapActive(false);
	RightMeleeComponent->SetupAttachment(GetMesh(), RightMeleeSocketName);
	
	Cue_InCombat = FGameplayTag::RequestGameplayTag("GameplayCue.Minion.InCombat");
	
	AbilityInputBindEnum = StaticEnum<EMinionAbilityTypes>();
}

FString AVCMinionCharacter::GetEntityName() const
{
	return "Guardian";
}

void AVCMinionCharacter::BeginPlay()
{
	Super::BeginPlay();
	//SetStatusBarVisibility(false);
}


void AVCMinionCharacter::DoDeath(FDeathInfo DeathInfo)
{
	Super::DoDeath(DeathInfo);
	if (GetLocalRole() != ROLE_Authority) return;

	SetLifeSpan(2.5f);
}



void AVCMinionCharacter::HandleCombatStateChange(const bool IsInCombat)
{
	if(AbilitySystemComponent)
	{
		IsInCombat ? AbilitySystemComponent->AddGameplayCueLocal(Cue_InCombat, FGameplayCueParameters()) :
					 AbilitySystemComponent->RemoveGameplayCueLocal(Cue_InCombat, FGameplayCueParameters());
	}
}

void AVCMinionCharacter::InitializeAbilities()
{
	InitializeInputAbilities<EMinionAbilityTypes>(AbilityMap);
	Super::InitializeAbilities();

}

