// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VCEntityInterface.h"
#include "VCGameTypes.h"
#include "VCActor.generated.h"

/*
 * Base Actor class that all actors in the game should inherit from
 */

UCLASS()
class VITACORE_API AVCActor : public AActor, public IVCEntityInterface
{
	GENERATED_BODY()
// =================== VC ACTOR ====================
public:	
	AVCActor();

	virtual void DoDeath(FDeathInfo DeathInfo = FDeathInfo()) override;
	virtual void DoHit(const FHitInfo& HitInfo) override;

	virtual FString GetEntityName() const override;
	virtual EAffiliationType GetAffiliation(AActor* Other) const override;
	
	virtual bool IsAlive() const override						{ return bIsAlive; };
	virtual ETeamType GetTeamType() const override				{ return TeamType; };
	float GetEntityLifetime() const								{ return EntityLifetime; };
	virtual FOnEntityDeath& GetOnEntityDeathDelegate() override { return OnEntityDeath; };

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Config", meta = (DisplayName = "OnDeath"))
		void BP_OnDeath();

	FOnEntityDeath OnEntityDeath;
	
protected:

	// If true, actor is alive
	UPROPERTY(Replicated, EditAnywhere, Category = "Config")
		bool bIsAlive;

	// Team type of the actor that its affiliated to
	UPROPERTY(Replicated, EditAnywhere, Category = "Config")
		ETeamType TeamType;

	// Duration before Death method is called for the actor
	UPROPERTY(EditAnywhere, Category = "Config")
		float EntityLifetime;

	// If true, actor will be destroyed on death
	UPROPERTY(EditAnywhere, Category = "Config")
		bool bIsDestroyedOnDeath;

	FTimerHandle TimerHandle_EntityLifetime;
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
