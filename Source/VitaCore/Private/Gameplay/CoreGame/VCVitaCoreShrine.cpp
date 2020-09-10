// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCVitaCoreShrine.h"
#include "VCWidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealNetwork.h"
#include "GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "VCEffectComponent.h"
#include "VCHealthAttributeSet.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "VCAbilitySystemComponent.h"
#include "VCCharacter.h"
#include "VCCoreGameState.h"

AVCVitaCoreShrine::AVCVitaCoreShrine()
	:bIsShieldActive(true),
	ShieldMaxRadius(50.f),
	ShieldExpansionRate(.1f),
	ShieldExpansionAmount(.2f),
	ShieldCurrentRadius(.1f)
{
	// Damage capsule
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	ShrineMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShrineMesh"));
	ShrineMesh->SetupAttachment(RootComponent);

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetRelativeScale3D(FVector(ShieldCurrentRadius, ShieldCurrentRadius, ShieldCurrentRadius));
	ShieldMesh->SetVisibility(false);
	ShieldMesh->SetCollisionProfileName("NoCollision");
	ShieldMesh->SetupAttachment(RootComponent);
	
	AbilitySystemComponent = CreateDefaultSubobject<UVCAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	HealthAttributeSet = CreateDefaultSubobject<UVCHealthAttributeSet>(TEXT("HealthAttributeSet"));
	
	ShieldEffectComponent = CreateDefaultSubobject<UVCEffectComponent>(TEXT("ShieldEffectComponent"));
	ShieldEffectComponent->SetOverlapActive(bIsShieldActive);
	ShieldEffectComponent->SetOverlappingAffiliationMask(0 | EAffiliationType::ENEMY);
	ShieldEffectComponent->SetOverlapRadius(ShieldCurrentRadius);
	ShieldEffectComponent->SetupAttachment(RootComponent);

	ShieldParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShieldParticle"));
	ShieldParticle->SetAutoActivate(false);
	ShieldParticle->SetupAttachment(RootComponent);

	ActiveShieldParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ActiveShieldParticle"));
	ActiveShieldParticle->SetAutoActivate(false);
	ActiveShieldParticle->SetupAttachment(RootComponent);

	Tag_Immune = FGameplayTag::RequestGameplayTag("Status.Immune");
}

void AVCVitaCoreShrine::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetHitPoints(10000); // Initialize HitPoints
}

void AVCVitaCoreShrine::BeginPlay()
{
	Super::BeginPlay();
	
	bIsShieldActive ? HandleShieldActivated() : HandleShieldDeactivated();
	
	HealthAttributeSet->OnDamage.AddUObject(this, &AVCVitaCoreShrine::HandleDamage);
}

UAbilitySystemComponent* AVCVitaCoreShrine::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AVCVitaCoreShrine::SetShieldActive(bool IsActive)
{
	check(HasAuthority())
	if(bIsShieldActive == IsActive){
		return;
	}
	bIsShieldActive = IsActive;
	ShieldEffectComponent->SetOverlapActive(IsActive);
	if (bIsShieldActive)
	{
		AbilitySystemComponent->AddLooseGameplayTag(Tag_Immune);
		HandleShieldActivated();
	}
	else
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(Tag_Immune);
		HandleShieldDeactivated();
	}
}

void AVCVitaCoreShrine::SetHitPoints(float HitPoints) const
{
	if(HitPoints > HealthAttributeSet->GetMaxHealth())
	{
		HealthAttributeSet->SetMaxHealth(HitPoints);
	}
	HealthAttributeSet->SetHealth(HitPoints);
}

void AVCVitaCoreShrine::DoCapture(AVCTeam* CapturedTeam)
{
	
}


void AVCVitaCoreShrine::OnRep_IsShieldActive(bool IsShieldActivePrev)
{
	if(IsShieldActivePrev != bIsShieldActive)
	{
		bIsShieldActive ? HandleShieldActivated() : HandleShieldDeactivated();
	}
}

void AVCVitaCoreShrine::HandleShieldActivated()
{
	auto& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(TimerHandle_ShieldExpansion);
	ActiveShieldParticle->ActivateSystem();

	// Start expanding shield
	TimerManager.SetTimer(TimerHandle_ShieldExpansion, this, &AVCVitaCoreShrine::ExpandShield, ShieldExpansionRate, true);
}

void AVCVitaCoreShrine::HandleShieldDeactivated()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ShieldExpansion);
	ActiveShieldParticle->DeactivateSystem();

	ShieldMesh->SetWorldScale3D(FVector(.1f,.1f,.1f));
	ShieldEffectComponent->SetOverlapRadius(.1f);
}

void AVCVitaCoreShrine::ExpandShield()
{
	ShieldCurrentRadius += ShieldExpansionAmount;
	ShieldMesh->SetWorldScale3D(FVector(ShieldCurrentRadius));
	ShieldEffectComponent->SetOverlapRadius(ShieldCurrentRadius);
	
	if(ShieldMesh->GetRelativeScale3D().X >= ShieldMaxRadius)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_ShieldExpansion);
	}
}

void AVCVitaCoreShrine::HandleDamage(AActor* Damager, float Damage, float CurrentHealth, float MaxHealth)
{
	
}


// Replicate properties
void AVCVitaCoreShrine::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AVCVitaCoreShrine, bIsShieldActive);
}


