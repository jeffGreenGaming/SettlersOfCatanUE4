// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "DevelopmentCard.h"


// Sets default values
ADevelopmentCard::ADevelopmentCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject < UStaticMeshComponent>(TEXT("CardMesh"));
}

// Called when the game starts or when spawned
void ADevelopmentCard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADevelopmentCard::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

