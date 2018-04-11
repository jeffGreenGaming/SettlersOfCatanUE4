// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "Settlement.h"
#include "CatanHUD.h"
#include "CatanGameState.h"
#include "CatanGameMode.h"
#include "PlaceableArea.h"
#include "DevCards/KnightCard.h"
#include "DevCards/MonopolyCard.h"
#include "DevCards/RoadBuildingCard.h"
#include "DevCards/YearOfPlentyCard.h"
#include "DevCards/VictoryPointCard.h"
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

	vertexBeam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam"));
	ConstructorHelpers::FObjectFinder< UParticleSystem> particleRef(TEXT("/Game/Content/Particles/Beam.Beam"));
	vertexBeam->SetTemplate(particleRef.Object);

}


void ACatanPlayerController::BeginPlay() {
	Super::BeginPlay();

	//start up the particle system for the vertex beam
	vertexBeam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), vertexBeam->Template, FVector(-40.0f, -6.0f, 400.0f), FRotator(90.0f, 0.0f, 0.0f), false);
	vertexBeam->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));

}
void ACatanPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ACatanPlayerController::updateSelection);
	InputComponent->BindAction("RotateRoad", IE_Pressed, this, &ACatanPlayerController::rotateRoad);

}

void ACatanPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACatanPlayerController, lastPlacedRoad);

}

void ACatanPlayerController::clickPurchase() {
	// can possibly remove
}

void ACatanPlayerController::clickBuySettlement() {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	if (gameState->isMyTurn(player_state->getPlayerNum())) {
		buySettlementServer(selectionRow, selectionCol, selectedVertex, dynamic_cast<ACatanPlayerState *>(PlayerState));
	}
}

void ACatanPlayerController::clickBuyCity() {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	if (gameState->isMyTurn(player_state->getPlayerNum())) {
		buyCityServer(selectionRow, selectionCol, selectedVertex, dynamic_cast<ACatanPlayerState *>(PlayerState));
	}
}

void ACatanPlayerController::clickBuyRoad(bool bRoadBuilding) {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	if (gameState->isMyTurn(player_state->getPlayerNum())) {
		buyRoadServer(selectionRow, selectionCol, selectedVertex, dynamic_cast<ACatanPlayerState *>(PlayerState), bRoadBuilding);
	}
}


void ACatanPlayerController::clickBuyDevCard() {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	if (gameState->isMyTurn(player_state->getPlayerNum())) {
		buyDevCardServer(dynamic_cast<ACatanPlayerState *>(PlayerState));
	}

}

void ACatanPlayerController::clickRoll() {
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	rollServer(player_state);
}

void ACatanPlayerController::clickConfirmRoadPlacement(bool bRoadBuilding) {
	confirmRoadServer(selectionRow, selectionCol, selectedVertex, bRoadBuilding);
}

void ACatanPlayerController::clickMoveRobber(bool bUsedKnight) {
	moveRobberServer(selectionRow, selectionCol, selectedVertex, bUsedKnight);
}

void ACatanPlayerController::clickMonopolyResource(EResourceType resourceType) {
	ACatanPlayerState * player_state = (ACatanPlayerState *)PlayerState;
	giveAllResourcesServer(player_state, resourceType);
}	

void ACatanPlayerController::clickUsePort(EPort portType, EResourceType resourceToTake, EResourceType resourceToGive) {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ACatanPlayerState * playerState = (ACatanPlayerState *)PlayerState;
	if (gameState->isMyTurn(playerState->getPlayerNum())) {
		usePortServer(playerState, portType, resourceToTake, resourceToGive);
	}
}

bool ACatanPlayerController::isLastPlacedRoadNull() {
	return lastPlacedRoad == nullptr;
}

void ACatanPlayerController::setHUD_Implementation(TSubclassOf<class UCatanWidget> newHUD) {
	ACatanHUD * HUD = dynamic_cast<ACatanHUD *>(GetHUD());
	HUD->setHUDClass(newHUD);
}

