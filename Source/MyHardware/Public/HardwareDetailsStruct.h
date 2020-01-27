// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "HardwareDetailsStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct MYHARDWARE_API FHardwareDetailsStruct
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		FString CPUVendor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		FString CPUBrand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		int32 NumberOfCoresIncludingHyperthread;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		int32 NumberOfCores;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		bool Is64BitOS;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		FString PrimaryGPUBrand;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		FString MacAddress;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		int32 BatteryLevel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		FString OSVersion;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		float DeviceTemperature;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hardware Data")
		FString EpicAccountID;
};
