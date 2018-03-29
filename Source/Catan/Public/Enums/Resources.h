// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Resources.generated.h"


USTRUCT(BlueprintType)
struct FResources {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	uint8 numBrick;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		uint8 numStone;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		uint8 numWood;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		uint8 numSheep;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		uint8 numWheat;
};