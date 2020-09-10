// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "VCGameTypes.h"
#include "VCEntityInterface.generated.h"

// Notify Death.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEntityDeath,const FDeathInfo)

// Delegate to notify team changed. First parameter "Old Team", second parameter "New Team"
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTeamChanged, const ETeamType, const ETeamType)

/** Interface for actors that expose access to an ability system component */
UINTERFACE(BlueprintType)
class VITACORE_API UVCEntityInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class VITACORE_API IVCEntityInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual FString GetEntityName() const = 0;
	virtual EAffiliationType GetAffiliation(AActor* Other) const = 0;
	virtual ETeamType GetTeamType() const = 0;
	virtual bool IsAlive() const = 0;
	virtual void DoHit(const FHitInfo& HitInfo) = 0; // TODO: Remove with DoDamage/OnDamage
	virtual void DoDeath(FDeathInfo DeathInfo = FDeathInfo()) = 0;
	virtual FOnEntityDeath& GetOnEntityDeathDelegate() = 0;
};