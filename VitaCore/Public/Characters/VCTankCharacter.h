// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCHeroCharacter.h"
#include "Components/SphereComponent.h"

#include "VCTankCharacter.generated.h"

class AVCCharacter;
class USphereComponent;
class UVCMeleeComponent;

/*
 * Tank Character - Joseph Shaw
 */

UCLASS()
class VITACORE_API AVCTankCharacter : public AVCHeroCharacter
{
	GENERATED_BODY()

//================ TANK CHARACTER - JOSEPH SHAW ===================
public:
	AVCTankCharacter();

protected:
	// Socket on the mesh that component attaches to 
	UPROPERTY(EditAnywhere, Category = "Config|Hero Character|Tank")
		FName MeleeComponentSocket;

	// Melee component for metal arm
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UVCMeleeComponent* MeleeComponent;

	// Particle to be attached to melee component
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* MeleeParticle;
	
//================ CHARACTER ACTOR ===================
protected:
	virtual void BeginPlay() override;

};
