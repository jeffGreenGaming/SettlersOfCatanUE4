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
	void InitGame(const FString & MapName,const FString & Options,FString & ErrorMessage) override;
	void StartPlay() override;

	void PostLogin(APlayerController *NewPlayer) override;

	bool isValidSettlementPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex, uint8 playerNum);

	bool isValidRoadPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex, uint8 playerNum);

	bool canAfford(FResources playerResources, EPurchaseType type);

	UFUNCTION(BlueprintCallable, Category = "GameModeFunctions")
	void endTurn();

	void GenericPlayerInitialization(AController* Controller) override;

	EGamePhase getGamePhase();

private:
	EGamePhase gamePhase;

};
