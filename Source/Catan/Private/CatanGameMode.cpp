// Fill out your copyright notice in the Description page of Project Settings.
#include "Catan.h"
#include "PlaceableArea.h"
#include "Enums/DevCardType.h"
#include "Robber.h"
#include "CatanGameMode.h"

// numBrick, numStone, numWood, numSheep, numWheat
const FResources ACatanGameMode::settlementCost = { 1,0,1,1,1 };
const FResources ACatanGameMode::cityCost = { 0,3,0,0,2 };
const FResources ACatanGameMode::roadCost = { 1,0,1,0,0 };
const FResources ACatanGameMode::developmentCardCost = { 0,1,0,1,1 };

ACatanGameMode::ACatanGameMode() {
	GameStateClass = ACatanGameState::StaticClass();
	HUDClass = ACatanHUD::StaticClass();
	PlayerControllerClass = ACatanPlayerController::StaticClass();
	DefaultPawnClass = ACatanDefaultCharacter::StaticClass();
	PlayerStateClass = ACatanPlayerState::StaticClass();
	gamePhase = EGamePhase::GamePhase_Placement1;
}

void  ACatanGameMode::StartPlay() {

	Super::StartPlay();
	ACatanGameState * gameState = (ACatanGameState *)GameState;
	uint8 numRollValues = 18;


	TArray<AActor*> Tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), Tiles);

	int rollValues[18] = { 2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12 };

	TArray<EDevCardType> devCardTypes;
	for (int i = 0; i < 14; i++) {
		devCardTypes.Add(EDevCardType::DevCardType_Knight);
	}

	for (int i = 0; i < 5; i++) {
		devCardTypes.Add(EDevCardType::DevCardType_VictoryPoints);
	}

	for (int i = 0; i < 2; i++) {
		devCardTypes.Add(EDevCardType::DevCardType_YearOfPlenty);
		devCardTypes.Add(EDevCardType::DevCardType_RoadBuilding);
		devCardTypes.Add(EDevCardType::DevCardType_Monopoly);
	}

	// use these so replicated properties only updated once -- represents the future tile types
	EResourceType swapTileTypes[19] = {
		EResourceType::ResourceType_Brick,EResourceType::ResourceType_Brick,EResourceType::ResourceType_Brick,
		EResourceType::ResourceType_Stone,EResourceType::ResourceType_Stone,EResourceType::ResourceType_Stone,
		EResourceType::ResourceType_Wood,EResourceType::ResourceType_Wood,EResourceType::ResourceType_Wood,EResourceType::ResourceType_Wood,
		EResourceType::ResourceType_Sheep,EResourceType::ResourceType_Sheep,EResourceType::ResourceType_Sheep,EResourceType::ResourceType_Sheep,
		EResourceType::ResourceType_Wheat,EResourceType::ResourceType_Wheat,EResourceType::ResourceType_Wheat,EResourceType::ResourceType_Wheat,
		EResourceType::ResourceType_Desert
	};

	for (int i = 0; i < 100; i++) {

		int SwapRoll1 = FMath::RandRange(0, numRollValues - 1);
		int SwapRoll2 = FMath::RandRange(0, numRollValues - 1);

		int tempRoll = rollValues[SwapRoll1];
		rollValues[SwapRoll1] = rollValues[SwapRoll2];
		rollValues[SwapRoll2] = tempRoll;

		int SwapTile1 = FMath::RandRange(0, Tiles.Num() - 1);
		int SwapTile2 = FMath::RandRange(0, Tiles.Num() - 1);

		EResourceType tempSwap = swapTileTypes[SwapTile1];
		swapTileTypes[SwapTile1] = swapTileTypes[SwapTile2];
		swapTileTypes[SwapTile2] = tempSwap;

		int SwapDev1 = FMath::RandRange(0, devCardTypes.Num() - 1);
		int SwapDev2 = FMath::RandRange(0, devCardTypes.Num() - 1);

		devCardTypes.Swap(SwapDev1, SwapDev2);

	}

	int rollCount = 0; //have to account for desert tile not having a roll
	for (int i = 0; i < Tiles.Num(); i++) {
		ATile * tile = dynamic_cast<ATile*> (Tiles[i]);
		if (tile != nullptr) {
			tile->setResourceType(swapTileTypes[i]);
			if (swapTileTypes[i] != EResourceType::ResourceType_Desert) {
				tile->setRollValue(rollValues[rollCount]);
				rollCount++;
			}
			else {
				FActorSpawnParameters SpawnInfo;
				ARobber * Robber = GetWorld()->SpawnActor<ARobber>(ARobber::StaticClass(), tile->GetActorLocation() + FVector(0.0f, 0.0f, 0.5f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
				Robber->setCurrentTile(tile);
				gameState->setRobber(Robber);
			}
		}
	}


	if (gameState != nullptr) {
		gameState->setDevelopmentCards(devCardTypes);
	}

}


void ACatanGameMode::GenericPlayerInitialization(AController* Controller) {
	Super::GenericPlayerInitialization(Controller);
	ACatanPlayerState * player_state = dynamic_cast<ACatanPlayerState *>(Controller->PlayerState);
	if (player_state != nullptr) {
		player_state->setPlayerNum(NumPlayers);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::FromInt(NumPlayers));
	}
}

