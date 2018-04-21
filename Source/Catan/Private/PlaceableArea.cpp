// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "PlaceableArea.h"


// Sets default values
APlaceableArea::APlaceableArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	takenStructure = nullptr;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void APlaceableArea::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlaceableArea::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

APlaceable * APlaceableArea::getTakenStructure() {
	return takenStructure;
}

void APlaceableArea::setTaken(APlaceable * takenBy) {
	takenStructure = takenBy;

}

bool APlaceableArea::hasRoadConnection(uint8 playerNum) {
	for (int i = 0; i < roads.Num(); i++) {
		if (roads[i]->getOwnerNum() == playerNum) {
			return true;
		}
	}
	return false;
}

bool APlaceableArea::hasRoadConnection(APlaceableArea * connection) {
	for (int i = 0; i < roadConnections.Num(); i++) {
		if (roadConnections[i] == connection) {
			return true;
		}
	}
	return false;

}

void APlaceableArea::addRoadConnection(APlaceableArea * connection, ARoad * road) {
	roadConnections.Add(connection);
	roads.Add(road);
}

TArray<APlaceableArea *> APlaceableArea::getPossibleRoadConnections() {
	return possibleRoadConnections;
}


bool APlaceableArea::hasPossibleRoadConnection() {
	return roadConnections.Num() != possibleRoadConnections.Num();
}

TArray<ATile *> APlaceableArea::getConnectedTiles() {
	return connectedTiles;

}

bool APlaceableArea::hasPort() {
	return portExists;
}

EPort APlaceableArea::getPort() {
	return port;
}