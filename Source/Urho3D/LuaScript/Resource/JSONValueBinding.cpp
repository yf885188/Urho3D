//
// Copyright (c) 2008-2016 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../../Precompiled.h"

#include "../../Core/Context.h"
#include "../../Core/Variant.h"
#include "../../Resource/JSONValue.h"
#include "../../LuaScript/LuaScriptUtils.h"

#include <kaguya.hpp>

namespace Urho3D
{

static void JSONValueSetBool(JSONValue& self, bool value)
{
    self = value;
}

static void JSONValueSetInt(JSONValue& self, int value)
{
    self = value;
}

static void JSONValueSetUInt(JSONValue& self, unsigned value)
{
    self = value;
}

static void JSONValueSetFloat(JSONValue& self, float value)
{
    self = value;
}

static void JSONValueSetDouble(JSONValue& self, double value)
{
    self = value;
}

static void JSONValueSetString(JSONValue& self, const char* value)
{
    self = value;
}

static void JSONValueSetArray(JSONValue& self, const JSONArray& value)
{
    self = value;
}

static void JSONValueSetObject(JSONValue& self, const JSONObject& value)
{
    self = value;
}

void RegisterJSONValue(kaguya::State& lua)
{
    using namespace kaguya;

    // [Enum] JSONValueType
    lua["JSON_NULL"] = JSON_NULL;
    lua["JSON_BOOL"] = JSON_BOOL;
    lua["JSON_NUMBER"] = JSON_NUMBER;
    lua["JSON_STRING"] = JSON_STRING;
    lua["JSON_ARRAY"] = JSON_ARRAY;
    lua["JSON_OBJECT"] = JSON_OBJECT;

    // [Enum] JSONNumberType
    lua["JSONNT_NAN"] = JSONNT_NAN;
    lua["JSONNT_INT"] = JSONNT_INT;
    lua["JSONNT_UINT"] = JSONNT_UINT;
    lua["JSONNT_FLOAT_DOUBLE"] = JSONNT_FLOAT_DOUBLE;

    // [Class] JSONValue
    lua["JSONValue"].setClass(UserdataMetatable<JSONValue>()
        // [Constructor] JSONValue()
        .setConstructors<JSONValue(),
        // [Constructor] JSONValue(bool value)
        JSONValue(bool),
        // [Constructor] JSONValue(int value)
        JSONValue(int),
        // [Constructor] JSONValue(unsigned value)
        JSONValue(unsigned),
        // [Constructor] JSONValue(float value)
        JSONValue(float),
        // [Constructor] JSONValue(double value)
        JSONValue(double),
        // [Constructor] JSONValue(const char* value),
        JSONValue(const char*),
        // [Constructor] JSONValue(const JSONValue& value)
        JSONValue(const JSONValue&)>()

        // [Method] JSONValueType GetValueType() const
        .addFunction("GetValueType", &JSONValue::GetValueType)
        // [Method] JSONNumberType GetNumberType() const
        .addFunction("GetNumberType", &JSONValue::GetNumberType)

        // [Method] bool IsNull() const
        .addFunction("IsNull", &JSONValue::IsNull)
        // [Method] bool IsBool() const
        .addFunction("IsBool", &JSONValue::IsBool)
        // [Method] bool IsNumber() const
        .addFunction("IsNumber", &JSONValue::IsNumber)
        // [Method] bool IsString() const
        .addFunction("IsString", &JSONValue::IsString)
        // [Method] bool IsArray() const
        .addFunction("IsArray", &JSONValue::IsArray)
        // [Method] bool IsObject() const
        .addFunction("IsObject", &JSONValue::IsObject)

        // [Method] void SetBool(bool value)
        .addStaticFunction("SetBool", &JSONValueSetBool)
        // [Method] void SetInt(int value)
        .addStaticFunction("SetInt", &JSONValueSetInt)
        // [Method] void SetUInt(unsigned value)
        .addStaticFunction("SetUInt", &JSONValueSetUInt)
        // [Method] void SetFloat(float value)
        .addStaticFunction("SetFloat", &JSONValueSetFloat)
        // [Method] void SetDouble(double value)
        .addStaticFunction("SetDouble", &JSONValueSetDouble)
        // [Method] void SetString(const char* value)
        .addStaticFunction("SetString", &JSONValueSetString)
        // [Method] void SetArray(const JSONArray& value)
        .addStaticFunction("SetArray", &JSONValueSetArray)
        // [Method] void SetObject(const JSONObject& value)
        .addStaticFunction("SetObject", &JSONValueSetObject)

        // [Method] bool GetBool() const
        .addFunction("GetBool", &JSONValue::GetBool)
        // [Method] int GetInt() const
        .addFunction("GetInt", &JSONValue::GetInt)
        // [Method] unsigned GetUInt() const
        .addFunction("GetUInt", &JSONValue::GetUInt)
        // [Method] float GetFloat() const
        .addFunction("GetFloat", &JSONValue::GetFloat)
        // [Method] double GetDouble() const
        .addFunction("GetDouble", &JSONValue::GetDouble)
        // [Method] const char* GetString() const
        .addFunction("GetString", &JSONValue::GetCString)
        // [Method] const JSONArray& GetArray() const
        .addFunction("GetArray", &JSONValue::GetArray)
        // [Method] const JSONObject& GetObject() const
        .addFunction("GetObject", &JSONValue::GetObject)

        .addOverloadedFunctions("__index",
            // [Method] const JSONValue& operator[](unsigned index) const
            static_cast<const JSONValue&(JSONValue::*)(unsigned) const>(&JSONValue::operator[]),
            // [Method] const JSONValue& operator[](const String& key) const
            static_cast<const JSONValue&(JSONValue::*)(const String&) const>(&JSONValue::operator[]))

        .addOverloadedFunctions("__newindex",
            // [Method] JSONValue& operator[](unsigned index)
            static_cast<JSONValue&(JSONValue::*)(unsigned)>(&JSONValue::operator[]),
            // [Method] JSONValue& operator[](const String& key)
            static_cast<JSONValue&(JSONValue::*)(const String&)>(&JSONValue::operator[]))

        // [Method] void Push(const JSONValue& value)
        .addFunction("Push", &JSONValue::Push)
        // [Method] void Pop()
        .addFunction("Pop", &JSONValue::Pop)
        // [Method] void Insert(unsigned pos, const JSONValue& value)
        .addFunction("Insert", &JSONValue::Insert)

        .addOverloadedFunctions("Erase",
            // [Method] void Erase(unsigned pos, unsigned length)
            static_cast<void(JSONValue::*)(unsigned, unsigned)>(&JSONValue::Erase),
            // [Method] bool Erase(const String& key)
            static_cast<bool(JSONValue::*)(const String&)>(&JSONValue::Erase))

        // [Method] void Resize(unsigned newSize)
        .addFunction("Resize", &JSONValue::Resize)
        // [Method] unsigned Size() const
        .addFunction("Size", &JSONValue::Size)

        // [Method] void Set(const String& key, const JSONValue& value)
        .addFunction("Set", &JSONValue::Set)
        // [Method] const JSONValue& Get(const String& key) const
        .addFunction("Get", &JSONValue::Get)
        // [Method] bool Contains(const String& key) const
        .addFunction("Contains", &JSONValue::Contains)

        // [Method] void Clear()
        .addFunction("Clear", &JSONValue::Clear)

        // [Property(Readonly)] JSONValueType valueType
        .addProperty("valueType", &JSONValue::GetValueType)
        // [Property(Readonly)] JSONNumberType numberType
        .addProperty("numberType", &JSONValue::GetNumberType)

        // [Property(Readonly)] bool isNull
        .addProperty("isNull", &JSONValue::IsNull)
        // [Property(Readonly)] bool isBool
        .addProperty("isBool", &JSONValue::IsBool)
        // [Property(Readonly)] bool isNumber
        .addProperty("isNumber", &JSONValue::IsNumber)
        // [Property(Readonly)] bool isString
        .addProperty("isString", &JSONValue::IsString)
        // [Property(Readonly)] bool isArray
        .addProperty("isArray", &JSONValue::IsArray)
        // [Property(Readonly)] bool isObject
        .addProperty("isObject", &JSONValue::IsObject)

        // [StaticConstant] JSONValue EMPTY
        .addStaticField("EMPTY", &JSONValue::EMPTY)
    );
}
}

