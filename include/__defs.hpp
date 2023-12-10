#pragma once
#ifndef __SRG_HELPER_DEFINITIONS_HEADER__
#define __SRG_HELPER_DEFINITIONS_HEADER__

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
using namespace godot;

#include <type_traits>
#include <vector>
#include <map>
#include <string>
#include <cstdint>

#define STR_(x) #x
#define CAT_(x, y) x##y

#define GDX_SUBCLASS(Which)  : public Which
#define GDX_CLASS_PREFIX(T, Which) \
    GDCLASS(T, Which) \
protected: \
    static void _bind_methods()

#define DECLARE_PROPERTY(T, VarName, ParamName, VarType) \
    ClassDB::bind_method(D_METHOD(STR_(CAT_(get, VarName))), &T::CAT_(get, VarName)); \
    ClassDB::bind_method(D_METHOD(STR_(CAT_(set, VarName)), #ParamName), &T::CAT_(set, VarName)); \
    ADD_PROPERTY(PropertyInfo(VarType, #VarName), STR_(CAT_(set, VarName)), STR_(CAT_(get, VarName)))

#define DECLARE_PROPERTY_READONLY(T, VarName, VarType, Getter) \
    ClassDB::bind_method(D_METHOD(#Getter), &T::Getter); \
    ADD_PROPERTY(PropertyInfo(VarType, #VarName), "", #Getter)

#define DECLARE_ENUM_PROPERTY(T, VarName, ParamName, EnumNames) \
    ClassDB::bind_method(D_METHOD(STR_(CAT_(get, VarName))), &T::CAT_(get, VarName)); \
    ClassDB::bind_method(D_METHOD(STR_(CAT_(set, VarName)), #ParamName), &T::CAT_(set, VarName)); \
    ADD_PROPERTY(PropertyInfo(Variant::INT, #VarName, PROPERTY_HINT_ENUM, EnumNames), STR_(CAT_(set, VarName)), STR_(CAT_(get, VarName)))

#define DECLARE_ENUM_PROPERTY_READONLY(T, VarName, EnumNames) \
    ClassDB::bind_method(D_METHOD(STR_(CAT_(get, VarName))), &T::CAT_(get, VarName)); \
    ADD_PROPERTY(PropertyInfo(Variant::INT, #VarName, PROPERTY_HINT_ENUM, EnumNames), "", STR_(CAT_(get, VarName)))

#define DECLARE_RESOURCE_PROPERTY(T, VarName, ParamName, ResourceClass) \
    ClassDB::bind_method(D_METHOD(STR_(CAT_(get, VarName))), &T::CAT_(get, VarName)); \
    ClassDB::bind_method(D_METHOD(STR_(CAT_(set, VarName)), #ParamName), &T::CAT_(set, VarName)); \
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #VarName, PROPERTY_HINT_RESOURCE_TYPE, #ResourceClass), STR_(CAT_(set, VarName)), STR_(CAT_(get, VarName)))

#define DECLARE_OBJECT_PROPERTY(T, VarName, ParamName, ObjectType) \
    ClassDB::bind_method(D_METHOD(STR_(CAT_(get, VarName))), &T::CAT_(get, VarName)); \
    ClassDB::bind_method(D_METHOD(STR_(CAT_(set, VarName)), #ParamName), &T::CAT_(set, VarName)); \
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #VarName, PROPERTY_HINT_OBJECT_ID, #ObjectType), STR_(CAT_(set, VarName)), STR_(CAT_(get, VarName)))

#endif /// __SRG_HELPER_DEFINITIONS_HEADER__
