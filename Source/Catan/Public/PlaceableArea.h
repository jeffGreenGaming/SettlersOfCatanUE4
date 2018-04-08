// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "Placeable.h"
#include "Road.h"
#include "Tile.h"
#include "Enums/Resources.h"
#include "PlaceableArea.generated.h"

class ATile;
class APlaceableArea;


UCLASS()
class CATAN_API APlaceableArea : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	APlaceableArea();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	APlaceable * getTakenStructure();

	void setTaken(APlaceable * takenBy);

	// has connection to specific placeable area
	bool hasRoadConnection(APlaceableArea * connection);

	// has a connection for that specific player
	bool hasRoadConnection(uint8 playerNum);

	void addRoadConnection(APlaceableArea * connection, ARoad* road);

	TArray<APlaceableArea *> getPossibleRoadConnections();

	bool hasPossibleRoadConnection();

	TArray<ATile *> getConnectedTiles();

	bool hasPort();

	EPort getPort();

private:


	APlaceable * takenStructure;

	TArray<ARoad *> roads;

	TArray<APlaceableArea *> roadConnections;

	UPROPERTY(EditAnywhere)
	TArray<APlaceableArea *> possibleRoadConnections;

	UPROPERTY(EditAnywhere)
	TArray<ATile *> connectedTiles;

	UPROPERTY(EditAnywhere)
	bool portExists;

	UPROPERTY(EditAnywhere)
	EPort port;

};
