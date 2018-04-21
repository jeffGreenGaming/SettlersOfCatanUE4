// Fill out your copyright notice in the Description page of Project Settings.

#include "Catan.h"
#include "CatanDefaultCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACatanDefaultCharacter::ACatanDefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create our components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 0.f;
	OurCameraSpringArm->AttachParent = RootComponent;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachParent = OurCameraSpringArm;
	GetCharacterMovement()->GravityScale = 0;
	SetActorEnableCollision(false);

}

// Called when the game starts or when spawned
void ACatanDefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACatanDefaultCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, 0.0f);
	NewRotation.Yaw = NewRotation.Yaw + CameraInput.X;
	OurCameraSpringArm->SetWorldRotation(NewRotation);

}

// Called to bind functionality to input
void ACatanDefaultCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("CameraPitch", this, &ACatanDefaultCharacter::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &ACatanDefaultCharacter::YawCamera);

}


void ACatanDefaultCharacter::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void ACatanDefaultCharacter::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

