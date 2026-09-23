#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OpenBattleTypes.h"
#include "OpenBattleIdCard.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

UCLASS()
class OPENBATTLE_API AOpenBattleIdCard : public AActor
{
    GENERATED_BODY()

public:
    AOpenBattleIdCard();

    void InitializeCard(const FOBIdCardRecord& InRecord);

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|ID")
    FOBIdCardRecord Record;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="OpenBattle|ID")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="OpenBattle|ID")
    TObjectPtr<USphereComponent> PickupSphere;

    UFUNCTION()
    void HandlePickupOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
