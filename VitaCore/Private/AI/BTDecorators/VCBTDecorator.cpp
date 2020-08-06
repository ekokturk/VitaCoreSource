// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.


#include "VCBTDecorator.h"

UVCBTDecorator::UVCBTDecorator()
	:CachedDescription(TEXT(""))
{
}

void UVCBTDecorator::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
#if WITH_EDITOR
	BuildDescription();
#endif	
}

FString UVCBTDecorator::GetStaticDescription() const
{
	return CachedDescription;
}

#if WITH_EDITOR
void UVCBTDecorator::BuildDescription()
{
	
}

void UVCBTDecorator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property == nullptr)
	{
		return;
	}

	BuildDescription();
}
#endif