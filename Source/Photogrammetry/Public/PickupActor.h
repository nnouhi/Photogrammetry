// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "PickupActor.generated.h"


UCLASS()
class PHOTOGRAMMETRY_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//SM for the pick-up actors
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MyMesh;

	//Will act as a dummy for pickup actor location,rotation
	UPROPERTY(EditAnywhere)
	USceneComponent* HoldingComp;

	UFUNCTION()
	void RotateActor();

	UFUNCTION()
	void Pickup();

	//whether we are holding actor
	bool bHolding;
	//whether actor should have enabled gravity or not
	bool bGravity;

	//Controllers Rotation (XBox controller)
	FRotator ControlRotation;

	//References to access class's methods
	ACharacter* MyCharacter;
	UCameraComponent* PlayerCamera;
	FVector ForwardVector;

};
