// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#include "Components/VCShooterComponent.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"

#include "Camera/CameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "VCCharacter.h"
#include "VCProjectile.h"
#include "VCLaser.h"

UVCShooterComponent::UVCShooterComponent()
	:bIsShooting(false),
	bShouldSpawnOnAuthority(true),
	DirectionUpdateType(EShooterComponentDirectionUpdateType::NO_UPDATE),
	CameraAimRaycastDistance(1000.0f),
	bShouldUseCameraToAim(true)
{
	// Component Ticks for direction updating if state is set
	bAutoActivate = true;
	bAutoRegister = true;
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicatedByDefault(true); // Component always replicates for state change
}

void UVCShooterComponent::BeginPlay()
{
	Super::BeginPlay();
	// Initialize references
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.Instigator = Cast<APawn>(GetOwner());
	CameraComponentRef = GetOwner()->FindComponentByClass<UCameraComponent>();

	// This will enable or disable the tick function based on update type
	SetDirectionUpdate(DirectionUpdateType);
}

void UVCShooterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Refresh the direction shooter component is facing
	UpdateDirection();
}

FTransform UVCShooterComponent::GetAimTransform(AActor* Target) const
{
	// Get screen space to world aim
	if(Target)
	{
		const FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), Target->GetActorLocation());
		const FVector AimLocation = GetComponentLocation();
		return FTransform(AimRotation, AimLocation);
	}
	else if (bShouldUseCameraToAim == true && CameraComponentRef != nullptr)
	{
		FHitResult OutHit;
		const FVector CameraStart = CameraComponentRef->GetComponentLocation();
		const FVector CameraForwardVector = CameraComponentRef->GetForwardVector();
		const FVector CameraEnd = ((CameraForwardVector * CameraAimRaycastDistance) + CameraStart);
		const bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, CameraStart, CameraEnd, ECC_Visibility);
		const FVector AimLocation = GetComponentLocation();

		if (isHit == true)	
		{
			const FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(AimLocation, OutHit.ImpactPoint);
			return FTransform(AimRotation, AimLocation);
		}
		else
		{
			const FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(AimLocation, CameraEnd);
			return FTransform(AimRotation, AimLocation);
		}
	}
	// Get component orientation as aim
	return GetComponentTransform();
}

void UVCShooterComponent::SetDirectionUpdate(EShooterComponentDirectionUpdateType DirectionUpdate)
{
	const bool ShouldUpdate = (DirectionUpdate != EShooterComponentDirectionUpdateType::NO_UPDATE);
	// Update Direction before Tick starts
	if(ShouldUpdate)
	{
		UpdateDirection();
	}
	// Start tick if it should update
	SetComponentTickEnabled(ShouldUpdate);
	DirectionUpdateType = DirectionUpdate;
}

void UVCShooterComponent::UpdateDirection()
{
	SetWorldRotation(GetAimTransform().Rotator());
}

void UVCShooterComponent::StopShooting()
{
	OnShootingStopped.Broadcast();
}
bool UVCShooterComponent::IsShooting() const
{
	return bIsShooting;
}

void UVCShooterComponent::Shoot(TSubclassOf<AVCProjectile> SpawnProjectile, 
                                FProjectileShootParams& ShootParams)
{

	if (ShootParams.DelayBetweenShots == 0.0f) // Shoot without delay
	{
		for(int i=0; i < ShootParams.Count; i++)
		{
			this->ShootProjectile(SpawnProjectile, ShootParams, i);
		}
		StopShooting();
	}
	else // Shoot with delay
	{
		const FTimerDelegate ShootDelegate = FTimerDelegate::CreateLambda([=, &ShootParams]()
		{
			this->ShootProjectile(SpawnProjectile, ShootParams);
		});
		
		GetWorld()->GetTimerManager().SetTimer(ShootParams.TimerProps.GetHandle(),
			ShootDelegate, ShootParams.DelayBetweenShots, true, 0 );
	}
}

