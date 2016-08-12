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

#include "../../Audio/Sound.h"
#include "../../IO/File.h"
#include "../../LuaScript/LuaScriptUtils.h"

#include <kaguya.hpp>

namespace Urho3D
{

static bool SoundLoadRaw(Sound* sound, const String& fileName)
{
    SharedPtr<File> file(new File(globalContext, fileName));
    if (!file->IsOpen())
        return false;
    return sound->LoadRaw(*file);
}

static bool SoundLoadWav(Sound* sound, const String& fileName)
{
    SharedPtr<File> file(new File(globalContext, fileName));
    if (!file->IsOpen())
        return false;
    return sound->LoadWav(*file);
}

static bool SoundLoadOggVorbis(Sound* sound, const String& fileName)
{
    SharedPtr<File> file(new File(globalContext, fileName));
    if (!file->IsOpen())
        return false;
    return sound->LoadOggVorbis(*file);
}

void RegisterSound(kaguya::State& lua)
{
    using namespace kaguya;

    // [Class] Sound : Resource
    lua["Sound"].setClass(UserdataMetatable<Sound, Resource>()
        // [Constructor] Sound()
        .addStaticFunction("new", &CreateObject<Sound>)

        .addOverloadedFunctions("LoadRaw", 
            // [Method] bool LoadRaw(Deserializer& source)
            &Sound::LoadRaw,
            // [Method] bool LoadRaw(const String& fileName)
            &SoundLoadRaw)

        .addOverloadedFunctions("LoadWav", 
            // [Method] bool LoadWav(Deserializer& source)
            &Sound::LoadWav,            
            // [Method] bool LoadWav(const String& fileName)
            &SoundLoadWav)

        .addOverloadedFunctions("LoadOggVorbis", 
            // [Method] bool LoadOggVorbis(Deserializer& source)
            &Sound::LoadOggVorbis,
            // [Method] bool LoadOggVorbis(const String& fileName)
            &SoundLoadOggVorbis)

        // [Method] void SetSize(unsigned dataSize)
        .addFunction("SetSize", &Sound::SetSize)
        // [Method] void SetData(const void* data, unsigned dataSize)
        .addFunction("SetData", &Sound::SetData)
        // [Method] void SetFormat(unsigned frequency, bool sixteenBit, bool stereo)
        .addFunction("SetFormat", &Sound::SetFormat)
        // [Method] void SetLooped(bool enable)
        .addFunction("SetLooped", &Sound::SetLooped)
        // [Method] void SetLoop(unsigned repeatOffset, unsigned endOffset)
        .addFunction("SetLoop", &Sound::SetLoop)

        // [Method] float GetLength() const
        .addFunction("GetLength", &Sound::GetLength)
        // [Method] unsigned GetDataSize() const
        .addFunction("GetDataSize", &Sound::GetDataSize)
        // [Method] unsigned GetSampleSize() const
        .addFunction("GetSampleSize", &Sound::GetSampleSize)
        // [Method] float GetFrequency() const
        .addFunction("GetFrequency", &Sound::GetFrequency)
        // [Method] unsigned GetIntFrequency() const
        .addFunction("GetIntFrequency", &Sound::GetIntFrequency)
        // [Method] bool IsLooped() const
        .addFunction("IsLooped", &Sound::IsLooped)
        // [Method] bool IsSixteenBit() const
        .addFunction("IsSixteenBit", &Sound::IsSixteenBit)
        // [Method] bool IsStereo() const
        .addFunction("IsStereo", &Sound::IsStereo)
        // [Method] bool IsCompressed() const
        .addFunction("IsCompressed", &Sound::IsCompressed)
        // [Method] void FixInterpolation()
        .addFunction("FixInterpolation", &Sound::FixInterpolation)

        // [Property(Readonly)] float length
        .addProperty("length", &Sound::GetLength)
        // [Property(Readonly)] unsigned dataSize
        .addProperty("dataSize", &Sound::GetDataSize)
        // [Property(Readonly)] unsigned sampleSize
        .addProperty("sampleSize", &Sound::GetSampleSize)
        // [Property(Readonly)] float frequency
        .addProperty("frequency", &Sound::GetFrequency)
        // [Property(Readonly)] unsigned intFrequency
        .addProperty("intFrequency", &Sound::GetIntFrequency)
        // [Property] bool looped
        .addProperty("looped", &Sound::IsLooped, &Sound::SetLooped)
        // [Property(Readonly)] bool sixteenBit
        .addProperty("sixteenBit", &Sound::IsSixteenBit)
        // [Property(Readonly)] bool stereo
        .addProperty("stereo", &Sound::IsStereo)
        // [Property(Readonly)] bool compressed
        .addProperty("compressed", &Sound::IsCompressed)
        );
}
}

