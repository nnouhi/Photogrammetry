// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create mesh
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interactable Actor"));
	MyMesh->SetSimulatePhysics(true);
	SetRootComponent(MyMesh);
	MyMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));

	bHolding = false;
	bGravity = true;
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	//obtain a reference to the character and camera
	MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	PlayerCamera = MyCharacter->FindComponentByClass<UCameraComponent>();

	//Iterrate throught the characters components to find the dummy
	TArray<USceneComponent*> Components;
	MyCharacter->GetComponents(Components);
	if (Components.Num() > 0)
	{
		for (auto& Comp : Components)
		{
			if (Comp->GetName() == "HoldingComponent")
			{
				HoldingComp = Cast<USceneComponent>(Comp);
			}
		}
	}
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHolding && HoldingComp)
	{
		//set actors location & rotation to match dummy's location & rotation
		SetActorLocationAndRotation(HoldingComp->GetComponentLocation(),HoldingComp->GetComponentRotation());
	}
}

void APickupActor::RotateActor()
{	
	//Get thumbstick's rotation
	ControlRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	SetActorRotation(FQuat(ControlRotation));

	//Rotate actor 90 degrees to face us
	FRotator NewRotation = FRotator(0.0f, 90.0f, 90.0f);

	FQuat QuatRotation = FQuat(NewRotation);

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

void APickupActor::Pickup()
{
	bHolding = !bHolding;
	bGravity = !bGravity;

	MyMesh->SetEnableGravity(bGravity);
	MyMesh->SetSimulatePhysics(bHolding ? false : true);
	//if we are holding the actor disable collision to not interfere with player capsule
	MyMesh->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	//if we are not holding the actor throw it away
	if (!bHolding)
	{
		ForwardVector = PlayerCamera->GetForwardVector();
		MyMesh->AddForce(ForwardVector * 100000 * MyMesh->GetMass());
	}
}


