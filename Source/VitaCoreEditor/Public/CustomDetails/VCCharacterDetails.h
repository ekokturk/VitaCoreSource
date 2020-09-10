// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

#if WITH_EDITOR

#include "Editor/DetailCustomizations/Public/DetailCustomizations.h"
#include "Editor/PropertyEditor/Public/IDetailCustomization.h"

class FVCCharacterDetails : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};

#endif