bool ACatanPlayerController::setHUD_Validate(TSubclassOf<class UCatanWidget> newHUD) {
	return true;
}

void ACatanPlayerController::endTurnServer_Implementation() {
	ACatanGameMode * gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
	gameMode->endTurn();
}

bool ACatanPlayerController::endTurnServer_Validate() {
	return true;
}


void ACatanPlayerController::spawnDevCardClient_Implementation(EDevCardType cardType) {

	FVector location(-95.0f, -6.0f, 90.5f);
	FActorSpawnParameters spawnInfo;
	FRotator rotation(0.0f, 180.0f, 0.0f);

	ADevelopmentCard * developmentCard = nullptr;
	switch (cardType) {
	case EDevCardType::DevCardType_Knight:
		developmentCard = GetWorld()->SpawnActor<AKnightCard>(location, rotation, spawnInfo);
		break;
	case EDevCardType::DevCardType_Monopoly:
		developmentCard = GetWorld()->SpawnActor<AMonopolyCard>(location, rotation, spawnInfo);
		break;
	case EDevCardType::DevCardType_YearOfPlenty:
		developmentCard = GetWorld()->SpawnActor<AYearOfPlentyCard>(location, rotation, spawnInfo);
		break;
	case EDevCardType::DevCardType_RoadBuilding:
		developmentCard = GetWorld()->SpawnActor<ARoadBuildingCard>(location, rotation, spawnInfo);
		break;
	case EDevCardType::DevCardType_VictoryPoints:
		developmentCard = GetWorld()->SpawnActor<AVictoryPointCard>(location, rotation, spawnInfo);
		break;

	}
	if (developmentCard != nullptr) {
		developmentCard->SetOwner(this);
	}
}

bool ACatanPlayerController::spawnDevCardClient_Validate(EDevCardType cardType) {
	return true;
}


void ACatanPlayerController::giveAllResourcesServer_Implementation(ACatanPlayerState * player_state, EResourceType resourceType) {
	ACatanGameMode* gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
	gameMode->useMonopoly(player_state, resourceType);
}

bool ACatanPlayerController::giveAllResourcesServer_Validate(ACatanPlayerState * player_state, EResourceType resourceType) {
	return true;
}

void ACatanPlayerController::usePortServer_Implementation(ACatanPlayerState * player_state, EPort portType, EResourceType resourceToTake, EResourceType resourceToGive) {
	ACatanGameMode* gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
	gameMode->usePort(player_state,portType,resourceToTake,resourceToGive);
}

bool ACatanPlayerController::usePortServer_Validate(ACatanPlayerState * player_state, EPort portType, EResourceType resourceToTake, EResourceType resourceToGive) {
	return true;
}

void ACatanPlayerController::buyDevCardServer_Implementation(ACatanPlayerState * player_state) {

	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ACatanGameMode* gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
	if (player_state != nullptr && gameState != nullptr && gameMode->canAfford(player_state->getResources(), EPurchaseType::Purchase_Settlement)) {

		EDevCardType cardType = gameState->getNextDevCard();
		spawnDevCardClient(cardType);
		player_state->payForPurchase(EPurchaseType::Purchase_DevelopmentCard);

	}

}


bool ACatanPlayerController::buyDevCardServer_Validate(ACatanPlayerState * player_state) {
	return true;

}


void ACatanPlayerController::confirmRoadServer_Implementation(uint8 row, uint8 col, EVertex vertex, bool bRoadBuilding) {

	if (lastPlacedRoad != nullptr) {
		ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
		ATile * selectedTile = gameState->getTileFromCoordinates(row, col);
		APlaceableArea * currentPlacementArea = selectedTile->getPlaceableAreaAtVertex(vertex);
		APlaceableArea * possibleConnection = currentPlacementArea->getPossibleRoadConnections()[lastPlaceRoadConnectionIndex];
		possibleConnection->addRoadConnection(currentPlacementArea, lastPlacedRoad);
		currentPlacementArea->addRoadConnection(possibleConnection, lastPlacedRoad);
		lastPlacedRoad = nullptr;
		lastPlaceRoadConnectionIndex = 0;

		ACatanGameMode * gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
		if (!bRoadBuilding) {
			//set custom UI for placement phase
			if (gameMode->getGamePhase() == EGamePhase::GamePhase_Placement1) {
				UClass* placementRoadUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/PlacementSettlementUI.PlacementSettlementUI_C"));
				setHUD(placementRoadUI);
			}
			//go to main game UI
			else if (gameMode->getGamePhase() == EGamePhase::GamePhase_Placement2) {
				UClass* placementRoadUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/MainUI.MainUI_C"));
				setHUD(placementRoadUI);
			}
		}

	}
}

