// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "CatanGameState.h"
#include "CatanGameMode.h"
#include "CatanPlayerState.h"




ACatanPlayerState::ACatanPlayerState() : numSettlementsLeft(5), numCitiesLeft(4), numRoadsLeft(20){
	bReplicates = true;
	//resources = { 4,0,4,2,2 }; //give enough resources for intial placements
	resources = { 10,10,10,10,10 };
	uint8 numPossibleRolls = 12;
	for (int i = 0; i <= numPossibleRolls; i++) {
		perRollIncreases.Add({ 0,0,0,0,0 });
	}
	ports.Add(EPort::Port_Any4);
}

void ACatanPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACatanPlayerState, resources);
	DOREPLIFETIME(ACatanPlayerState, playerNum);

}

void ACatanPlayerState::addSettlement(ASettlement * newSettlement) {
	if (numSettlementsLeft > 0) {
		settlements.Add(newSettlement);
		numSettlementsLeft--;
	}

}

uint8 ACatanPlayerState::getNumCitiesLeft() {
	return numCitiesLeft;
}


void ACatanPlayerState::addCity(ACity * newCity) {
	if (numCitiesLeft > 0) {
		cities.Add(newCity);
		numCitiesLeft--;
	}

}

uint8 ACatanPlayerState::getNumSettlementsLeft() {
	return numSettlementsLeft;
}


void ACatanPlayerState::addRoad(ARoad * newRoad) {
	if (numRoadsLeft > 0) {
		roads.Add(newRoad);
		numRoadsLeft--;
	}
}

uint8 ACatanPlayerState::getNumRoadsLeft() {
	return numRoadsLeft;
}

FResources ACatanPlayerState::getResources() {
	return resources;
}

void ACatanPlayerState::payForPurchase(EPurchaseType type) {
	FResources placeableCost;
	switch (type) {
		case EPurchaseType::Purchase_Settlement:
			placeableCost = ACatanGameMode::settlementCost;
			break;
		case EPurchaseType::Purchase_City:
			placeableCost = ACatanGameMode::cityCost;
			break;
		case EPurchaseType::Purchase_Road:
			placeableCost = ACatanGameMode::roadCost;
			break;
		case EPurchaseType::Purchase_DevelopmentCard:
			placeableCost = ACatanGameMode::developmentCardCost;
			break;
		default:
			return;
	}
	resources.numBrick -= placeableCost.numBrick;
	resources.numStone -= placeableCost.numStone;
	resources.numWheat -= placeableCost.numWheat;
	resources.numSheep -= placeableCost.numSheep;
	resources.numWood -= placeableCost.numWood;

}

void  ACatanPlayerState::setPlayerNum(uint8 num) {
	playerNum = num;
}

uint8 ACatanPlayerState::getPlayerNum() {
	return playerNum;
}

void ACatanPlayerState::addPerRoll(uint8 rollNumber, EResourceType resourceType, int amount) {
	switch(resourceType) {
		case EResourceType::ResourceType_Brick:
			perRollIncreases[rollNumber].numBrick += amount;
			break;
		case EResourceType::ResourceType_Wheat:
			perRollIncreases[rollNumber].numWheat += amount;
			break;
		case EResourceType::ResourceType_Stone:
			perRollIncreases[rollNumber].numStone += amount;
			break;
		case EResourceType::ResourceType_Wood:
			perRollIncreases[rollNumber].numWood += amount;
			break;
		case EResourceType::ResourceType_Sheep:
			perRollIncreases[rollNumber].numSheep += amount;
			break;
	}
}


void ACatanPlayerState::takeResources_Implementation(FResources resourcesToTake) {
	resources.numBrick -= resourcesToTake.numBrick;
	resources.numSheep -= resourcesToTake.numSheep;
	resources.numStone -= resourcesToTake.numStone;
	resources.numWood -= resourcesToTake.numWood;
	resources.numWheat -= resourcesToTake.numWheat;
}

bool ACatanPlayerState::takeResources_Validate(FResources resourcesToTake) {
	return true;
}

void ACatanPlayerState::giveResources_Implementation(FResources resourcesToGive) {
	resources.numBrick += resourcesToGive.numBrick;
	resources.numSheep += resourcesToGive.numSheep;
	resources.numStone += resourcesToGive.numStone;
	resources.numWood += resourcesToGive.numWood;
	resources.numWheat += resourcesToGive.numWheat;
}

bool ACatanPlayerState::giveResources_Validate(FResources resourcesToGive) {
	return true;
}

void ACatanPlayerState::giveResources(uint8 rollNumber) {
	resources.numBrick += perRollIncreases[rollNumber].numBrick;
	resources.numSheep += perRollIncreases[rollNumber].numSheep;
	resources.numStone += perRollIncreases[rollNumber].numStone;
	resources.numWood += perRollIncreases[rollNumber].numWood;
	resources.numWheat += perRollIncreases[rollNumber].numWheat;

}

void ACatanPlayerState::addTradeOfferClient_Implementation(FTradeOffer offer) {
	currentTradeOffers.Add(offer);
}


bool ACatanPlayerState::addTradeOfferClient_Validate(FTradeOffer offer) {
	return true;
}

void ACatanPlayerState::removeTradeOfferClient_Implementation(uint8 playerNum) {
	for (int i = 0; i < currentTradeOffers.Num(); i++) {
		if (playerNum == currentTradeOffers[i].playerNum) {
			currentTradeOffers.RemoveAt(i);
		}
	}
}

bool ACatanPlayerState::removeTradeOfferClient_Validate(uint8 playerNum) {
	return true;
}

FTradeOffer ACatanPlayerState::getNextTradeOffer() {
	if (currentTradeOffers.Num() >= 0) {
		return currentTradeOffers[0];
	}
	return FTradeOffer{ 0,0 };
}

void ACatanPlayerState::addPort_Implementation(EPort newPort) {
	//dont want to have two of the same port in the array
	for (int i = 0; i < ports.Num(); i++) {
		if (ports[i] == newPort) {
			return;
		}
	}
	ports.Add(newPort);
}


bool ACatanPlayerState::addPort_Validate(EPort newPort) {
	return true;
}

