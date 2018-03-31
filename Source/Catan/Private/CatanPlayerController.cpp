// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "Settlement.h"
#include "CatanHUD.h"
#include "CatanGameState.h"
#include "CatanGameMode.h"
#include "PlaceableArea.h"
#include "CatanPlayerController.h"




ACatanPlayerController::ACatanPlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	selectionCol = 0;
	selectionRow = 0;
	bReplicates = true;
	bAlwaysRelevant = true;
	selectedVertex = EVertex::Vertex_BottomRight;
	inputMode = EInputMode::InputMode_Main;
	lastPlacedRoad = nullptr;
	lastPlaceRoadConnectionIndex = 0;
}


void ACatanPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ACatanPlayerController::UpdateSelection);
	InputComponent->BindAction("RotateRoad", IE_Pressed, this, &ACatanPlayerController::RotateRoad);

}

void ACatanPlayerController::clickConfirmRoadPlacement() {
	if (Role == ROLE_Authority) {
		ConfirmRoadServer(selectionRow, selectionCol, selectedVertex);
	}
	else {
		ConfirmRoadServer(selectionRow, selectionCol, selectedVertex);
	}
}

void ACatanPlayerController::RotateRoad() {
	if (Role == ROLE_Authority) {
		RotateRoadServer(selectionRow, selectionCol, selectedVertex);
	}
	else {
		RotateRoadServer(selectionRow, selectionCol, selectedVertex);
	}
}


void ACatanPlayerController::ConfirmRoadServer_Implementation(uint8 row, uint8 col, EVertex vertex) {

	if (lastPlacedRoad != nullptr) {
		ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
		ATile * selectedTile = gameState->getTileFromCoordinates(row, col);
		APlaceableArea * currentPlacementArea = selectedTile->getPlaceableAreaAtVertex(vertex);
		APlaceableArea * possibleConnection = currentPlacementArea->getPossibleRoadConnections()[lastPlaceRoadConnectionIndex];
		possibleConnection->addRoadConnection(currentPlacementArea, lastPlacedRoad);
		currentPlacementArea->addRoadConnection(possibleConnection, lastPlacedRoad);
		lastPlacedRoad = nullptr;
		lastPlaceRoadConnectionIndex = 0;
	}
}

bool ACatanPlayerController::ConfirmRoadServer_Validate(uint8 row, uint8 col, EVertex vertex) {
	return true;
}

void ACatanPlayerController::RotateRoadServer_Implementation(uint8 row, uint8 col, EVertex vertex) {

	if (lastPlacedRoad != nullptr) {
		ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
		lastPlaceRoadConnectionIndex++;
		ATile * selectedTile = gameState->getTileFromCoordinates(row, col);
		APlaceableArea * currentPlacementArea = selectedTile->getPlaceableAreaAtVertex(vertex);
		TArray<APlaceableArea *> possibleConnections = currentPlacementArea->getPossibleRoadConnections();
		if (lastPlaceRoadConnectionIndex >= possibleConnections.Num()) {
			lastPlaceRoadConnectionIndex = 0;
		}
		APlaceableArea * possibleConnection = possibleConnections[lastPlaceRoadConnectionIndex];

		while (possibleConnection->hasRoadConnection(currentPlacementArea)) {
			lastPlaceRoadConnectionIndex++;
			if (lastPlaceRoadConnectionIndex >= possibleConnections.Num()) {
				lastPlaceRoadConnectionIndex = 0;
			}
			possibleConnection = possibleConnections[lastPlaceRoadConnectionIndex];
		}
		FRotator newRotation = (lastPlacedRoad->GetActorLocation() - possibleConnection->GetActorLocation()).Rotation();
		lastPlacedRoad->SetActorRotation(newRotation + FRotator(0.0f,90.0f,0.0f));

	}
}

bool ACatanPlayerController::RotateRoadServer_Validate(uint8 row, uint8 col, EVertex vertex) {
	return true;
}

void ACatanPlayerController::UpdateSelection() {

	FHitResult hitResult;

	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), false, hitResult);

	ATile* clickedTile = dynamic_cast<ATile *>(hitResult.GetActor());

	if (clickedTile != nullptr) {
		selectionCol = clickedTile->getColPos();
		selectionRow = clickedTile->getRowPos();
		selectedVertex = clickedTile->getClosestVertex(hitResult.ImpactPoint);
	}

	TArray<AActor*> Tiles;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), Tiles);
	for (int i = 0; i < Tiles.Num(); i++) {
		ATile * tile = dynamic_cast<ATile*>(Tiles[i]);
		tile->highlightTile(false);
	}
	highlightTiles();

}

