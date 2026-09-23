#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OpenBattleGameModeBase.generated.h"

UCLASS()
class OPENBATTLE_API AOpenBattleGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AOpenBattleGameModeBase();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Match")
    int32 MaxPlayers = 40;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Match")
    int32 SecuritySlots = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Match")
    int32 PlayersPerFloor = 4;
};
