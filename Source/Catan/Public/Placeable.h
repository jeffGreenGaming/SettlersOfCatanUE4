// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Placeable.generated.h"




UCLASS()
class CATAN_API APlaceable : public AActor
{
	GENERATED_BODY()



protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;


public:	
	// Sets default values for this actor's properties
	APlaceable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	uint8 getOwnerNum();


	void setOwnerNum(uint8 num);

	UFUNCTION()
	void OnRep_ownerNum();

private:

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_ownerNum)
	uint8 ownerNum;
	
};
