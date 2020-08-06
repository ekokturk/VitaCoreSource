// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCHeroCharacter.h"
#include "VCEngineerCharacter.generated.h"


class UCapsuleComponent;
class UArrowComponent;
class UParticleSystemComponent;
class UVCEffectComponent;

UCLASS()
class VITACORE_API AVCEngineerCharacter : public AVCHeroCharacter
{
	GENERATED_BODY()

public:
	
	AVCEngineerCharacter();
	
protected:
	// Jetpack skeletal mesh which has it's own animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent * JetpackMesh;

	// Socket that jetpack will be attached to
	UPROPERTY(EditAnywhere, Category = "Config|Hero Character|Engineer")
		FName JetpackSocket;

	// Shooter component for the laser
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UVCShooterComponent* JetpackLaserShooterComponent;

	// Socket that laser component will be attached to
	UPROPERTY(EditAnywhere, Category = "Config|Hero Character|Engineer")
		FName LaserSocket;

	// Flamethrower effect overlap
	UPROPERTY(EditAnywhere)
		UVCEffectComponent* FlamethrowerEffectComponent;

	UPROPERTY(EditAnywhere, Category = "Config|Audio")
		UAkAudioEvent* AkEvent_EngineerIdle;

	UPROPERTY(EditAnywhere, Category = "Config|Hero Character|Engineer")
		FName LeftJetpackThrusterSocket;
	
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* LeftJetpackThrusterParticle;

	UPROPERTY(EditAnywhere, Category = "Config|Hero Character|Engineer")
		FName RightJetpackThrusterSocket;
	
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* RightJetpackThrusterParticle;

	
	virtual void HandleDeathStateChange(bool IsDead) override;
	
protected:
	virtual void BeginPlay() override;
};
