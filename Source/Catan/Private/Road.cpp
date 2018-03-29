// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "Road.h"




// Sets default values
ARoad::ARoad()
{
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("StaticMesh'/Game/Content/Meshes/road.road'"));
	Mesh->SetStaticMesh(MeshRef.Object);
}

// Called when the game starts or when spawned
void ARoad::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

