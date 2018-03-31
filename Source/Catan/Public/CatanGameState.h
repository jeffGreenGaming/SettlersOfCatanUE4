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



	ACatanGameState();
	virtual void BeginPlay() override;

	ATile * getTileFromCoordinates(uint32 row, uint32 col);

	void giveOutResources(uint8 rollNumber);

	bool isMyTurn(uint8 playerNum);

	void setPlayerTurn(uint8 playerNum);

	uint8 getPlayerTurn();

private:

	TArray<ATile *> tiles;

	UPROPERTY(replicated)
	uint8 currentPlayerTurn;
	
	
	
};