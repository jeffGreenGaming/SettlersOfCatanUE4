// Fill out your copyright notice in the Description page of Project Settings.
#include "Catan.h"
#include "PlaceableArea.h"
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
	FVector Location(-170.0f, 20.0f, 110.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	TArray<AActor*> Tiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), Tiles);

	int rollValues[18] = { 2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12 };

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

		int SwapRoll1 = FMath::RandRange(0, 18 - 1);
		int SwapRoll2 = FMath::RandRange(0, 18 - 1);

		int tempRoll = rollValues[SwapRoll1];
		rollValues[SwapRoll1] = rollValues[SwapRoll2];
		rollValues[SwapRoll2] = tempRoll;

		int SwapTile1 = FMath::RandRange(0, Tiles.Num() - 1);
		int SwapTile2 = FMath::RandRange(0, Tiles.Num() - 1);

		EResourceType tempSwap = swapTileTypes[SwapTile1];
		swapTileTypes[SwapTile1] = swapTileTypes[SwapTile2];
		swapTileTypes[SwapTile2] = tempSwap;


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
		}
	}

}
void  ACatanGameMode::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage) {
	Super::InitGame(MapName,Options, ErrorMessage);

}

void ACatanGameMode::PostLogin(APlayerController *NewPlayer) {
	Super::PostLogin(NewPlayer);
	ACatanPlayerState * player_state = dynamic_cast<ACatanPlayerState *>(NewPlayer->PlayerState);
	if (player_state != nullptr) {
		player_state->setPlayerNum(NumPlayers);
	}

}



bool ACatanGameMode::isValidSettlementPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex, uint8 playerNum) {
	ACatanGameState * gameState = (ACatanGameState *)GameState;
	ATile * selectedTile = gameState->getTileFromCoordinates(selectionRow, selectionCol);

	//tiles for each edge -- two needed for each vertex to make sure is a valid placement
	ATile * topTile = selectedTile->getEdgeTile(ETileEdge::TileEdge_Top);
	ATile * bottomTile = selectedTile->getEdgeTile(ETileEdge::TileEdge_Bottom);
	ATile * topRightTile = selectedTile->getEdgeTile(ETileEdge::TileEdge_TopRight);
	ATile * bottomRightTile = selectedTile->getEdgeTile(ETileEdge::TileEdge_BottomRight);
	ATile * bottomLeftTile = selectedTile->getEdgeTile(ETileEdge::TileEdge_BottomLeft);
	ATile * topLeftTile = selectedTile->getEdgeTile(ETileEdge::TileEdge_TopLeft);

	bool bIsValid = !selectedTile->getPlaceableOnVertex(selectionVertex);
	// for each tile, check the 3 connected by the edges of the vertex
	switch (selectionVertex) {
	case EVertex::Vertex_TopRight:
		if (topRightTile != nullptr)
			bIsValid = bIsValid && !topRightTile->getPlaceableOnVertex(EVertex::Vertex_TopLeft);
		else if (topTile != nullptr)
			bIsValid = bIsValid && !topTile->getPlaceableOnVertex(EVertex::Vertex_MiddleRight);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_MiddleRight);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_TopLeft);
		break;
	case EVertex::Vertex_MiddleRight:
		if (topRightTile != nullptr)
			bIsValid = bIsValid && !topRightTile->getPlaceableOnVertex(EVertex::Vertex_BottomRight);
		else if (bottomRightTile != nullptr)
			bIsValid = bIsValid && !bottomRightTile->getPlaceableOnVertex(EVertex::Vertex_TopLeft);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_BottomRight);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_TopRight);
		break;
	case EVertex::Vertex_BottomRight:
		if (bottomRightTile != nullptr)
			bIsValid = bIsValid && !bottomRightTile->getPlaceableOnVertex(EVertex::Vertex_BottomLeft);
		else if (bottomTile != nullptr)
			bIsValid = bIsValid && !bottomTile->getPlaceableOnVertex(EVertex::Vertex_MiddleRight);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_MiddleRight);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_BottomLeft);
		break;
	case EVertex::Vertex_BottomLeft:
		if (bottomLeftTile != nullptr)
			bIsValid = bIsValid && !bottomLeftTile->getPlaceableOnVertex(EVertex::Vertex_BottomRight);
		else if (bottomTile != nullptr)
			bIsValid = bIsValid && !bottomTile->getPlaceableOnVertex(EVertex::Vertex_MiddleLeft);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_MiddleLeft);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_BottomRight);
		break;
	case EVertex::Vertex_MiddleLeft:
		if (topLeftTile != nullptr)
			bIsValid = bIsValid && !topLeftTile->getPlaceableOnVertex(EVertex::Vertex_BottomLeft);
		else if (bottomLeftTile != nullptr)
			bIsValid = bIsValid && !bottomLeftTile->getPlaceableOnVertex(EVertex::Vertex_TopLeft);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_TopLeft);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_BottomLeft);
		break;
	case EVertex::Vertex_TopLeft:
		if (topLeftTile != nullptr)
			bIsValid = bIsValid && !topLeftTile->getPlaceableOnVertex(EVertex::Vertex_TopRight);
		else if (topTile != nullptr)
			bIsValid = bIsValid && !topTile->getPlaceableOnVertex(EVertex::Vertex_MiddleLeft);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_MiddleLeft);
		bIsValid = bIsValid && !selectedTile->getPlaceableOnVertex(EVertex::Vertex_TopRight);
		break;
	}
	bIsValid = bIsValid && (selectedTile->isVertexConnected(selectionVertex, playerNum) || gamePhase != EGamePhase::GamePhase_MainGame);
	return bIsValid;


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
					gamePhase = EGamePhase::GamePhase_Placement2;
				}
				break;
		}
		gameState->setPlayerTurn(currentPlayerTurn);

	}
}

EGamePhase ACatanGameMode::getGamePhase() {
	return gamePhase;

}