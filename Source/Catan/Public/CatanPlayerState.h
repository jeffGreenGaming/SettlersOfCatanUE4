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
#include "Enums/Port.h"
#include "CatanPlayerState.generated.h"

/**
 * 
 */



USTRUCT(BlueprintType)
struct FTradeOffer {

	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FResources resourcesToTrade;

	UPROPERTY(BlueprintReadOnly)
	uint8 playerNum;


};

UCLASS()
class CATAN_API ACatanPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:

	UPROPERTY(BlueprintReadOnly)
	TArray<EPort> ports;

	ACatanPlayerState();

	void addSettlement(ASettlement * newSettlement);
	
	uint8 getNumSettlementsLeft();

	void addRoad(ARoad * newRoad);

	uint8 getNumRoadsLeft();

	void addCity(ACity * newCity);

	uint8 getNumCitiesLeft();

	UFUNCTION(BlueprintCallable, Category = "CatanPlayerStateFunctions")
	FResources getResources();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "CatanPlayerStateFunctions")
	void takeResources(FResources resourcesToTake);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "CatanPlayerStateFunctions")
	void giveResources(FResources resourcesToGive);

	void payForPurchase(EPurchaseType type);

	void setPlayerNum(uint8 num);

	UFUNCTION(BlueprintCallable, Category = "CatanPlayerStateFunctions")
	uint8 getPlayerNum();

	void addPerRoll(uint8 rollNumber, EResourceType resourceType, int amount);

	void giveResources(uint8 rollNumber);

	UFUNCTION(Client, Reliable, WithValidation)
	void addTradeOfferClient(FTradeOffer offer);

	UFUNCTION(Client, Reliable, WithValidation)
	void removeTradeOfferClient(uint8 playerNum);

	UFUNCTION(BlueprintCallable, Category = "CatanPlayerStateFunctions")
	FTradeOffer getNextTradeOffer();

	//only the client cares about what ports we have
	UFUNCTION(Client, Reliable, WithValidation)
	void addPort(EPort newPort);

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

	TArray<FTradeOffer> currentTradeOffers;

	
};
