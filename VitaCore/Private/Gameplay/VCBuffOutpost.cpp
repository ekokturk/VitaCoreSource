// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCBuffOutpost.h"
#include "GameplayEffect.h"
#include "VCGameLibrary.h"
#include "VCCoreGameState.h"
#include "VCAbilitySystemComponent.h"
#include "VCCharacter.h"
#include "Components/SphereComponent.h"
#include "GameplayTagsManager.h"
#include "UnrealNetwork.h"
#include "TimerManager.h"
#include "VCWidgetComponent.h"
#include "VCWidget_BuffOutpost.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VCCorePlayerState.h"
#include "VCTeam.h"


// Sets default values
AVCBuffOutpost::AVCBuffOutpost()
{
	// Collision
	BuffAreaVolume = CreateDefaultSubobject<USphereComponent>(TEXT("BuffAreaVolume"));
	BuffAreaVolume->SetGenerateOverlapEvents(true);
	BuffAreaVolume->SetEnableGravity(false);
	BuffAreaVolume->SetSphereRadius(750.0f);
	BuffAreaVolume->SetCollisionProfileName("OverlapPawnIgnoreAll");
	RootComponent = BuffAreaVolume;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponetn"));
	MeshComponent->SetRelativeLocation(FVector(0, 0, 250.0f));
	MeshComponent->SetRelativeRotation(FRotator(0, -90.0f, 0));
	MeshComponent->SetupAttachment(RootComponent);
	
	AbilitySystemComponent = CreateDefaultSubobject<UVCAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	OutpostWidgetComponent = CreateDefaultSubobject<UVCWidgetComponent>(TEXT("BuffOutpostWidget"));
	OutpostWidgetComponent->SetRelativeLocation(FVector(0, 0, 550.0f));
	OutpostWidgetComponent->SetGenerateOverlapEvents(false);
	OutpostWidgetComponent->SetCollisionProfileName("NoCollision");
	OutpostWidgetComponent->SetupAttachment(RootComponent);

	LaserParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaserParticle"));
	LaserParticle->SetupAttachment(RootComponent);
	
	PrimaryActorTick.bCanEverTick = true;

	// Initialization
	bShouldResetAfterLockout = false;
	bShouldRemoveBuffFromTeam = true;
	CaptureRate = 1.0f;
	DeclineRate = 0.5f;
	RefreshRate = 0.2f;
	CapturedTeam = ETeamType::NEUTRAL;
	
	bReplicates = true;
	
	// Tag To check 
	const FName InCombatTagName = "Status.InCombat";
	const FName DeadTagName = "Status.Dead";
	FGameplayTag InCombatTag = UGameplayTagsManager::Get().RequestGameplayTag(InCombatTagName);
	FGameplayTag DeadTag = UGameplayTagsManager::Get().RequestGameplayTag(InCombatTagName);

	IgnoreCaptureWithTags.AddTag(DeadTag);
	IgnoreCaptureWithTags.AddTag(InCombatTag);
}

UAbilitySystemComponent* AVCBuffOutpost::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AVCBuffOutpost::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AVCBuffOutpost::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AVCBuffOutpost::OnOverlapEnd);

	UVCWidget_BuffOutpost * OutpostWidget = Cast<UVCWidget_BuffOutpost>(OutpostWidgetComponent->GetUserWidgetObject());
	if (OutpostWidget != nullptr)
	{
		OutpostWidget->SetOwningOutpost(this);
	}
	
	SetOutpostColor();
}

// Called every frame
void AVCBuffOutpost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsLocked == false)
	{
		UpdateCapturePoints(DeltaTime);

		if (CaptureAmount >= 1.0f && bIsHolding == false)
		{
			DoCapture();
		}	
	}
}

void AVCBuffOutpost::OnRep_IsLocked()
{
	if(bIsLocked == false)
	{
		BP_OnReset();
	}
	else
	{
		BP_OnCapture();
	}
}


void AVCBuffOutpost::UpdateCapturePoints(float DeltaTime)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (OverlappedPlayers.Num() == 0)
		{
			if(CapturedTeam != ETeamType::NEUTRAL)
			{
				if(bIsHolding == false)
				{
					CaptureAmount = FMath::Clamp(CaptureAmount - DeclineRate * DeltaTime, 0.0f, 1.0f);
					if (CaptureAmount == 0)
					{
						if (bShouldRemoveBuffFromTeam == true)
						{
							RemoveExistingBuffFromTeam();
							SetCapturedTeam(ETeamType::NEUTRAL);
						}
					}
				}
				else
				{
					// Refresh to max if it is currently held
					CaptureAmount = FMath::Clamp(CaptureAmount + RefreshRate * DeltaTime, 0.0f, 1.0f);
				}
			}
		}
		else
		{
			for (AVCCharacter* Character : OverlappedPlayers)
			{
				UAbilitySystemComponent * PlayerAbilitySystemComponent = Character->GetAbilitySystemComponent();
				if (PlayerAbilitySystemComponent != nullptr)
				{
					if (PlayerAbilitySystemComponent->HasAnyMatchingGameplayTags(IgnoreCaptureWithTags) == false)
					{
						AddCapturePoints(Character->GetTeamType(), DeltaTime);
					}
				}
			}
		}
	}
}

void AVCBuffOutpost::AddCapturePoints(ETeamType TeamType, float DeltaTime)
{
	if (CapturedTeam == TeamType)
	{
		CaptureAmount += CaptureRate * DeltaTime;
	}
	else
	{
		CaptureAmount -= CaptureRate * DeltaTime;
	}

	CaptureAmount = FMath::Clamp(CaptureAmount, 0.0f, 1.0f);
	if (CaptureAmount == 0)
	{
		if (bShouldRemoveBuffFromTeam == true)
		{
			RemoveExistingBuffFromTeam();
		}

		bIsHolding = false;

		SetCapturedTeam(TeamType);
	}
}


