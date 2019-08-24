// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticLibrary.h"
#include "Engine/Engine.h"


FString UStaticLibrary::GetNetRoleEnumAsString(ENetRole EnumValue) {
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetNameByValue((int64)EnumValue).ToString(); // for EnumValue == VE_Dance returns "VE_Dance"
}
