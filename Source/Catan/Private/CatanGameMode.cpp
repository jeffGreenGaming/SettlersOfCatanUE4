// Fill out your copyright notice in the Description page of Project Settings.
#include "Catan.h"
#include "CatanGameMode.h"



ACatanGameMode::ACatanGameMode() {
	GameStateClass = ACatanGameState::StaticClass();
	HUDClass = ACatanHUD::StaticClass();
	PlayerControllerClass = ACatanPlayerController::StaticClass();
	DefaultPawnClass = ACatanDefaultCharacter::StaticClass();
	PlayerStateClass = ACatanPlayerState::StaticClass();
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