void AVCBuffOutpost::RemoveExistingBuffFromTeam()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		if(CapturedTeam == ETeamType::NEUTRAL)
		{
			return;
		}

		UWorld * World = GetWorld();
		if(World == nullptr)
		{
			return;
		}

		AVCCoreGameState * MyGameState = World->GetGameState<AVCCoreGameState>();
		if(MyGameState != nullptr)
		{
			AVCTeam * CapturedPlayerTeam = Cast<AVCTeam>(MyGameState->GetTeam(CapturedTeam));
			if(CapturedPlayerTeam != nullptr)
			{
				const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
				const FGameplayEffectSpecHandle Handle = AbilitySystemComponent->MakeOutgoingSpec(BuffEffect, BuffLevel, ContextHandle);

				CapturedPlayerTeam->RemoveTeamEffect(Handle);
			}
		}
	}
}


void AVCBuffOutpost::DoCapture()
{
	if (GetLocalRole() == ROLE_Authority)
	{

		UWorld* World = GetWorld();
		if (World == nullptr)
		{
			return;
		}

		AVCCoreGameState* MyGameState = World->GetGameState < AVCCoreGameState>();
		if (MyGameState == nullptr)
		{
			return;
		}

		AVCTeam* PlayerTeam = Cast<AVCTeam>(MyGameState->GetTeam(CapturedTeam));
		if (PlayerTeam == nullptr)
		{
			return;
		}

		bIsLocked = true;
		OnRep_IsLocked();
		if (bShouldResetAfterLockout == false)
		{
			bIsHolding = true;
		}

		for (AVCCharacter* Character : OverlappedPlayers)
		{
			if (Character && Character->GetTeamType() == CapturedTeam)
			{
				AVCCorePlayerState* PlayerState = Character->GetPlayerState<AVCCorePlayerState>();
				if (PlayerState != nullptr)
				{
					PlayerState->Stats.CrystalsCaptured++;
				}
			}
		}

		if (LockoutDuration > 0.0f)
		{
			World->GetTimerManager().SetTimer(TimerHandle_LockoutTime, this, &AVCBuffOutpost::DoReset, LockoutDuration, false);
		}

		// Apply Gameplayeffect
		if (BuffEffect == nullptr)
		{
			return;
		}
		const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle Handle = AbilitySystemComponent->MakeOutgoingSpec(BuffEffect, BuffLevel, ContextHandle);

		PlayerTeam->ApplyTeamEffect(Handle);
	}
}


// ==================== OUTPOST RESET ===================================

void AVCBuffOutpost::DoReset()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsLocked = false;
		OnRep_IsLocked();
		if (bShouldResetAfterLockout == true)
		{
			if (bShouldRemoveBuffFromTeam == true)
			{
				RemoveExistingBuffFromTeam();
			}

			CaptureAmount = 0.0f;
			SetCapturedTeam(ETeamType::NEUTRAL);
		}
	}
}

// ==================== OVERLAP COLLISION ===================================
void AVCBuffOutpost::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		AVCCharacter * Character = Cast<AVCCharacter>(OtherActor);
		
		if(Character && OverlappedPlayers.Contains(Character) == false)
		{
			if(OverlappedPlayers.Num() == 0 && CaptureAmount == 0)
			{
				SetCapturedTeam(Character->GetTeamType());
			}
			OverlappedPlayers.Add(Character);
		}
	}
}

void AVCBuffOutpost::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if(GetLocalRole() == ROLE_Authority)
	{
		AVCCharacter * Character = Cast<AVCCharacter>(OtherActor);

		if(Character && OverlappedPlayers.Contains(Character) == true)
		{
			OverlappedPlayers.Remove(Character);
		}
	}
}

void AVCBuffOutpost::SetCapturedTeam(ETeamType Team)
{
	CapturedTeam = Team;
	OnRep_CapturedTeam();
}

void AVCBuffOutpost::SetOutpostColor()
{
	UWorld * World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	AVCCoreGameState * MyGameState = World->GetGameState<AVCCoreGameState>();
	if (MyGameState == nullptr)
	{
		return;
	}

	FColor CapturedTeamColor = FColor::White;
	AVCTeam * CapturedPlayerTeam = Cast<AVCTeam>(MyGameState->GetTeam(CapturedTeam));
	if (CapturedPlayerTeam != nullptr)
	{
		CapturedTeamColor = CapturedPlayerTeam->GetTeamColor().ToFColor(true);
	}
	UMaterialInterface * Material = MeshComponent->GetMaterial(0);
	UMaterialInstanceDynamic* MaterialInstance = MeshComponent->CreateDynamicMaterialInstance(0, Material);

	if (MaterialInstance != nullptr)
	{
		MaterialInstance->SetVectorParameterValue("Emissive_Color", CapturedTeamColor);
	}

	if (LaserParticle != nullptr)
	{
		LaserParticle->SetColorParameter("Particle_Color", CapturedTeamColor.ReinterpretAsLinear());
	}
}

void AVCBuffOutpost::OnRep_CapturedTeam()
{
	SetOutpostColor();
}


// Replicate properties
void AVCBuffOutpost::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVCBuffOutpost, CapturedTeam);
	DOREPLIFETIME(AVCBuffOutpost, CaptureAmount);
	DOREPLIFETIME(AVCBuffOutpost, bIsLocked);
	DOREPLIFETIME(AVCBuffOutpost, bIsHolding);
}