bool ACatanPlayerController::confirmRoadServer_Validate(uint8 row, uint8 col, EVertex vertex, bool bRoadBuilding) {
	return true;
}


void ACatanPlayerController::rotateRoadServer_Implementation(uint8 row, uint8 col, EVertex vertex) {

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
		lastPlacedRoad->SetActorRotation(newRotation + FRotator(0.0f, 90.0f, 0.0f));

	}
}

bool ACatanPlayerController::rotateRoadServer_Validate(uint8 row, uint8 col, EVertex vertex) {
	return true;
}


void ACatanPlayerController::buySettlementServer_Implementation(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {

	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();

	ATile * selectedTile = gameState->getTileFromCoordinates(row, col);
	if (selectedTile != nullptr) {
		ACatanGameMode* gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
		if (player_state != nullptr && player_state->getNumSettlementsLeft() > 0 &&
			gameMode->isValidSettlementPlacement(row, col, vertex, player_state->getPlayerNum()) && gameMode->canAfford(player_state->getResources(), EPurchaseType::Purchase_Settlement)) {

			FVector location = getPlacementLocation(vertex, selectedTile->GetActorLocation());
			FActorSpawnParameters spawnInfo;
			FRotator rotation(0.0f, 0.0f, 0.0f);

			ASettlement * settlement = GetWorld()->SpawnActor<ASettlement>(location, rotation, spawnInfo);
			settlement->setOwnerNum(player_state->getPlayerNum());

			player_state->addSettlement(settlement);
			player_state->payForPurchase(EPurchaseType::Purchase_Settlement);

			// add resources on the roll value
			APlaceableArea * currentPlacementArea = selectedTile->getPlaceableAreaAtVertex(vertex);
			TArray<ATile *> connectedTiles = currentPlacementArea->getConnectedTiles();
			for (int i = 0; i < connectedTiles.Num(); i++) {
				player_state->addPerRoll(connectedTiles[i]->getRollVal(), connectedTiles[i]->getTileType(),1);
			}

			//add port
			if (selectedTile->hasPortOnVertex(selectedVertex)) {
				player_state->addPort(selectedTile->getPortOnVertex(selectedVertex));
			}

			//if we are in the intial placing phases of the game we need special HUD cycles
			if (gameMode->getGamePhase() == EGamePhase::GamePhase_Placement1 || gameMode->getGamePhase() == EGamePhase::GamePhase_Placement2) {
				UClass* placementRoadUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/PlacementRoadUI.PlacementRoadUI_C"));
				setHUD(placementRoadUI);
			}

			selectedTile->addPlaceableOnVertex(vertex, settlement);
		}
	}
}

bool ACatanPlayerController::buySettlementServer_Validate(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {
	return true;
}


void ACatanPlayerController::buyCityServer_Implementation(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {

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
				player_state->addPerRoll(connectedTiles[i]->getRollVal(), connectedTiles[i]->getTileType(),1);
			}

			selectedTile->addPlaceableOnVertex(vertex, city);
		}
	}
}

bool ACatanPlayerController::buyCityServer_Validate(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state) {
	return true;
}



