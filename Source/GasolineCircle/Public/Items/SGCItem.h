// Gasoline Circle. Skillbox's Gamebox test, All Right Reserved!!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGCItem.generated.h"


/*
	Базовый класс для различных итемов.
	Обрабатывает вращение итема, расположенного на карте, 
	Вызывает виртуальную функцию при перекрытии итема
	Определяет звук и эффект частиц при перекрытии итема. Есть функция для проигрывания, но в данном классе не вызывается, только в наследниках.
*/


UCLASS()
class GASOLINECIRCLE_API ASGCItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGCItem();
	virtual void Tick(float DeltaTime) override;


	// Виртуальная функция, вызываемая при перекрытии CollisionVolume
	UFUNCTION()
	virtual void OnCollisionVolumeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class USphereComponent* CollisionVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* Mesh;

	// Звук и эффект перекрытия, выбирается в Blueprint. При пустых не проигрываются
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item | Effects")
	class USoundCue* PickupSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Effects")
	class UParticleSystem* PickupParticles;

	// Настройки для вращения итема
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item | Rotation")
	bool bRotate = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item | Rotation", meta = (EditCondition = "bRotate"), meta = (ToolTip = "Rotation Speed"))
	float RotationRate = 90.f;

	virtual void BeginPlay() override;

	// Проигрывает звук и эффект перекрытия объекта
	// Можно вызвать в наследниках. В текущем классе не вызывается
	void PlayEffects();
};
