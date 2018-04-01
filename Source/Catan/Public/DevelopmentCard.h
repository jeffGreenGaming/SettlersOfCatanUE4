// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DevelopmentCard.generated.h"

UCLASS()
class CATAN_API ADevelopmentCard : public AActor
{
	GENERATED_BODY()
	

protected:
	UPROPERTY()
	UStaticMeshComponent* Mesh;

public:	
	// Sets default values for this actor's properties
	ADevelopmentCard();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
