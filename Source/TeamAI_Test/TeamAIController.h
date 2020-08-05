// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TeamsAI.h"
#include "TeamAIController.generated.h"

/**
 * 
 */


UCLASS()
class TEAMAI_TEST_API ATeamAIController : public AAIController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetTeamName, Category = AI)
	FTeamName teamName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TSet<int> friendliesOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	TSet<int> hostilesOverride;

	ATeamAIController();

	UFUNCTION(BlueprintCallable, Category = "AI")
	FORCEINLINE FString GetTeamName() const { return teamName.name; }
	//void SetTeamName(FString newName); create new team from AI?

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTeamName(FTeamName newName);

	UFUNCTION(BlueprintCallable, Category = "AI")
	FGenericTeamId GetTeamID() const;

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	ETeamAttitude::Type IsFriendOrFoe(AActor *Other);

	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
