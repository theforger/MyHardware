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

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetLocalIP", Keywords = "Hardware"), Category = "Hardware")
		static const FString GetLocalIP(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Gamepad Connected", Keywords = "Hardware"), Category = "Hardware")
		static bool IsGamepadConnected();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Mouse Connected", Keywords = "Hardware"), Category = "Hardware")
		static bool IsMouseConnected();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Capslock Active", Keywords = "Hardware"), Category = "Hardware")
		static bool IsCapsLockActive();
};
