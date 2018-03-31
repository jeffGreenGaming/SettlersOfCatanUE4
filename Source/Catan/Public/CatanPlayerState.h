// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Settlement.h"
#include "City.h"
#include "Road.h"
#include "Net/UnrealNetwork.h"
#include "Enums/PurchaseType.h"
#include "Enums/Resources.h"
#include "CatanPlayerState.generated.h"

/**
 * 
 */




UCLASS()
class CATAN_API ACatanPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:

	ACatanPlayerState();

	void addSettlement(ASettlement * newSettlement);
	
	uint8 getNumSettlementsLeft();

	void addRoad(ARoad * newRoad);

	uint8 getNumRoadsLeft();

	void addCity(ACity * newCity);

	uint8 getNumCitiesLeft();

	UFUNCTION(BlueprintCallable, Category = "CatanPlayerStateFunctions")
	FResources getResources();

	void payForPurchase(EPurchaseType type);


	void setPlayerNum(uint8 num);

	uint8 getPlayerNum();

	void addPerRoll(uint8 rollNumber, EResourceType resourceType);

	void giveResources(uint8 rollNumber);

private:

	TArray<ASettlement *> settlements;

	uint8 numSettlementsLeft;

	TArray<ACity*> cities;

	uint8 numCitiesLeft;

	TArray<ARoad*> roads;

	uint8 numRoadsLeft;

	uint8 score;

	UPROPERTY(replicated, VisibleAnywhere)
	FResources resources;

	UPROPERTY(VisibleAnywhere)
	TArray<FResources> perRollIncreases;

	UPROPERTY(replicated)
	uint8 playerNum;
	
};
