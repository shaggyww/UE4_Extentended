// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "IPropertyTypeCustomization.h"
#include "TeamsAI.generated.h"

/**
 * 
 */

#define SAVEDATAFILENAME "AITeamsSavedData"
#define TeamsAI UTeamsAI::Inst

USTRUCT(BlueprintType)
struct FTeamName
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(Category = GameplayAttribute, VisibleAnywhere, BlueprintReadOnly)
	FString name;

	FTeamName();
	FTeamName(FString teamName);
};

USTRUCT()
struct FTeamData
{
	GENERATED_USTRUCT_BODY()

	FString teamName;

	TSet<int> friendlies;
	TSet<int> hostiles;

	FTeamData();
	FTeamData(FString teamName);

	void SetTeamName(FString teamName);
	void SetFriends(TSet<int> friendsSet);
	void SetHostiles(TSet<int> hostilesSet);

	friend FArchive& operator<<(FArchive& Ar, FTeamData& TeamDataRef);
};

UCLASS()
class TEAMAI_TEST_API UTeamsAI : public UObject
{
	GENERATED_BODY()

public:

	UTeamsAI();

	static UTeamsAI* Inst;

	TSet<TSharedPtr<FString>> teamNames;
	UPROPERTY()
	TMap<int, FTeamData> teamsData;
	
	UFUNCTION(BlueprintPure, Category = "AI")
	static UTeamsAI* GetTeamsAI() { return Inst; }

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool AddTeam (FString newName);
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool RemoveTeam(int TeamID);
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool RenameTeam(int TeamID, FString newName);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void RemoveAllTeams();

	UFUNCTION(BlueprintCallable, Category = "AI")
	TArray<FString> GetTeamNames();
	UFUNCTION(BlueprintCallable, Category = "AI")
	TSet<int>& GetFriends(int TeamID);
	UFUNCTION(BlueprintCallable, Category = "AI")
	TSet<int>& GetHostiles(int TeamID);
	UFUNCTION(BlueprintCallable, Category = "AI")
	int GetTeamID(FString findName);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetFriends(int TeamID, TSet<int> friendsSet);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetHostiles(int TeamID, TSet<int> hostilesSet);

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool AddFriend(int TeamID, int friendTeamID);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddFriends(int TeamID, TSet<int> friendsSet);
	UFUNCTION(BlueprintCallable, Category = "AI")
	bool AddHostile(int TeamID, int hostileTeamID);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void AddHostiles(int TeamID, TSet<int> hostilesSet);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void RemoveFriend(int TeamID, int friendTeamID);
	UFUNCTION(BlueprintCallable, Category = "AI")
	void RemoveHostile(int TeamID, int hostileTeamID);

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsConflictBetween(FGenericTeamId thisTeamMemberID, FGenericTeamId otherTeamMemberID);
	
	ETeamAttitude::Type GetTeamAttitudeTowards(FGenericTeamId thisTeamMemberID, FGenericTeamId otherTeamMemberID);

	void SaveLoadData(FArchive& Ar);

	UFUNCTION(BlueprintCallable, Category = SaveLoad)
	bool SaveData();

	UFUNCTION(BlueprintCallable, Category = SaveLoad)
	bool LoadData();

	//void PreSave(const ITargetPlatform *targetPlatform) override;

	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

};
