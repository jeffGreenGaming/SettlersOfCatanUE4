// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Tile.h"
#include "Robber.generated.h"

UCLASS()
class CATAN_API ARobber : public AActor
{
	GENERATED_BODY()
	

protected:
	UPROPERTY()
	UStaticMeshComponent* Mesh;

public:	
	// Sets default values for this actor's properties
	ARobber();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void setCurrentTile(ATile * tile);

	ATile * getCurrentTile();

private:
	ATile * currentTile;
	
	
};
