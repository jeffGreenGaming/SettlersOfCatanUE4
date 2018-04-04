// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DevelopmentCard.h"
#include "KnightCard.generated.h"

/**
 * 
 */
UCLASS()
class CATAN_API AKnightCard : public ADevelopmentCard
{
	GENERATED_BODY()

public:
	AKnightCard();
	
	UFUNCTION()
	virtual void use(UPrimitiveComponent* pComponent) override;
	
};