void ACatanPlayerController::buyRoadServer_Implementation(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state, bool bRoadBuilding) {

	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();
	ATile * selectedTile = gameState->getTileFromCoordinates(row, col);
	if (selectedTile != nullptr) {
		ACatanGameMode* gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
		if (player_state != nullptr && player_state->getNumRoadsLeft() > 0 &&
			gameMode->isValidRoadPlacement(row, col, vertex, player_state->getPlayerNum()) &&
			(gameMode->canAfford(player_state->getResources(), EPurchaseType::Purchase_Road) || bRoadBuilding)) {

			FVector location = getPlacementLocation(vertex, selectedTile->GetActorLocation());
			FActorSpawnParameters spawnInfo;
			FRotator rotation(0.0f, 0.0f, 0.0f);

			ARoad * road = GetWorld()->SpawnActor<ARoad>(location, rotation, spawnInfo);
			road->setOwnerNum(player_state->getPlayerNum());

			lastPlacedRoad = road;
			rotateRoadServer(row, col, vertex);

			player_state->addRoad(road);
			if (!bRoadBuilding) {
				player_state->payForPurchase(EPurchaseType::Purchase_Road);
				//if we are in the intial placing phases of the game we need special HUD cycles
				if (gameMode->getGamePhase() == EGamePhase::GamePhase_Placement1 || gameMode->getGamePhase() == EGamePhase::GamePhase_Placement2) {
					UClass* RoadUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/PlacementConfirmRoadUI.PlacementConfirmRoadUI_C"));
					setHUD(RoadUI);
				}
				else {
					UClass* roadUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/RoadUI.RoadUI_C"));
					setHUD(roadUI);
				}
			}

		}
	}
}

bool ACatanPlayerController::buyRoadServer_Validate(uint8 row, uint8 col, EVertex vertex, ACatanPlayerState * player_state, bool bRoadBuilding) {
	return true;
}


void ACatanPlayerController::rollServer_Implementation(ACatanPlayerState * player_state) {
	ACatanGameState* gameState = (ACatanGameState*)GetWorld()->GetGameState();

	if (gameState->isMyTurn(player_state->getPlayerNum())) {
		endTurnServer();
		uint8 rollValue = FMath::RandRange(1, 6) + FMath::RandRange(1, 6);
		if (rollValue == 7) {
			UClass* robberUI = LoadObject<UClass>(nullptr, TEXT("/Game/Content/Blueprints/UI/RobberUI.RobberUI_C"));
			setHUD(robberUI);
		}
		gameState->giveOutResourcesOnRollValue(rollValue);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::FromInt(rollValue));
	}
}

bool ACatanPlayerController::rollServer_Validate(ACatanPlayerState * player_state) {
	return true;
}

void ACatanPlayerController::moveRobberServer_Implementation(uint8 row, uint8 col, EVertex vertex, bool bUsedKnight) {
	ACatanGameMode * gameMode = (ACatanGameMode*)GetWorld()->GetAuthGameMode();
	gameMode->moveRobber(row, col, vertex);
	if (!bUsedKnight) {
		gameMode->makeClientsRemoveCards();
	}
}

bool ACatanPlayerController::moveRobberServer_Validate(uint8 row, uint8 col, EVertex vertex, bool bUsedKnight) {
	return true;
}

void ACatanPlayerController::updateSelection() {

	FHitResult hitResult;

	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), false, hitResult);

	ATile* clickedTile = dynamic_cast<ATile *>(hitResult.GetActor());

	if (clickedTile != nullptr) {
		selectionCol = clickedTile->getColPos();
		selectionRow = clickedTile->getRowPos();
		selectedVertex = clickedTile->getClosestVertex(hitResult.ImpactPoint);
		vertexBeam->SetWorldLocation(getPlacementLocation(selectedVertex, clickedTile->GetActorLocation()));
	}

	TArray<AActor*> Tiles;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), Tiles);
	for (int i = 0; i < Tiles.Num(); i++) {
		ATile * tile = dynamic_cast<ATile*>(Tiles[i]);
		tile->highlightTile(false);
	}


	highlightTiles();

}

void ACatanPlayerController::rotateRoad() {
	rotateRoadServer(selectionRow, selectionCol, selectedVertex);
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
