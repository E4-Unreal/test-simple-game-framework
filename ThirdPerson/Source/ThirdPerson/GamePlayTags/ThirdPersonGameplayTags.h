#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

/**
 * ThirdPersonPlayTags
 *
 *	Singleton containing native gameplay tags.
 */
struct FThirdPersonGameplayTags
{
public:
	static const FThirdPersonGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	// Equipment Slots
	FGameplayTag EquipmentSlot_Weapon_Main_Primary;
	FGameplayTag EquipmentSlot_Weapon_Main_Secondary;
	FGameplayTag EquipmentSlot_Weapon_SideArm;
	FGameplayTag EquipmentSlot_Armor_Head;
	FGameplayTag EquipmentSlot_Armor_Chest;
	FGameplayTag EquipmentSlot_Armor_Leg;
	FGameplayTag EquipmentSlot_Armor_Arm;

	// Item Tags
	FGameplayTag Item;
	FGameplayTag Item_Clutter;
	FGameplayTag Item_Consumable;

	// Item.Equipment Tags
	FGameplayTag Item_Equipment;
	FGameplayTag Item_Equipment_Weapon_Melee_Knife;
	FGameplayTag Item_Equipment_Weapon_Ranged_Gun_Rifle;
	FGameplayTag Item_Equipment_Weapon_Ranged_Gun_ShotGun;
	FGameplayTag Item_Equipment_Weapon_Ranged_Gun_Pistol;
	FGameplayTag Item_Equipment_Armor_Head;
	FGameplayTag Item_Equipment_Armor_Chest;
	FGameplayTag Item_Equipment_Armor_Leg;
	FGameplayTag Item_Equipment_Armor_Arm;

protected:

	//Registers all of the tags with the GameplayTags Manager
	void AddAllTags(UGameplayTagsManager& Manager);

	//Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:

	static FThirdPersonGameplayTags GameplayTags;
};