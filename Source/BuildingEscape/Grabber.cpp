// Copyright Pedro Henriques 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MaxUseDistance = 1000;
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	Controller = GetWorld()->GetFirstPlayerController();
	// ...
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Object %s is missing a PhysicsHandle Component"), *GetOwner()->GetName());
	}
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Object %s is missing a Input Component"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Ge the player view point
	GetUsableInView();
	//UE_LOG(LogTemp, Warning, TEXT("ViewPoint: %s"), GetUsableInView().ToString());
	
	// Ray-cast out to reach distance

	// See what we hit

}

void UGrabber::GetUsableInView()
{
	FVector CamLoc;
	FRotator CamRot;

	if (Controller == NULL)
		return;

	/* This retrieves are camera point of view to find the start and direction we will trace. */
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	//UE_LOG(LogTemp, Warning, TEXT("CamLoc: %s :: CamRot: %s"), *CamLoc.ToString(), *CamRot.ToString());
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);
	
	// Draw a debug line
	DrawDebugLine(GetWorld(), CamLoc, TraceEnd, FColor(255, 0, 0), false, 0.0f, 0.0f, 2.0f);

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		CamLoc,
		TraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	if (Hit.Actor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Looking at: %s :: Distance: %f"), *Hit.Actor->GetName(), Hit.Distance);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Looking at: nothing recognizable"));
	}


	//FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	//TraceParams.bTraceAsyncScene = true;
	//TraceParams.bReturnPhysicalMaterial = false;
	//TraceParams.bTraceComplex = true;

	/* FHitResults is passed in with the trace function and holds the result of the trace. */
	//FHitResult Hit(ForceInit);
	//GetWorld()->LineTraceSingle(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	/* Uncomment this to visualize your line during gameplay. */
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	//return Cast<AActor>(Hit.GetActor());
	return;
}