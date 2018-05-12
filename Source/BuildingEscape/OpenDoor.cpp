// Copyright Pedro Henriques 2018

#include "OpenDoor.h"


UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s PressurePlate is null!"), *GetOwner()->GetName());
		return;
	}
}

void UOpenDoor::OpenDoor()
{
	OnOpen.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	OnClose.Broadcast();
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OpenDoor();
	}
	else
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	if (PressurePlate == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("%s PressurePlate is null!"), *GetOwner()->GetName());
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
