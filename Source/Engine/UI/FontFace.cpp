//
// Copyright (c) 2008-2014 the Urho3D project.
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

#include "Precompiled.h"
#include "Context.h"
#include "Font.h"
#include "FontFace.h"
#include "Log.h"
#include "Texture2D.h"

#include "DebugNew.h"

namespace Urho3D
{

MutableGlyph::MutableGlyph() :
    glyphIndex_(M_MAX_UNSIGNED)
{
}

FontGlyph::FontGlyph() :
    used_(false),
    page_(M_MAX_UNSIGNED)
{
}

FontFace::FontFace(Font* font) :
    font_(font),
    hasKerning_(false)
{
}

FontFace::~FontFace()
{
    if (font_)
    {
        // When a face is unloaded, deduct the used texture data size from the parent font
        unsigned totalTextureSize = 0;
        for (unsigned i = 0; i < textures_.Size(); ++i)
            totalTextureSize += textures_[i]->GetWidth() * textures_[i]->GetHeight();
        font_->SetMemoryUse(font_->GetMemoryUse() - totalTextureSize);
    }
}

const FontGlyph* FontFace::GetGlyph(unsigned c)
{
    HashMap<unsigned, unsigned>::ConstIterator i = glyphMapping_.Find(c);
    if (i != glyphMapping_.End())
    {
        FontGlyph& glyph = glyphs_[i->second_];
        glyph.used_ = true;
        return &glyph;
    }
    else
        return 0;
}

short FontFace::GetKerning(unsigned c, unsigned d) const
{
    if (!hasKerning_)
        return 0;

    if (c == '\n' || d == '\n')
        return 0;

    unsigned leftIndex = 0;
    unsigned rightIndex = 0;
    HashMap<unsigned, unsigned>::ConstIterator leftIt = glyphMapping_.Find(c);
    if (leftIt != glyphMapping_.End())
        leftIndex = leftIt->second_;
    else
        return 0;
    HashMap<unsigned, unsigned>::ConstIterator rightIt = glyphMapping_.Find(d);
    if (rightIt != glyphMapping_.End())
        rightIndex = rightIt->second_;
    else
        return 0;

    HashMap<unsigned, unsigned>::ConstIterator kerningIt = glyphs_[leftIndex].kerning_.Find(rightIndex);
    if (kerningIt != glyphs_[leftIndex].kerning_.End())
        return kerningIt->second_;
    else
        return 0;
}

bool FontFace::IsDataLost() const
{
    for (unsigned i = 0; i < textures_.Size(); ++i)
    {
        if (textures_[i]->IsDataLost())
            return true;
    }
    return false;
}


SharedPtr<Texture2D> FontFace::CreateFaceTexture()
{
    SharedPtr<Texture2D> texture(new Texture2D(font_->GetContext()));
    texture->SetMipsToSkip(QUALITY_LOW, 0); // No quality reduction
    texture->SetNumLevels(1); // No mipmaps
    texture->SetAddressMode(COORD_U, ADDRESS_BORDER);
    texture->SetAddressMode(COORD_V, ADDRESS_BORDER),
        texture->SetBorderColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
    return texture;
}

SharedPtr<Texture2D> FontFace::LoadFaceTexture(SharedPtr<Image> image)
{
    SharedPtr<Texture2D> texture = CreateFaceTexture();
    if (!texture->Load(image, true))
    {
        LOGERROR("Could not load texture from image resource");
        return SharedPtr<Texture2D>();
    }
    return texture;
}

}
