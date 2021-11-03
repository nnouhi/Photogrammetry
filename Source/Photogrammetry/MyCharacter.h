// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyCharacter.generated.h"

UCLASS()
class PHOTOGRAMMETRY_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	/*First Person Camera*/
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*Methods for Character Movement*/
	void MoveForwardBackwards(float Value);
	void MoveLeftRight(float Value);

	/*Used for line tracing*/
	UPROPERTY(EditAnywhere, Category = "Interaction")
		float TraceDistance;

	UPROPERTY(EditAnywhere, Category = "Interaction")
		bool bInteract;

	void InteractPress();

	void TraceForward_Implementation();

private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
