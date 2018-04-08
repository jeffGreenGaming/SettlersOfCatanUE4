// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "Placeable.h"
#include "CatanGameState.h"
#include "CatanHUD.h"
#include "CatanPlayerController.h"
#include "CatanDefaultCharacter.h"
#include "CatanPlayerState.h"
#include "Enums/GamePhase.h"
#include "Enums/ResourceType.h"
#include "CatanGameMode.generated.h"

/**
 * 
 */



UCLASS()
class CATAN_API ACatanGameMode : public AGameMode
{
	GENERATED_BODY()

		
public:

	static const FResources settlementCost;
	static const FResources cityCost;
	static const FResources roadCost;
	static const FResources developmentCardCost;

	ACatanGameMode();


	void StartPlay() override;

	bool isValidSettlementPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex, uint8 playerNum);

	bool isValidRoadPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex, uint8 playerNum);

	bool canAfford(FResources playerResources, EPurchaseType type);

	void endTurn();

	void GenericPlayerInitialization(AController* Controller) override;

	EGamePhase getGamePhase();

	void moveRobber(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex);

	void makeClientsRemoveCards();

	void useMonopoly(ACatanPlayerState * playerToGive, EResourceType resourceType);

	void usePort(ACatanPlayerState * playerUsing, EPort portType, EResourceType resourceToTake, EResourceType resourceToGive);

private:
	EGamePhase gamePhase;

};
