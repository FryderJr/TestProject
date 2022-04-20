// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UBarrel;
class UTurret;
class AProjectile;
class UStaticMesh;
class USplineComponent;
class USplineMeshComponent;

UCLASS()
class TESTPROJECT_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Blueprint functions to set our barrel and turret components
	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetBarrelReference(UBarrel* BarrelToSet);

	UFUNCTION(BlueprintCallable, Category = Setup)
	void SetTurretReference(UTurret* TurretToSet);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UPROPERTY(EditAnywhere, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(VisibleAnywhere, Category = Spline)
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spline)
	UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, Category = Firing)
	float LaunchSpeed = 4000;

private:
	float ReloadTimeInSecods = 3;

	double LastFireTime = 0;

	UBarrel* Barrel;

	UTurret* Turret;

	TArray<USplineMeshComponent*> SplineMeshComponents;

	void RotateTurret(float Rotation);

	void ElevateBarrel(float Elevation);
};
