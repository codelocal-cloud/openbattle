#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OpenBattlePrototypeTower.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;

UCLASS()
class OPENBATTLE_API AOpenBattlePrototypeTower : public AActor
{
    GENERATED_BODY()

public:
    AOpenBattlePrototypeTower();

    virtual void OnConstruction(const FTransform& Transform) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OpenBattle|Prototype")
    int32 PrototypeFloors = 3;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="OpenBattle|Prototype")
    float FloorHeight = 450.0f;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="OpenBattle|Prototype")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="OpenBattle|Prototype")
    TObjectPtr<UInstancedStaticMeshComponent> Blocks;

private:
    void BuildTower();
};
