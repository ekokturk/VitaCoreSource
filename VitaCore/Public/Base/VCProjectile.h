// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "VCActor.h"
#include "VCProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class AVCCharacter;

UCLASS()
class VITACORE_API AVCProjectile : public AVCActor
{
	GENERATED_BODY()


public:	
	AVCProjectile();

public:
	/* Add Gameplay Effect to projectile */
	UPROPERTY()FGameplayEffectSpecHandle AbilityGameplayEffectSpecHandle;
	
	// Initial speed of the projectile that will be used by projectile movement component
	UPROPERTY(Replicated, EditAnywhere, Category = "Config|Projectile")
		float InitialSpeed;
	
protected:

	// TODO Change on component overlap begin
	// This method is called when projectile is overlapped with another actor
	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// If true entity will die on overlap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|Projectile")
		bool ShouldDieOnOverlap;

	// If true projectile will overlap with allied teams
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config|Projectile")
		bool bShouldOverlapWithAllies;

	// This will determine the if actor can be effected based on affiliation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap Component", meta = (Bitmask, BitmaskEnum = "EAffiliationType"))
		int EffectAffiliationMask;

	UPROPERTY(Transient) AVCCharacter* InstigatorCharacter;
	
	// --- Components
	
	// Projectile collider
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile")
		USphereComponent* CollisionComponent;

	// Projectile Static Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		UStaticMeshComponent* MeshComponent;

	// Projectile Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		UProjectileMovementComponent* ProjectileMovementComponent;
	
// ================ ACTOR ================
protected:
	virtual void BeginPlay() override;
	virtual void OnRep_Instigator() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



};