bool ACatanGameMode::isValidSettlementPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex, uint8 playerNum) {
	ACatanGameState * gameState = (ACatanGameState *)GameState;
	ATile * selectedTile = gameState->getTileFromCoordinates(selectionRow, selectionCol);
	APlaceableArea * selectedPlaceableArea = selectedTile->getPlaceableAreaAtVertex(selectionVertex);
	TArray<APlaceableArea *> checkablePlaceableAreas = selectedPlaceableArea->getPossibleRoadConnections();
	bool bIsValid = !selectedTile->getPlaceableOnVertex(selectionVertex);
	for (int i = 0; i < checkablePlaceableAreas.Num(); i++) {
		if (checkablePlaceableAreas[i]->getTakenStructure() != nullptr) {
			bIsValid =  false;
		}
	}
	return bIsValid && (selectedTile->isVertexConnected(selectionVertex, playerNum) || gamePhase != EGamePhase::GamePhase_MainGame);
}

bool ACatanGameMode::isValidRoadPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex, uint8 playerNum) {
	ACatanGameState * gameState = (ACatanGameState *)GameState;
	ATile * selectedTile = gameState->getTileFromCoordinates(selectionRow, selectionCol);
	APlaceable * placeableAtVertex = selectedTile->getPlaceableOnVertex(selectionVertex);
	APlaceableArea * placeableArea = selectedTile->getPlaceableAreaAtVertex(selectionVertex);
	return placeableArea->hasPossibleRoadConnection() &&
		((placeableAtVertex != nullptr && placeableAtVertex->getOwnerNum() == playerNum) || selectedTile->isVertexConnected(selectionVertex, playerNum));
}

bool ACatanGameMode::canAfford(FResources playerResources, EPurchaseType type) {
	FResources placeableCost;
	switch (type) {
	case EPurchaseType::Purchase_Settlement:
		placeableCost = settlementCost;
		break;
	case EPurchaseType::Purchase_City:
		placeableCost = cityCost;
		break;
	case EPurchaseType::Purchase_Road:
		placeableCost = roadCost;
		break;
	case EPurchaseType::Purchase_DevelopmentCard:
		placeableCost = developmentCardCost;
		break;
	default:
		return false;
	}
	return (playerResources.numBrick >= placeableCost.numBrick
		&& playerResources.numWood >= placeableCost.numWood
		&& playerResources.numWheat >= placeableCost.numWheat
		&& playerResources.numSheep >= placeableCost.numSheep
		&& playerResources.numStone >= placeableCost.numStone);
}

