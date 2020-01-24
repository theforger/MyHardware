// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Kismet/BlueprintFunctionLibrary.h"
#include "HardwareDetailsStruct.h"
#include "MyHardwareBPLibrary.generated.h"

UCLASS()
class UMyHardwareBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetCurrentHardware", Keywords = "Hardware"), Category = "Hardware")
	static FHardwareDetailsStruct GetCurrentHardware();
};
