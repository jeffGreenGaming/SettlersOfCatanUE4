// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "Settlement.h"


// Sets default values
ASettlement::ASettlement()
{
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("StaticMesh'/Game/Content/Meshes/settlement.settlement'"));
	Mesh->SetStaticMesh(MeshRef.Object);
	bReplicates = true;
	bReplicateMovement = true;

}

// Called when the game starts or when spawned
void ASettlement::BeginPlay()
{
	Super::BeginPlay();
	//const FString text = "/Game/Content/Materials/Blue.Blue";
	//UMaterial * newMat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *text));
	//Mesh->SetMaterial(0,newMat);

}

// Called every frame
void ASettlement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

