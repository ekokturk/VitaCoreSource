// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameplayEffectTypes.h"
#include "ComponentTypes/VCShooterComponentTypes.h"
#include "VCShooterComponent.generated.h"

class AVCProjectile;
class AVCLaser;
class UArrowComponent;
class UCameraComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnShooterProjectileSpawned, AVCProjectile*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShooterLaserSpawned, AVCLaser*)
DECLARE_MULTICAST_DELEGATE(FOnShooterShootingStopped)

/**
 * This component allows owner to spawn projectile, laser actors with certain properties
 * It can update the direction it's facing to match character look direction
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), hideCategories = (Input, Actor, "Actor Tick", LOD, Variable))
class VITACORE_API UVCShooterComponent : public UArrowComponent
{
	GENERATED_BODY()

// =============== SHOOTER COMPONENT ===============
public:
	UVCShooterComponent();

	// Get transform for aiming (Target, Camera, default component transform)
	UFUNCTION(BlueprintCallable)
		FTransform GetAimTransform(AActor * Target = nullptr) const;

	// Set type of how component direction will be updated (Value is replicated)
	UFUNCTION(BlueprintCallable)
		void SetDirectionUpdate(EShooterComponentDirectionUpdateType DirectionUpdate);
	
	// Check if component is currently shooting
	UFUNCTION()
		bool IsShooting() const;

private:
	// If true shooter is currently active
	UPROPERTY()
		bool bIsShooting;

	// Update direction of the actor component based on selected update type (Activate replication to change it on runtime)
	UFUNCTION()
		void UpdateDirection();

	// Spawn Parameters for actors spawned by shooter component
	FActorSpawnParameters SpawnParameters;
	
// ============ SHOOTER CONFIG =============
protected:
	// Only Spawn on Server
	UPROPERTY(EditAnywhere, Category = "ShooterComponent - Config")
		bool bShouldSpawnOnAuthority;

	// Defines update type of the component orientation that will be used for targeting (Activate replication to change it on runtime)
	UPROPERTY(Replicated, EditAnywhere, Category = "ShooterComponent - Config")
		EShooterComponentDirectionUpdateType DirectionUpdateType;
	
// =============== STOP SHOOTING =====================
public:
	// TODO Stop condition can be used for this method as a paramerter
	// Stop shooting and clear all leftover operations
	UFUNCTION()
		void StopShooting();

	// Callback that is triggered when StopShooting is called
	FOnShooterShootingStopped OnShootingStopped;
	
// =============== ACTOR COMPONENT =====================
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// ============ CAMERA ============
	// Raycast distance from screen space to world space targeting
	UPROPERTY(EditAnywhere, Category = "ShooterComponent - Config")
		float CameraAimRaycastDistance;
	
	// Data to keep track of projectiles for timed actions
	UPROPERTY()
		UCameraComponent* CameraComponentRef;

	UPROPERTY(EditAnywhere, Category = "ShooterComponent - Config")
		bool bShouldUseCameraToAim;

// ============ PROJECTILE ============
public:
	// Projectile Shooting
	void Shoot(TSubclassOf<AVCProjectile> SpawnProjectile, FProjectileShootParams& ShootParams);

	// Multicast delegate called when a projectile is spawned
	FOnShooterProjectileSpawned OnShooterProjectileSpawned;
	
private:
	// Internal method to shoot a projectile with a given index
	void ShootProjectile(TSubclassOf<AVCProjectile> SpawnProjectile, FProjectileShootParams& ShootParams, int ProjectileIndex = 0);

// ============ LASER ============
public:
	void Shoot(TSubclassOf<AVCLaser> LaserClass, const FLaserShootParams& ShootParams);
	
	// Multicast delegate called when a laser is spawned
	FOnShooterLaserSpawned OnShooterLaserSpawned;
};