void UVCShooterComponent::Shoot(TSubclassOf<AVCLaser> LaserClass, const FLaserShootParams& ShootParams)
{
	AVCLaser* Laser = GetWorld()->SpawnActorDeferred<AVCLaser>(LaserClass, GetAimTransform(),
		SpawnParameters.Owner, SpawnParameters.Instigator);
	if(Laser)
	{
		// Initialize Laser
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		Laser->AttachToComponent(this, AttachmentRules);
		Laser->EnemyEffectSpecHandle = ShootParams.EnemyEffectHandle;
		Laser->AllyEffectSpecHandle = ShootParams.AllyEffectHandle;
		// Notify laser is spawned
		OnShooterLaserSpawned.Broadcast(Laser);

		UGameplayStatics::FinishSpawningActor(Laser, Laser->GetTransform());
	}
}


void UVCShooterComponent::ShootProjectile(TSubclassOf<AVCProjectile> SpawnProjectile,
                                          FProjectileShootParams & ShootParams, int ProjectileIndex)
{
	UWorld* World = GetWorld();
	if(World == nullptr)
	{
		return;
	}
	
	// Check if projectile is delayed
	if(ShootParams.TimerProps.IsValid() == true)
	{
		ProjectileIndex = ShootParams.TimerProps.GetTimeTick();
		// If delay is over trigger callback function and clear timer
		if(ShootParams.TimerProps.ShouldTimerEnd(ShootParams.Count * ShootParams.DelayBetweenShots) == true)
		{
			// Notify shooting is ended
			StopShooting();
			return;
		}
		ShootParams.TimerProps.UpdateTimeCounter(ShootParams.DelayBetweenShots);
	}
	AVCProjectile* Projectile = World->SpawnActorDeferred<AVCProjectile>(SpawnProjectile, GetAimTransform(ShootParams.Target),
		SpawnParameters.Owner, SpawnParameters.Instigator);
	
	if (Projectile)
	{
		OnShooterProjectileSpawned.Broadcast(Projectile);
		const FTransform AimTransform = GetAimTransform(ShootParams.Target);
		switch (ShootParams.ProjectileShootType) {
			case EProjectileShootTypes::DEFAULT: 
			{
				Projectile->SetActorTransform(AimTransform);
				break;
			}
			// Spread projectiles around the target with a certain angle between them
			case EProjectileShootTypes::SPREAD: 
			{
				FRotator Rotation = AimTransform.Rotator();
				const float AngleBetween = ShootParams.SpreadParams.bShouldClampMaxAngle ? ShootParams.SpreadParams.MaxAngle / (ShootParams.Count-1) : ShootParams.SpreadParams.AngleBetween;
				const float InitialAngle = -0.5f * (ShootParams.SpreadParams.bShouldClampMaxAngle ? ShootParams.SpreadParams.MaxAngle : ShootParams.SpreadParams.AngleBetween * (ShootParams.Count-1));
				Rotation.Yaw += InitialAngle + ProjectileIndex * AngleBetween + ShootParams.SpreadParams.AngleOffset;
				Projectile->SetActorTransform(FTransform(Rotation, AimTransform.GetLocation() + GetForwardVector() * ShootParams.SpreadParams.Distance));
				break;
			}
			// Randomize projectile direction
			case EProjectileShootTypes::RANDOM:
			{
				FRotator Rotation = AimTransform.Rotator();
				Rotation.Yaw += FMath::RandRange(ShootParams.HorizontalRandomAngles.X, ShootParams.HorizontalRandomAngles.Y);
				Rotation.Pitch += FMath::RandRange(ShootParams.VerticalRandomAngles.X, ShootParams.VerticalRandomAngles.Y);
				Projectile->SetActorTransform(FTransform(Rotation, AimTransform.GetLocation()));
				break;
			}
			default: ;
		}
		Projectile->AbilityGameplayEffectSpecHandle = ShootParams.GameplayEffectHandle;
		Projectile->InitialSpeed = ShootParams.InitialSpeed;
		UGameplayStatics::FinishSpawningActor(Projectile, Projectile->GetTransform());
	}
}

void UVCShooterComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UVCShooterComponent, DirectionUpdateType);
}