void ACatanPlayerController::highlightTiles() {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	switch (selectedVertex) {
		case EVertex::Vertex_BottomLeft:
			if (gameState->getTileFromCoordinates(selectionRow, selectionCol) != nullptr) {
				gameState->getTileFromCoordinates(selectionRow, selectionCol)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_BottomLeft) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_BottomLeft)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_Bottom) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_Bottom)->highlightTile(true);
			}
			break;
		case EVertex::Vertex_MiddleLeft:
			if (gameState->getTileFromCoordinates(selectionRow, selectionCol) != nullptr) {
				gameState->getTileFromCoordinates(selectionRow, selectionCol)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_BottomLeft) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_BottomLeft)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_TopLeft) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_TopLeft)->highlightTile(true);
			}
			break;
		case EVertex::Vertex_TopLeft:
			if (gameState->getTileFromCoordinates(selectionRow, selectionCol) != nullptr) {
				gameState->getTileFromCoordinates(selectionRow, selectionCol)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_TopLeft) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_TopLeft)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_Top) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_Top)->highlightTile(true);
			}
			break;
		case EVertex::Vertex_TopRight:
			if (gameState->getTileFromCoordinates(selectionRow, selectionCol) != nullptr) {
				gameState->getTileFromCoordinates(selectionRow, selectionCol)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_TopRight) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_TopRight)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_Top) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_Top)->highlightTile(true);
			}
			break;
		case EVertex::Vertex_MiddleRight:
			if (gameState->getTileFromCoordinates(selectionRow, selectionCol) != nullptr) {
				gameState->getTileFromCoordinates(selectionRow, selectionCol)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_TopRight) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_TopRight)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_BottomRight) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_BottomRight)->highlightTile(true);
			}
			break;
		case EVertex::Vertex_BottomRight:
			if (gameState->getTileFromCoordinates(selectionRow, selectionCol) != nullptr) {
				gameState->getTileFromCoordinates(selectionRow, selectionCol)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_BottomRight) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_BottomRight)->highlightTile(true);
				if (gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_Bottom) != nullptr)
					gameState->getTileFromCoordinates(selectionRow, selectionCol)->getEdgeTile(ETileEdge::TileEdge_Bottom)->highlightTile(true);
			}
			break;
	}
}

void ACatanPlayerController::clickBuySettlement() {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	if (gameState->isMyTurn(player_state->getPlayerNum())) {
		SpawnSettlement(selectionRow, selectionCol, selectedVertex, dynamic_cast<ACatanPlayerState *>(PlayerState));
	}
}

void ACatanPlayerController::clickBuyCity() {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	if (gameState->isMyTurn(player_state->getPlayerNum())) {
		SpawnCity(selectionRow, selectionCol, selectedVertex, dynamic_cast<ACatanPlayerState *>(PlayerState));
	}
}

void ACatanPlayerController::clickBuyRoad() {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	if (gameState->isMyTurn(player_state->getPlayerNum())) {
		SpawnRoad(selectionRow, selectionCol, selectedVertex, dynamic_cast<ACatanPlayerState *>(PlayerState));
	}
}

void ACatanPlayerController::clickPurchase() {
	inputMode = EInputMode::InputMode_Purchase;
	highlightTiles();
}

void ACatanPlayerController::clickRoll() {
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	RollServer(player_state);
}

void ACatanPlayerController::RollServer_Implementation(ACatanPlayerState * player_state) {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();

	if (gameState->isMyTurn(player_state->getPlayerNum())) {
		ACatanGameMode * gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
		gameMode->endTurn();
		uint8 rollValue = FMath::RandRange(1, 6) + FMath::RandRange(1, 6);
		gameState->giveOutResources(rollValue);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::FromInt(rollValue));
	}
}

bool ACatanPlayerController::RollServer_Validate(ACatanPlayerState * player_state) {
	return true;
}






void ACatanPlayerController::setHUD_Implementation(TSubclassOf<class UCatanWidget> newHUD) {
	ACatanHUD * HUD = dynamic_cast<ACatanHUD *>(GetHUD());
	HUD->setHUDClass(newHUD);
}

bool ACatanPlayerController::setHUD_Validate(TSubclassOf<class UCatanWidget> newHUD) {
	return true;
}

