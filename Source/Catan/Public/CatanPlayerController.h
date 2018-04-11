// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Tile.h"
#include "CatanDefaultCharacter.h"
#include "CatanPlayerState.h"
#include "Road.h"
#include "CatanWidget.h"
#include "Enums/InputMode.h"
#include "Enums/DevCardType.h"
#include "Enums/TileEdge.h"
#include "ParticleDefinitions.h"
#include "CatanPlayerController.generated.h"



/**
 * 
 */
UCLASS()
class CATAN_API ACatanPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	//beam particle system to be shown on vertex selection
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles")
	UParticleSystemComponent * vertexBeam;

	ACatanPlayerController();

	void BeginPlay() override;

	void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickPurchase();

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickBuySettlement();

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickBuyCity();

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickBuyRoad(bool bRoadBuilding);

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickBuyDevCard();

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickRoll();

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickConfirmRoadPlacement(bool bRoadBuilding);

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickMoveRobber(bool bUsedKnight);

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickMonopolyResource(EResourceType resourceType);

	UFUNCTION(BlueprintCallable, Category = "ClickFunctions")
	void clickUsePort(EPort portType, EResourceType resourceToTake, EResourceType resourceToGive);

	// used to check if we actually placed a road last turn or not
	UFUNCTION(BlueprintCallable, Category = "RoadPlacementFunctions")
	bool isLastPlacedRoadNull();

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable, Category = "HUDFunctions")
	void setHUD(TSubclassOf<class UCatanWidget> newHUD);

	UFUNCTION(BlueprintCallable, Category = "GameModeFunctions", Reliable, Server, WithValidation)
	void endTurnServer();

	UFUNCTION(Reliable, Client, WithValidation)
	void spawnDevCardClient(EDevCardType cardType);

	//function called when monopoly is used to give all resources of type to a player
	UFUNCTION(Reliable, Server, WithValidation)
	void giveAllResourcesServer(ACatanPlayerState * player_state, EResourceType resourceType);

	UFUNCTION(Reliable, Server, WithValidation)
	void usePortServer(ACatanPlayerState * player_state, EPort portType, EResourceType resourceToTake, EResourceType resourceToGive);

	UFUNCTION(Reliable, Server, WithValidation)
	void buyDevCardServer(ACatanPlayerState * player_state);

	UFUNCTION(Reliable, Server, WithValidation)
	void confirmRoadServer(uint8 row, uint8 col, EVertex vertex, bool bRoadBuilding);

	UFUNCTION(Reliable, Server, WithValidation)
	void rotateRoadServer(uint8 row, uint8 col, EVertex vertex);

	UFUNCTION(Reliable, Server, WithValidation)
	void buySettlementServer(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state);

	UFUNCTION(Reliable, Server, WithValidation)
	void buyCityServer(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state);

	UFUNCTION(Reliable, Server, WithValidation)
	void buyRoadServer(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state, bool bRoadBuilding);

	UFUNCTION(Reliable, Server, WithValidation)
	void rollServer(ACatanPlayerState * player_state);

	UFUNCTION(Reliable, Server, WithValidation)
	void moveRobberServer(uint8 row, uint8 col, EVertex vertex, bool bUsedKnight);

	void updateSelection();

	void rotateRoad();

private:

	TSubclassOf<class UCatanWidget> changeHUD;

	EInputMode inputMode;

	void highlightTiles();

	UPROPERTY()
	uint32 selectionRow;

	UPROPERTY()
	uint32 selectionCol;

	UPROPERTY()
	EVertex selectedVertex;

	// used for rotating road after placement
	UPROPERTY(replicated)
	ARoad * lastPlacedRoad;

	//index of last referenced placeablearea -- used to rotate road around
	uint8 lastPlaceRoadConnectionIndex;

	FVector getPlacementLocation(EVertex vertex, FVector tileLocation);
};
