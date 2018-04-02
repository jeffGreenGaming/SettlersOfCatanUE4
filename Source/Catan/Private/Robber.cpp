// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "Robber.h"


// Sets default values
ARobber::ARobber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject < UStaticMeshComponent>(TEXT("TileMesh"));
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/robber.robber"));
	Mesh->SetStaticMesh(MeshRef.Object);
	bReplicates = true;
	bReplicateMovement = true;
}

// Called when the game starts or when spawned
void ARobber::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobber::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

