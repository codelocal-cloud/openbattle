#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OpenBattleExchangeBox.generated.h"

class AOpenBattleCharacter;
class UStaticMeshComponent;

UCLASS()
class OPENBATTLE_API AOpenBattleExchangeBox : public AActor
{
    GENERATED_BODY()

public:
    AOpenBattleExchangeBox();

    int32 RedeemFor(AOpenBattleCharacter* Character);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="OpenBattle|Exchange")
    TObjectPtr<UStaticMeshComponent> Mesh;
};
