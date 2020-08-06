// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "VCBTDecorator.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class VITACORE_API UVCBTDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UVCBTDecorator();

protected:
	UPROPERTY()
		FString CachedDescription;
	
protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	
#if WITH_EDITOR
	virtual void BuildDescription();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif



};
