// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Resources.generated.h"


USTRUCT(BlueprintType)
struct FResources {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 numBrick;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 numStone;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 numWood;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 numSheep;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	uint8 numWheat;

	uint8 getTotalNumResources() {
		return numBrick + numStone + numWood + numSheep + numWheat;
	}
};