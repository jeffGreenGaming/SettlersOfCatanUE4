// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "Placeable.h"
#include "Enums/Port.h"
#include "Enums/Vertex.h"
#include "Enums/TileEdge.h"
#include "Enums/ResourceType.h"
#include "Tile.generated.h"

class ATile;
class APlaceableArea;

UCLASS()
class CATAN_API ATile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UStaticMeshComponent* Mesh;


public:	
	// Sets default values for this actor's properties
	ATile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void setRollValue(uint16 newVal);

	uint8 getColPos();

	uint8 getRowPos();

	uint8 getRollVal();

	void setGridPosition(uint16 row, uint16 col);

	void highlightTile(bool toggle);

	ATile* getEdgeTile(ETileEdge edge);

	APlaceable * getPlaceableOnVertex(EVertex vertex);

	bool hasPortOnVertex(EVertex vertex);

	EPort getPortOnVertex(EVertex vertex);

	void addPlaceableOnVertex(EVertex vertex, APlaceable * newStructure);

	UFUNCTION()
	void OnRep_rollValue();

	UFUNCTION()
	void OnRep_tileType();

	void setResourceType(EResourceType resourceType);

	EVertex getClosestVertex(FVector location);

	bool isVertexConnected(EVertex vertex, uint8 playerNum);

	APlaceableArea * getPlaceableAreaAtVertex(EVertex vertex);

	UFUNCTION(BlueprintCallable, Category = "TileFunctions")
	EResourceType getTileType();

	UFUNCTION(BlueprintCallable, Category = "TileFunctions")
	uint8 getRollValue();



private:

	UPROPERTY(ReplicatedUsing = OnRep_tileType)
	EResourceType tileType;

	UPROPERTY(EditAnywhere)
	TArray<APlaceableArea *> vertices;

	UPROPERTY(EditAnywhere)
	TArray<ATile *> edgeTiles;

	UPROPERTY(EditAnywhere)
	uint8 rowPos;

	UPROPERTY(EditAnywhere)
	uint8 colPos;

	UPROPERTY(ReplicatedUsing = OnRep_rollValue)
	uint8 rollValue;

	UPROPERTY(replicated)
	AStaticMeshActor* Token;


	
};
