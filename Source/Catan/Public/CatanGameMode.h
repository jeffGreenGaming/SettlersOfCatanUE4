// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "Placeable.h"
#include "CatanGameState.h"
#include "CatanHUD.h"
#include "CatanPlayerController.h"
#include "CatanDefaultCharacter.h"
#include "CatanPlayerState.h"
#include "HelperFunctions.h"
#include "CatanGameMode.generated.h"

/**
 * 
 */


UCLASS()
class CATAN_API ACatanGameMode : public AGameMode
{
	GENERATED_BODY()

		
public:


	ACatanGameMode();
	void InitGame(const FString & MapName,const FString & Options,FString & ErrorMessage) override;
	void StartPlay() override;

	void PostLogin(APlayerController *NewPlayer) override;

};
