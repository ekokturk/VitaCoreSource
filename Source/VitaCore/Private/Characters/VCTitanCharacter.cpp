// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCTitanCharacter.h"
#include "VCEnergyAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "VCAIController.h"
#include "VCAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "VCHealthAttributeSet.h"
#include "VCCoreGameState.h"
#include "VCWidgetComponent.h"
#include "VCHeroCharacter.h"
#include "VCMeleeComponent.h"
#include "VCShooterComponent.h"

AVCTitanCharacter::AVCTitanCharacter()
	: LeftMeleeSocketName(TEXT("LeftMeleeSocket")),
	RightMeleeSocketName(TEXT("RightMeleeSocket")),
	ShooterComponentSocketName(TEXT("ShooterSocket"))
{

	RightMeleeComponent = CreateDefaultSubobject<UVCMeleeComponent>(TEXT("RightMeleeComponent"));
	RightMeleeComponent->ComponentTags.Add(TEXT("RightMeleeComponent"));
	RightMeleeComponent->SetupAttachment(GetMesh(), RightMeleeSocketName);


	LeftMeleeComponent = CreateDefaultSubobject<UVCMeleeComponent>(TEXT("LeftMeleeComponent"));
	LeftMeleeComponent->ComponentTags.Add(TEXT("LeftMeleeComponent"));
	LeftMeleeComponent->SetupAttachment(GetMesh(), LeftMeleeSocketName);

	ShooterComponent = CreateDefaultSubobject<UVCShooterComponent>(TEXT("ShooterComponent"));
	ShooterComponent->SetupAttachment(GetMesh(), ShooterComponentSocketName);
	
	GetMesh()->SetEnableGravity(false);
	GetCapsuleComponent()->SetEnableGravity(false);

	bAlwaysRelevant = true;
	
	AbilityInputBindEnum = StaticEnum<ETitanAbilityTypes>();
}


void AVCTitanCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AVCTitanCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AVCTitanCharacter::DoDeath(FDeathInfo DeathInfo)
{
	Super::DoDeath(DeathInfo);
	SetLifeSpan(10.0f);
}

FString AVCTitanCharacter::GetEntityName() const
{
	switch (TitanType) {
		case ETitanTypes::Ice:
			return "Ice Protector";
		break;
		case ETitanTypes::Fire:
			return "Fire Protector";
		break;
		default:
			return FString()
		;
	}
}

void AVCTitanCharacter::InitializeAbilities()
{
	InitializeInputAbilities<ETitanAbilityTypes>(AbilityMap);
	Super::InitializeAbilities();
}


