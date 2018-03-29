// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Tile.h"
#include "Enums/Direction.h"
#include "CatanDefaultCharacter.generated.h"




UCLASS()
class CATAN_API ACatanDefaultCharacter : public ACharacter
{
	GENERATED_BODY()




public:
	// Sets default values for this character's properties
	ACatanDefaultCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;



	void PitchCamera(float AxisValue);

	void YawCamera(float AxisValue);


protected:
	UPROPERTY(EditAnywhere)
		USpringArmComponent* OurCameraSpringArm;


	UCameraComponent* OurCamera;

private:
	FVector2D CameraInput;

};
