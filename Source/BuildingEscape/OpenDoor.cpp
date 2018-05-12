// Copyright Pedro Henriques 2018

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
}

void UOpenDoor::OpenDoor()
{
	GetOwner()->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	GetOwner()->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > 40.f)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PressurePlate is null!"));
		return TotalMass;
	}

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (auto& Actor : OverlappingActors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor in  Volume: %s"), *Actor->GetName());
		TArray<UPrimitiveComponent*> comps;
		Actor->GetComponents(comps);
		for (auto Iter = comps.CreateConstIterator(); Iter; ++Iter)
		{
			UPrimitiveComponent* comp = Cast<UPrimitiveComponent>(*Iter);
			if (comp)
			{
				TotalMass += comp->GetMass();
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Total Mass in Volume: %f"), TotalMass);
	return TotalMass;
}
