#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OpenBattleGameModeBase.generated.h"

class APlayerController;
class APlayerStart;

UCLASS()
class OPENBATTLE_API AOpenBattleGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AOpenBattleGameModeBase();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Match")
    int32 MaxPlayers = 40;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Match")
    int32 SecuritySlots = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Match")
    int32 PlayersPerFloor = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Prototype")
    int32 PrototypeMaxPlayers = 12;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Prototype")
    int32 PrototypeFloors = 3;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Prototype")
    float FloorPressureTickSeconds = 1.0f;

protected:
    void SpawnPrototypeWorld();
    void UpdateFloorPressure();

private:
    float MatchStartSeconds = 0.0f;
    FTimerHandle FloorPressureTimer;
    TArray<APlayerStart*> PrototypeStarts;
};
