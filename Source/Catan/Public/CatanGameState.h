// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/GameState.h"
#include "Tile.h"
#include "Enums/Resources.h"
#include "Enums/PurchaseType.h"
#include "Enums/DevCardType.h"
#include "DevelopmentCard.h"
#include "Robber.h"
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

	void setDevelopmentCards(TArray<EDevCardType> cards);

	void setRobber(ARobber* newRobber);

	// hands next deve card back and removes it from deck
	EDevCardType getNextDevCard();

	void moveRobberLocation(ATile * tile);

private:

	UPROPERTY(replicated)
	ARobber* Robber;

	TArray<ATile *> tiles;

	UPROPERTY(replicated)
	uint8 currentPlayerTurn;

	TArray<EDevCardType> developmentCards;
	

	
};
