#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OpenBattleTypes.h"
#include "OpenBattlePlayerState.generated.h"

UCLASS()
class OPENBATTLE_API AOpenBattlePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    AOpenBattlePlayerState();

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|Identity")
    int32 OriginFloor = 1;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|Identity")
    EOBRole Role = EOBRole::Rebel;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|Alliance")
    FOBAllianceIdentity Alliance;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|Score")
    int32 RankScore = 0;

    void SetOriginFloor(int32 NewFloor);
    void SetRole(EOBRole NewRole);
    void SetAlliance(const FOBAllianceIdentity& NewAlliance);
    void ClearAlliance();
    void AddRankScore(int32 Delta);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
