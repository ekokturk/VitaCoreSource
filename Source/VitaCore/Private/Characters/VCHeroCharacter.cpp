// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "VCHeroCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "VCAbilitySystemComponent.h"
#include "VCEnergyAttributeSet.h"
#include "VCGameplayAbility.h"
#include "AkGameplayStatics.h"
#include "VCWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "VCPlayerState.h"
#include "Components/VCShooterComponent.h"


AVCHeroCharacter::AVCHeroCharacter()
	:WeaponSocketName(TEXT("WeaponSocket")),
	MuzzleSocketName(TEXT("MuzzleSocket"))
{

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetGenerateOverlapEvents(false);
	WeaponMesh->SetCollisionProfileName("NoCollision");
	WeaponMesh->SetupAttachment(GetMesh(), WeaponSocketName);

	EnergyAttributeSet = CreateDefaultSubobject<UVCEnergyAttributeSet>("EnergyAttributeSet");

	ShooterComponent = CreateDefaultSubobject<UVCShooterComponent>(TEXT("ShooterComponent"));
	ShooterComponent->SetupAttachment(GetMesh(), MuzzleSocketName);
	
	AbilityInputBindEnum = StaticEnum<EHeroCharacterAbilityTypes>();
}

void AVCHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	ShooterComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false), MuzzleSocketName);


	
	if(IsLocallyControlled() == true && StatusBarComponent != nullptr)
	{
		StatusBarComponent->DestroyComponent();
	}
}


void AVCHeroCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if (NewController->IsLocalController() == true && StatusBarComponent != nullptr)
	{
		StatusBarComponent->DestroyComponent();
	}

}

FString AVCHeroCharacter::GetEntityName() const
{
	AVCPlayerState * MyPlayerState = GetPlayerState<AVCPlayerState>();
	if(MyPlayerState != nullptr)
	{
		return MyPlayerState->GetPlayerName();
	}

	return FString();
}

// Initialize abilities with their designated inputs
void AVCHeroCharacter::InitializeAbilities()
{
	InitializeInputAbilities<EHeroCharacterAbilityTypes>(AbilityMap);
	Super::InitializeAbilities();
}


// Get Player attributes from base attributes
UVCEnergyAttributeSet * AVCHeroCharacter::GetEnergyAttributes() const
{
	return EnergyAttributeSet;
}




