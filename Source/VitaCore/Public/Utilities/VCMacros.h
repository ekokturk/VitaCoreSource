// VitaCore - Copyright (C) Eser Kokturk, 2020. All Rights Reserved.

#pragma once

// ======================= ENUM MACROS =======================

// Bit operations for enums that are set as Bitflags
#define ENABLE_ENUM_FLAG_OPERATIONS(Enum) \
	inline           int& operator|= (int& Lhs, Enum Rhs) { return Lhs = Lhs | 1 << static_cast<int>(Rhs); } \
	inline           int& operator&= (int& Lhs, Enum Rhs) { return Lhs = Lhs & 1 << static_cast<int>(Rhs); } \
	inline CONSTEXPR __underlying_type(Enum)  operator| (Enum  Lhs, Enum Rhs) { return 1 << static_cast<__underlying_type(Enum)>(Lhs) | 1 << static_cast<__underlying_type(Enum)>(Rhs); } \
	inline CONSTEXPR __underlying_type(Enum)  operator& (Enum  Lhs, Enum Rhs) { return 1 << static_cast<__underlying_type(Enum)>(Lhs) & 1 << static_cast<__underlying_type(Enum)>(Rhs); } \
	inline CONSTEXPR int  operator& (Enum  Lhs, int Rhs) { return 1 << static_cast<int>(Lhs)& Rhs; } \
	inline CONSTEXPR int  operator& (int  Lhs, Enum Rhs) { return Lhs & 1 << static_cast<int>(Rhs); } \
	inline CONSTEXPR int  operator| (Enum  Lhs, int Rhs) { return 1 << static_cast<int>(Lhs)| Rhs; } \
	inline CONSTEXPR int  operator| (int  Lhs, Enum Rhs) { return Lhs | 1 << static_cast<int>(Rhs); } 


// Attribute set property macros
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)