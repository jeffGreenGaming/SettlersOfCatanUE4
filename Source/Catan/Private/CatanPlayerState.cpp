// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "CatanGameState.h"
#include "CatanGameMode.h"
#include "CatanPlayerState.h"




ACatanPlayerState::ACatanPlayerState() : numSettlementsLeft(5), numCitiesLeft(4), numRoadsLeft(20){
	bReplicates = true;
	resources = { 5,5,5,5,5 };
	uint8 numPossibleRolls = 12;
	for (int i = 0; i <= numPossibleRolls; i++) {
		perRollIncreases.Add({ 0,0,0,0,0 });
	}
}

void ACatanPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACatanPlayerState, resources);

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

void ACatanPlayerState::addPerRoll(uint8 rollNumber, EResourceType resourceType) {
	switch(resourceType) {
		case EResourceType::ResourceType_Brick:
			perRollIncreases[rollNumber].numBrick++;
			break;
		case EResourceType::ResourceType_Wheat:
			perRollIncreases[rollNumber].numWheat++;
			break;
		case EResourceType::ResourceType_Stone:
			perRollIncreases[rollNumber].numStone++;
			break;
		case EResourceType::ResourceType_Wood:
			perRollIncreases[rollNumber].numWood++;
			break;
		case EResourceType::ResourceType_Sheep:
			perRollIncreases[rollNumber].numSheep++;
			break;
	}
}

void ACatanPlayerState::giveResources(uint8 rollNumber) {
	resources.numBrick += perRollIncreases[rollNumber].numBrick;
	resources.numSheep += perRollIncreases[rollNumber].numSheep;
	resources.numStone += perRollIncreases[rollNumber].numStone;
	resources.numWood += perRollIncreases[rollNumber].numWood;
	resources.numWheat += perRollIncreases[rollNumber].numWheat;

}

