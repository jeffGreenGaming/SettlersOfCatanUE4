// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "Placeable.h"


// Sets default values
APlaceable::APlaceable()
{
	//PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject < UStaticMeshComponent>(TEXT("PlaceableMesh"));
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;
	ownerNum = 0;

}

// Called when the game starts or when spawned
void APlaceable::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void APlaceable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void APlaceable::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlaceable, ownerNum);

}

void APlaceable::OnRep_ownerNum() {
	FString text = "";
	switch (ownerNum) {
		case 1:
			text = "/Game/Content/Materials/Blue.Blue";
			break;
		case 2:
			text = "/Game/Content/Materials/Red.Red";
			break;
		case 3:
			text = "/Game/Content/Materials/Green.Green";
			break;
		case 4:
			text = "/Game/Content/Materials/Black.Black";
			break;
	}
	UMaterial * newMat = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *text));
	Mesh->SetMaterial(0,newMat);
}

void APlaceable::setOwnerNum(uint8 num) {
	ownerNum = num;
	if (Role == ROLE_Authority) {
		OnRep_ownerNum();
	}
}

uint8 APlaceable::getOwnerNum() {
	return ownerNum;

}