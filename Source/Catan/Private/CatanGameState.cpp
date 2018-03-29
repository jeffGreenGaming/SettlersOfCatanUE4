// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "HelperFunctions.h"
#include "PlaceableArea.h"
#include "CatanPlayerState.h"
#include "CatanGameState.h"

// numBrick, numStone, numWood, numSheep, numWheat
const FResources ACatanGameState::settlementCost = { 1,0,1,1,1 };
const FResources ACatanGameState::cityCost = { 0,3,0,0,2 };
const FResources ACatanGameState::roadCost = { 1,0,1,0,0 };
const FResources ACatanGameState::developmentCardCost = { 0,1,0,1,1 };

ACatanGameState::ACatanGameState() {
	currentPlayerTurn = 1;
	bReplicates = true;
}

void ACatanGameState::BeginPlay() {
	TArray<AActor*> Tiles;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), Tiles);
	for (int i = 0; i < Tiles.Num(); i++) {
		ATile * tile = dynamic_cast<ATile*>(Tiles[i]);
		tiles.Add(tile);
	}
}


bool ACatanGameState::isValidSettlementPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex) {
	ATile * selectedTile = getTileFromCoordinates(selectionRow, selectionCol);
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
	return bIsValid;


}

bool ACatanGameState::isValidRoadPlacement(uint8 selectionRow, uint8 selectionCol, EVertex selectionVertex,uint8 playerNum) {
	ATile * selectedTile = getTileFromCoordinates(selectionRow, selectionCol);
	APlaceable * placeableAtVertex = selectedTile->getPlaceableOnVertex(selectionVertex);
	APlaceableArea * placeableArea = selectedTile->getPlaceableAreaAtVertex(selectionVertex);
	return placeableArea->hasPossibleRoadConnection() && 
		((placeableAtVertex != nullptr && placeableAtVertex->getOwnerNum() == playerNum) || selectedTile->isVertexConnected(selectionVertex, playerNum));
}

bool ACatanGameState::canAfford(FResources playerResources, EPurchaseType type) {
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

ATile * ACatanGameState::getTileFromCoordinates(uint32 row, uint32 col) {

	for (int i = 0; i < tiles.Num(); i++) {
		ATile * tile = tiles[i];
		if (tile->getColPos() == col && tile->getRowPos() == row) {
			return tile;
		}
	}
	return nullptr;
}

void ACatanGameState::endTurn() {
	currentPlayerTurn++;
	if (currentPlayerTurn > PlayerArray.Num()) {
		currentPlayerTurn = 1;
	}
}


void ACatanGameState::giveOutResources(uint8 rollNumber) {
	for (int i = 0; i < PlayerArray.Num(); i++) {
		ACatanPlayerState * player_state = dynamic_cast<ACatanPlayerState *>(PlayerArray[i]);
		player_state->giveResources(rollNumber);
	}

}

bool ACatanGameState::isMyTurn(uint8 playerNum) {
	return playerNum == currentPlayerTurn;
}

void ACatanGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACatanGameState, currentPlayerTurn);
}