void ACatanGameMode::endTurn() {
	ACatanGameState * gameState = (ACatanGameState *)GameState;

	if (GameState != nullptr) {
		uint8 currentPlayerTurn = gameState->getPlayerTurn();
		switch (gamePhase) {
			case EGamePhase::GamePhase_Placement1:
				currentPlayerTurn++;
				if (currentPlayerTurn > gameState->PlayerArray.Num()) {
					currentPlayerTurn--;
					gamePhase = EGamePhase::GamePhase_Placement2;
				}
				break;
			case EGamePhase::GamePhase_Placement2:
				currentPlayerTurn--;
				if (currentPlayerTurn == 0) {
					currentPlayerTurn = 1;
					gamePhase = EGamePhase::GamePhase_MainGame;
				}
				break;
			case EGamePhase::GamePhase_MainGame:
				currentPlayerTurn++;
				if (currentPlayerTurn > gameState->PlayerArray.Num()) {
					currentPlayerTurn = 1;
				}
				break;
		}
		gameState->setPlayerTurn(currentPlayerTurn);

	}
}

EGamePhase ACatanGameMode::getGamePhase() {
	return gamePhase;

}

void ACatanGameMode::moveRobber(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex) {
	ACatanGameState * gameState = (ACatanGameState *)GameState;
	TArray<APlayerState* > players = gameState->PlayerArray;
	ATile * selectedTile = gameState->getTileFromCoordinates(selectionRow, selectionCol);

	ATile * currentRobberTile = gameState->getRobberTile();
	int numVertices = 6;

	gameState->moveRobberLocation(selectedTile);
	
	//give back per rolls of tile robber is leaving
	for (int i = 0; i < numVertices; i++) {
		APlaceable * placeable = currentRobberTile->getPlaceableOnVertex(EVertex(i));
		if (dynamic_cast<ASettlement*>(placeable)) {
			uint8 playerNum = placeable->getOwnerNum();
			ACatanPlayerState * playerState = dynamic_cast<ACatanPlayerState*>(players[playerNum - 1]);
			playerState->addPerRoll(currentRobberTile->getRollVal(), currentRobberTile->getTileType(), 1);
		}
		else if (dynamic_cast<ACity*>(placeable)) {
			uint8 playerNum = placeable->getOwnerNum();
			ACatanPlayerState * playerState = dynamic_cast<ACatanPlayerState*>(players[playerNum - 1]);
			playerState->addPerRoll(currentRobberTile->getRollVal(), currentRobberTile->getTileType(), 2);
		}
	}

	//take away per rolls of tile robber is going to 
	for (int i = 0; i < numVertices; i++) {
		APlaceable * placeable = selectedTile->getPlaceableOnVertex(EVertex(i));
		if (dynamic_cast<ASettlement*>(placeable)) {
			uint8 playerNum = placeable->getOwnerNum();
			ACatanPlayerState * playerState = dynamic_cast<ACatanPlayerState*>(players[playerNum - 1]);
			playerState->addPerRoll(selectedTile->getRollVal(), selectedTile->getTileType(), -1);
		}
		else if (dynamic_cast<ACity*>(placeable)) {
			uint8 playerNum = placeable->getOwnerNum();
			ACatanPlayerState * playerState = dynamic_cast<ACatanPlayerState*>(players[playerNum - 1]);
			playerState->addPerRoll(selectedTile->getRollVal(), selectedTile->getTileType(), -2);
		}
	}


}

void ACatanGameMode::makeClientsRemoveCards() {
	ACatanGameState * gameState = (ACatanGameState *)GameState;
	TArray<APlayerState* > players = gameState->PlayerArray;
	for (int i = 0; i < gameState->PlayerArray.Num(); i++) {
		if (ACatanPlayerState * playerState = dynamic_cast<ACatanPlayerState*>(players[i])) {
			FResources playerResources = playerState->getResources();
			ACatanPlayerController * controller = dynamic_cast<ACatanPlayerController*>(playerState->GetOwner());
			if (controller != nullptr && playerResources.getTotalNumResources() > 7) {
				UClass* throwAwayUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/ThrowAwayResourcesUI.ThrowAwayResourcesUI_C"));
				controller->setHUD(throwAwayUI);
			}
		}
	}
}