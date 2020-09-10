// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/VCActor.h"
#include "GameplayEffectTypes.h"
#include "Particles/ParticleSystemComponent.h"
#include "VCLaser.generated.h"

class USphereComponent;
class UPoseableMeshComponent;
class USpringArmComponent;
class USceneComponent;
class UParticleSystemComponent;
class UAkAudioEvent;

DECLARE_MULTICAST_DELEGATE(FOnLaserStoppedDelegate);

/*
 * Laser actor which deforms a mesh based on hit location
 * It will apply effects to multiple targets at hit location
 */
UCLASS()
class VITACORE_API AVCLaser : public AVCActor
{
	GENERATED_BODY()
public:
	AVCLaser();

	//Set Duration of the laser (It can only be used before initialization)
	UFUNCTION(BlueprintCallable)
		void SetLaserDuration(float NewDuration);

	UFUNCTION(BlueprintCallable)
		void SetLaserLength(float NewLength);

	// This will deactivate the laser and disable collision (Also will destroy actor if bShouldDieOnStop is true)
	UFUNCTION(BlueprintCallable)
		void StopLaser();

	// Delegate that is called with StopLaser
	FOnLaserStoppedDelegate OnLaserStopped;

protected:

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Gameplay effect handle for the laser
	UPROPERTY(BlueprintReadWrite, Category = "Config|Laser")
		FGameplayEffectSpecHandle AllyEffectSpecHandle;

	// Gameplay effect handle for the laser
	UPROPERTY(BlueprintReadWrite, Category = "Config|Laser")
		FGameplayEffectSpecHandle EnemyEffectSpecHandle;
	
protected:
	// If true laser will be destroyed when LaserStop method is called
	UPROPERTY(EditAnywhere, Category = "Config|Laser")
		bool bShouldDieOnStop;

	// Delay to kill entity after laser is stopped
	UPROPERTY(EditAnywhere, Category = "Config|Laser")
		float DeathDelay;

	FTimerHandle TimerHandle_DeathDelay;
	
	// Active duration of the laser
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|Laser")
		float LaserDuration;
	
	FTimerHandle TimerHandle_LaserDuration;

	// Active duration of the laser
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|Laser")
		float LaserLength;
	
	// Vector parameter name of the particle effect that controls the length of laser beam
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|Laser|Particles")
		FName LaserLengthVectorParameter;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|Laser|Audio")
		UAkAudioEvent* LaserInitAkEvent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Laser")
		USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Laser")
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Laser")
		UParticleSystemComponent* LaserImpactParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Laser")
		UParticleSystemComponent* LaserBeamParticle;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostActorCreated() override;

protected:

};
