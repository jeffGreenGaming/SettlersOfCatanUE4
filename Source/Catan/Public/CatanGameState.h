// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/GameState.h"
#include "Tile.h"
#include "Enums/Resources.h"
#include "Enums/PurchaseType.h"
#include "CatanGameState.generated.h"


/**
 * 
 */
UCLASS()
class CATAN_API ACatanGameState : public AGameState
{
	GENERATED_BODY()

public:

	static const FResources settlementCost;
	static const FResources cityCost;
	static const FResources roadCost;
	static const FResources developmentCardCost;

	ACatanGameState();
	virtual void BeginPlay() override;

	bool isValidSettlementPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex);

	bool isValidRoadPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex, uint8 playerNum);

	bool canAfford(FResources playerResources, EPurchaseType type);

	ATile * getTileFromCoordinates(uint32 row, uint32 col);

	void endTurn();

	void giveOutResources(uint8 rollNumber);

	bool isMyTurn(uint8 playerNum);

private:

	TArray<ATile *> tiles;

	UPROPERTY(replicated)
	uint8 currentPlayerTurn;
	
	
	
};
