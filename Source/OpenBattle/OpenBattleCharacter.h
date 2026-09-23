#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OpenBattleTypes.h"
#include "OpenBattleCharacter.generated.h"

class UCameraComponent;

UCLASS()
class OPENBATTLE_API AOpenBattleCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AOpenBattleCharacter();

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual float TakeDamage(
        float DamageAmount,
        FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintPure, Category="OpenBattle|Health")
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintPure, Category="OpenBattle|Health")
    bool IsAlive() const { return !bDead && Health > 0.0f; }

    UFUNCTION(BlueprintPure, Category="OpenBattle|Tower")
    int32 GetCurrentFloor() const;

    UFUNCTION(BlueprintPure, Category="OpenBattle|ID")
    int32 GetCarriedIdCount() const { return CarriedIds.Num(); }

    bool TryCollectIdCard(const FOBIdCardRecord& Record);
    int32 RedeemCarriedIds();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="OpenBattle|Camera")
    TObjectPtr<UCameraComponent> FirstPersonCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Health")
    float MaxHealth = 100.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|Health")
    float Health = 100.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|Health")
    bool bDead = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Weapon")
    float DamagePerShot = 34.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Weapon")
    float FireRange = 12000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Weapon")
    float FireCooldown = 0.25f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Interaction")
    float InteractionRange = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Alliance")
    float AllianceRange = 350.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Tower")
    float FloorHeight = 450.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="OpenBattle|Tower")
    float TowerBaseZ = 0.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|ID")
    TArray<FOBIdCardRecord> CarriedIds;

    UPROPERTY(Replicated, BlueprintReadOnly, Category="OpenBattle|Alliance")
    TObjectPtr<AOpenBattleCharacter> PendingAllianceRequester;

    UFUNCTION(Server, Reliable)
    void ServerFire();

    UFUNCTION(Server, Reliable)
    void ServerInteract();

    UFUNCTION(Server, Reliable)
    void ServerRequestAlliance();

    UFUNCTION(Server, Reliable)
    void ServerAcceptAlliance();

private:
    float LastServerFireTime = -1000.0f;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void Fire();
    void Interact();
    void RequestAlliance();
    void AcceptAlliance();

    bool TraceFromView(float Range, FHitResult& OutHit) const;
    void HandleDeath(AController* Killer);
};
