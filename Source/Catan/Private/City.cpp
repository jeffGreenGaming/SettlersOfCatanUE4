// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "City.h"

// Sets default values
ACity::ACity()
{
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("StaticMesh'/Game/Content/Meshes/city.city'"));
	Mesh->SetStaticMesh(MeshRef.Object);
}

// Called when the game starts or when spawned
void ACity::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

