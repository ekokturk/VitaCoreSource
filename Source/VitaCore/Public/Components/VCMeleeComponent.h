// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VCOverlapComponent.h"
#include "VCGameTypes.h"
#include "VCMeleeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMeleeHit, const FHitInfo);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VITACORE_API UVCMeleeComponent : public UVCOverlapComponent
{
	GENERATED_BODY()

public:	
	UVCMeleeComponent();

	FOnMeleeHit OnMeleeHit;
	
protected:
	UPROPERTY()
		bool bIsMeleeCollisionActive;
	
	virtual void HandleOverlapActorAdded(AActor* OtherActor, UPrimitiveComponent* OtherComp, const EAffiliationType OtherAffiliation, const FHitInfo HitInfo) override;
		
};