void ACatanPlayerController::SpawnSettlement_Implementation(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {

	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();

	ATile * selectedTile = gameState->getTileFromCoordinates(row, col);
	if (selectedTile != nullptr) {
		ACatanGameMode* gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
		if (player_state != nullptr && player_state->getNumSettlementsLeft() > 0 &&
			gameMode->isValidSettlementPlacement(row, col, vertex) && gameMode->canAfford(player_state->getResources(), EPurchaseType::Purchase_Settlement)) {
			
			FVector location = getPlacementLocation(vertex, selectedTile->GetActorLocation());
			FActorSpawnParameters spawnInfo;
			FRotator rotation(0.0f, 0.0f, 0.0f);

			ASettlement * settlement = GetWorld()->SpawnActor<ASettlement>(location, rotation, spawnInfo);
			settlement->setOwnerNum(player_state->getPlayerNum());

			player_state->addSettlement(settlement);
			player_state->payForPurchase(EPurchaseType::Purchase_Settlement);

			APlaceableArea * currentPlacementArea = selectedTile->getPlaceableAreaAtVertex(vertex);
			TArray<ATile *> connectedTiles = currentPlacementArea->getConnectedTiles();
			for (int i = 0; i < connectedTiles.Num(); i++) {
				player_state->addPerRoll(connectedTiles[i]->getRollVal(),connectedTiles[i]->getTileType());
			}

			selectedTile->addPlaceableOnVertex(vertex, settlement);
		}
	}
}

bool ACatanPlayerController::SpawnSettlement_Validate(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {
	return true;
}


void ACatanPlayerController::SpawnCity_Implementation(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {

	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ATile * selectedTile = gameState->getTileFromCoordinates(row, col);

	if (selectedTile != nullptr) {
		ACatanGameMode* gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
		ASettlement * settlement = dynamic_cast<ASettlement *>(selectedTile->getPlaceableOnVertex(vertex));
		if (gameState != nullptr && player_state != nullptr && player_state->getNumCitiesLeft() > 0 && settlement != nullptr
			  && settlement->getOwnerNum() == player_state->getPlayerNum() && gameMode->canAfford(player_state->getResources(), EPurchaseType::Purchase_City)) {

			FVector location = getPlacementLocation(vertex, selectedTile->GetActorLocation());
			FActorSpawnParameters spawnInfo;
			FRotator rotation(0.0f, 0.0f, 0.0f);

			ACity * city = GetWorld()->SpawnActor<ACity>(location, rotation, spawnInfo);
			city->setOwnerNum(player_state->getPlayerNum());

			settlement->Destroy();

			player_state->addCity(city);
			player_state->payForPurchase(EPurchaseType::Purchase_City);

			APlaceableArea * currentPlacementArea = selectedTile->getPlaceableAreaAtVertex(vertex);
			TArray<ATile *> connectedTiles = currentPlacementArea->getConnectedTiles();
			for (int i = 0; i < connectedTiles.Num(); i++) {
				player_state->addPerRoll(connectedTiles[i]->getRollVal(), connectedTiles[i]->getTileType());
			}

			selectedTile->addPlaceableOnVertex(vertex, city);
		}
	}
}

bool ACatanPlayerController::SpawnCity_Validate(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {
	return true;
}



void ACatanPlayerController::SpawnRoad_Implementation(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {

	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ATile * selectedTile = gameState->getTileFromCoordinates(row, col);
	if (selectedTile != nullptr) {
		ACatanGameMode* gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
		if (player_state != nullptr && player_state->getNumRoadsLeft() > 0 &&
			gameMode->isValidRoadPlacement(row, col, vertex, player_state->getPlayerNum()) && 
			gameMode->canAfford(player_state->getResources(), EPurchaseType::Purchase_Road)) {

			FVector location = getPlacementLocation(vertex, selectedTile->GetActorLocation());
			FActorSpawnParameters spawnInfo;
			FRotator rotation(0.0f, 0.0f, 0.0f);

			ARoad * road = GetWorld()->SpawnActor<ARoad>(location, rotation, spawnInfo);
			road->setOwnerNum(player_state->getPlayerNum());

			lastPlacedRoad = road;
			RotateRoadServer(row,col,vertex);

			player_state->addRoad(road);
			player_state->payForPurchase(EPurchaseType::Purchase_Road);

			UClass* roadUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/RoadUI.RoadUI_C"));
			setHUD(roadUI);

		}
	}
}

bool ACatanPlayerController::SpawnRoad_Validate(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {
	return true;
}

FVector ACatanPlayerController::getPlacementLocation(EVertex vertex, FVector tileLocation) {
	//TODO: remove constants
		FVector location = tileLocation;
		switch (vertex) {
			case EVertex::Vertex_BottomRight:
				location.Y = location.Y + 4.0f;
				location.X = location.X - 7.0f;
				break;
			case EVertex::Vertex_BottomLeft:
				location.Y = location.Y - 4.0f;
				location.X = location.X - 7.0f;
				break;
			case EVertex::Vertex_MiddleRight:
				location.Y = location.Y + 8.0f;
				break;
			case EVertex::Vertex_MiddleLeft:
				location.Y = location.Y - 8.0f;
				break;
			case EVertex::Vertex_TopRight:
				location.Y = location.Y + 4.0f;
				location.X = location.X + 7.0f;
				break;
			case EVertex::Vertex_TopLeft:
				location.Y = location.Y - 4.0f;
				location.X = location.X + 7.0f;
				break;
		}

		location.Z = location.Z + 1.0f;
		return location;
}


void swapHUD() {


}