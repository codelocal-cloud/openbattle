#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OpenBattleTypes.h"
#include "OpenBattleGameState.generated.h"

USTRUCT(BlueprintType)
struct FOBFloorRuntimeState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 Floor = 1;

    UPROPERTY(BlueprintReadOnly)
    EOBFloorState State = EOBFloorState::Safe;
};

UCLASS()
class OPENBATTLE_API AOpenBattleGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AOpenBattleGameState();

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|Tower")
    TArray<FOBFloorRuntimeState> FloorStates;

    void SetFloorState(int32 Floor, EOBFloorState NewState);
    EOBFloorState GetFloorState(int32 Floor) const;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
