// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Barrel.h"
#include "Turret.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SplineMeshComponents.Empty();
	SplineComponent->ClearSplinePoints();

	FPredictProjectilePathParams PathParams;
	FPredictProjectilePathResult PathResult;

	//PathParams.TraceChannel = ECollisionChannel::ECC_WorldStatic;
	//PathParams.bTraceWithChannel = true;
	PathParams.ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	PathParams.StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	PathParams.LaunchVelocity = Barrel->GetSocketRotation(FName("Projectile")).Vector() * LaunchSpeed;
	PathParams.ProjectileRadius = 20.0f;
	PathParams.SimFrequency = 15.0f;
	PathParams.MaxSimTime = 2.0f;
	PathParams.bTraceWithCollision = true;
	PathParams.DrawDebugType = EDrawDebugTrace::ForDuration;
	PathParams.DrawDebugTime = 0.1;

	UGameplayStatics::PredictProjectilePath(this, PathParams, PathResult);
	UE_LOG(LogTemp, Warning, TEXT("Predict projectile path length is %d"), PathResult.PathData.Num());
	size_t PathLength = PathResult.PathData.Num();
	for (size_t i = 0; i < PathLength; i++)
	{
		SplineComponent->AddSplinePoint(PathResult.PathData[i].Location, ESplineCoordinateSpace::World);

	}
	SplineComponent->SetSplinePointType(PathLength - 1, ESplinePointType::CurveClamped);
	size_t SplineLength = SplineComponent->GetNumberOfSplinePoints();
	for (size_t i = 0; i < SplineLength - 2; i++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepWorldTransform);
		SplineMeshComponent->SetStaticMesh(SplineMesh);
		SplineMeshComponent->SetStartAndEnd(PathResult.PathData[i].Location, SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World), PathResult.PathData[i + 1].Location, SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World));
		//SplineMeshComponents.Add(SplineMeshComponent);
	}
	
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Azimuth", this, &ATank::RotateTurret);
	PlayerInputComponent->BindAxis("Altitude", this, &ATank::ElevateBarrel);
}

void ATank::SetBarrelReference(UBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void ATank::SetTurretReference(UTurret* TurretToSet)
{
	Turret = TurretToSet;
}

void ATank::Fire()
{
	bool IsReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSecods;
	if (IsReloaded)
	{
		//TankAimingComponent->FireProjectile(ProjectileBlueprint);
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

void ATank::RotateTurret(float Rotation)
{
	Turret->Rotate(Rotation);
}

void ATank::ElevateBarrel(float Elevation)
{
	Barrel->Elevate(Elevation);
}

