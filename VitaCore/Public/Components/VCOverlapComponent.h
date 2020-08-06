// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "VCGameTypes.h"
#include "VCOverlapComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FOnOverlapAdded, AActor*, UPrimitiveComponent*, const EAffiliationType, const FHitInfo)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnOverlapRemoved, AActor*, UPrimitiveComponent*, const EAffiliationType)

class IVCEntityInterface;

/**
 * This component can be used to keep track of actors overlapping with the owner
 * It is a base class for components that need the similar functionality
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VITACORE_API UVCOverlapComponent : public UCapsuleComponent
{
	GENERATED_BODY()
// ============= OVERLAP COMPONENT ==================
public:	
	UVCOverlapComponent();

	// Enable or disable the active overlap collision
	UFUNCTION() void SetOverlapActive(bool IsActive);

	// Change the radius of overlap component
	UFUNCTION() void SetOverlapRadius(float OverlapRadius);

	// Set team affiliation mask for overlap collision
	UFUNCTION() void SetOverlappingAffiliationMask(int NewMask);

	// Clear overlapped actors array (OnClearOverlappedActors will be called before)
	UFUNCTION() void ClearOverlappedActors();

	// Return currently overlapped actor references
	UFUNCTION() const TArray<AActor*>& GetOverlappedActors() const;

	// Delegate that will be called whenever an actor is added to overlapped array
	FOnOverlapAdded OnOverlapActorAdded;

	// Delegate to be called whenever an actor removed from array (Does not include ClearOverlappedActors)
	FOnOverlapRemoved OnOverlapActorRemoved;
	
protected:
	
	// This method called when component overlaps with an actor
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// This method called when component stops overlapping with an actor
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	// This methods will be called just before clearing contained overlapped actors from the array
	UFUNCTION()
		virtual void OnClearOverlappedActors();
	
	// Internal method to extend functionality if valid actor is added to array
	UFUNCTION()
		virtual void HandleOverlapActorAdded(AActor* OtherActor, UPrimitiveComponent* OtherComp, const EAffiliationType OtherAffiliation, const FHitInfo HitInfo);

	// Internal method to extend functionality if valid actor is removed from array
	UFUNCTION()
		virtual void HandleOverlapActorRemoved(AActor* OtherActor, UPrimitiveComponent* OtherComp, const EAffiliationType OtherAffiliation);

	
// ============= OVERLAP COMPONENT CONFIG ==================
protected:
	// If this is true overlap occurs only on server/authority
	UPROPERTY(EditAnywhere, Category = "Config - Overlap Component")
		bool bShouldOverlapIfAuthority;

	// If this is true actor that is overlapped will be removed from list when it is no longer overlapping
	UPROPERTY(EditAnywhere, Category = "Config - Overlap Component")
		bool bShouldRemoveActorOnOverlapEnd;

	// If this is true component will not overlap with the actor that owns it
	UPROPERTY(EditAnywhere, Category = "Config - Overlap Component")
		bool bShouldOverlapWithOwner;
	
	// This will be the collision profile when the component is active
	UPROPERTY(EditAnywhere, Category = "Config - Overlap Component")
		FName OverlapCollisionProfile;

	// This will determine the if actor can be overlapped based on it's affiliation
	UPROPERTY(EditAnywhere, Category = "Config - Overlap Component", meta=(Bitmask, BitmaskEnum = "EAffiliationType"))
		int OverlappingAffiliationMask;

	// This array will contain the references of all the actors that are overlapped
	UPROPERTY() TArray<AActor*> OverlappedActors;
	
	UPROPERTY()
		TScriptInterface<IVCEntityInterface> OwnerEntity;
	
// ============= ACTOR COMPONENT ==================
protected:
	virtual void BeginPlay() override;
};
