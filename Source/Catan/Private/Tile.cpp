// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "Tile.h"
#include "PlaceableArea.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h "

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject < UStaticMeshComponent>(TEXT("TileMesh"));
	ConstructorHelpers::FObjectFinder< UStaticMesh> MeshRef(TEXT("/Game/Content/Meshes/NoMaterialTile.NoMaterialTile"));
	Mesh->SetStaticMesh(MeshRef.Object);
	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;
	rollValue = 0;
	tileType = EResourceType::ResourceType_None;
}

APlaceableArea * ATile::getPlaceableAreaAtVertex(EVertex vertex) {
	return vertices[uint8(vertex)];
}
// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

APlaceable * ATile::getPlaceableOnVertex(EVertex vertex) {
	return vertices[uint8(vertex)]->getTakenStructure();
}

bool ATile::hasPortOnVertex(EVertex vertex) {
	return vertices[uint8(vertex)]->hasPort();
}

EPort ATile::getPortOnVertex(EVertex vertex) {
	return vertices[uint8(vertex)]->getPort();
}

void ATile::addPlaceableOnVertex(EVertex vertex, APlaceable * newStructure) {
	vertices[uint8(vertex)]->setTaken(newStructure);
}

EVertex ATile::getClosestVertex(FVector location) {

	int minIndex = 0;
	float minDistance = (location - vertices[0]->GetActorLocation()).Size();
	for (int i = 1; i < vertices.Num(); i++) {
		float distance = (location - vertices[i]->GetActorLocation()).Size();
		if (distance <= minDistance) {
			minIndex = i;
			minDistance = distance;
		}
	}
	return EVertex(minIndex);
}

bool ATile::isVertexConnected(EVertex vertex, uint8 playerNum) {
	return vertices[uint8(vertex)]->hasRoadConnection(playerNum);
}
// Called every frame
void ATile::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
}

void ATile::OnRep_rollValue() {
		FVector Location = GetActorLocation();
		Location.Z = Location.Z + 1.0f;
		FRotator Rotation(0.0f, 90.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;


		Token = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), Location, Rotation, SpawnInfo);
		Token->GetStaticMeshComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		UStaticMeshComponent * mesh = Token->GetStaticMeshComponent();
		mesh->SetMobility(EComponentMobility::Movable);
		const FString text = "/Game/Content/Meshes/" + FString::FromInt(rollValue) + "token." + FString::FromInt(rollValue) + "token";
		UStaticMesh * newMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *text));
		mesh->SetStaticMesh(newMesh);
}

void ATile::setRollValue(uint16 newVal) {
	rollValue = newVal;
	if (Role == ROLE_Authority) {
		OnRep_rollValue();
	}
}

void ATile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATile, rollValue);
	DOREPLIFETIME(ATile, Token);
	DOREPLIFETIME(ATile, tileType);

}


uint8 ATile::getRowPos() {
	return rowPos;
}


uint8 ATile::getColPos() {
	return colPos;
}

uint8 ATile::getRollVal() {
	return rollValue;
}

void ATile::setGridPosition(uint16 row, uint16 col) {
	rowPos = row;
	colPos = col;
	
}

void ATile::highlightTile(bool toggle) {
	Mesh->SetRenderCustomDepth(toggle);
}

ATile * ATile::getEdgeTile(ETileEdge edge) {
	return edgeTiles[uint8(edge)];
}



void ATile::OnRep_tileType() {
	FString path = "";
	switch (tileType) {
		case EResourceType::ResourceType_Brick:
			path = "/Game/Content/Meshes/brick.brick";
			break;
		case EResourceType::ResourceType_Sheep:
			path = "/Game/Content/Meshes/sheep.sheep";
			break;
		case EResourceType::ResourceType_Stone:
			path = "/Game/Content/Meshes/stone.stone";
			break;
		case EResourceType::ResourceType_Wheat:
			path = "/Game/Content/Meshes/wheat.wheat";
			break;
		case EResourceType::ResourceType_Wood:
			path = "/Game/Content/Meshes/wood.wood";
			break;
		case EResourceType::ResourceType_Desert:
			path = "/Game/Content/Meshes/desert.desert";
			break;
	}
	UStaticMesh * newMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *path));
	Mesh->SetStaticMesh(newMesh);
}

void ATile::setResourceType(EResourceType resourceType) {
	tileType = resourceType;
	if (Role == ROLE_Authority) {
		OnRep_tileType();
	}
}

EResourceType ATile::getTileType() {
	return tileType;
}

uint8 ATile::getRollValue() {
	return rollValue;
}