// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VCPropController.generated.h"

UCLASS()
class VITACORE_API AVCPropController : public AActor
{
	GENERATED_BODY()
	
public:	
	AVCPropController();

	// Activate a certain prop in the level
	UFUNCTION() void ActiveProp(AActor* Prop);

	// Deactivate a certain prop in the level
	UFUNCTION() void DeactiveProp(AActor* Prop);

	UFUNCTION(BlueprintCallable)
		void SetFireTitanPropsActive(bool IsActive);

	UFUNCTION(BlueprintCallable)
		void SetIceTitanPropsActive(bool IsActive);

protected:
	UPROPERTY(EditAnywhere, Category="Config")
		TArray<AActor*> FireTitanProps;

	UPROPERTY(EditAnywhere, Category = "Config")
		TArray<AActor*> IceTitanProps;

protected:
	
	UPROPERTY(ReplicatedUsing = OnRep_IsFireTitanPropsActive)
		bool bIsFireTitanPropsActive;

	UPROPERTY(ReplicatedUsing = OnRep_IsIceTitanPropsActive)
		bool bIsIceTitanPropsActive;
	
	UFUNCTION() void OnRep_IsFireTitanPropsActive();
	UFUNCTION() void OnRep_IsIceTitanPropsActive();

	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	


};
