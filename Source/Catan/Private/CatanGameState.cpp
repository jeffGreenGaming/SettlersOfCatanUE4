// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "PlaceableArea.h"
#include "CatanPlayerState.h"
#include "CatanGameState.h"



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


ATile * ACatanGameState::getTileFromCoordinates(uint32 row, uint32 col) {

	for (int i = 0; i < tiles.Num(); i++) {
		ATile * tile = tiles[i];
		if (tile->getColPos() == col && tile->getRowPos() == row) {
			return tile;
		}
	}
	return nullptr;
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
	DOREPLIFETIME(ACatanGameState, Robber);
	
}


void ACatanGameState::setPlayerTurn(uint8 playerNum) {
	currentPlayerTurn = playerNum;
}

uint8 ACatanGameState::getPlayerTurn() {
	return currentPlayerTurn;
}

void ACatanGameState::setDevelopmentCards(TArray<EDevCardType> cards) {
	developmentCards = cards;
}


void ACatanGameState::setRobber(ARobber* newRobber) {
	Robber = newRobber;
}

EDevCardType ACatanGameState::getNextDevCard() {
	EDevCardType returnType;
	if (developmentCards.Num() > 0) {
		returnType = developmentCards[0];
		developmentCards.RemoveAt(0);
	}
	return returnType;
}

void ACatanGameState::moveRobberLocation(ATile * tile) {
	Robber->SetActorLocation(tile->GetActorLocation() + FVector(0.0f, 0.0f, 0.5f));
	Robber->setCurrentTile(tile);
}