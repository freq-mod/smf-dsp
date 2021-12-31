//          Copyright Jean Pierre Cimalando 2019-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "metadata_display.h"
#include "player/state.h"
#include "ui/color_palette.h"
#include "ui/fonts.h"
#include "ui/text.h"
#include "utility/SDL++.h"
#include "utility/logs.h"

Metadata_Display::Metadata_Display(const Rect &bounds)
    : bounds_(bounds), md_(new Player_Song_Metadata)
{
}

Metadata_Display::~Metadata_Display()
{
}

void Metadata_Display::update_data(const Player_Song_Metadata &md)
{
    *md_ = md;
}

void Metadata_Display::paint(SDL_Renderer *rr)
{
    const Rect bounds = bounds_;
    const Color_Palette &pal = Color_Palette::get_current();
    const Player_Song_Metadata &md = *md_;

    if (0) {
        Log::i("Format: %s", md.format);
        Log::i("Tracks: %u", md.track_count);
        Log::i("Author: %s", md.author.c_str());
        Log::i("Name: %s", md.name.c_str());
        for (size_t i = 0, n = md.text.size(); i < n; ++i)
            Log::i("Text[%lu]: %s", (unsigned long)(i), md.text[i].c_str());
    }

    Rect r = bounds.reduced(Point(0, 2));

    int rows = 4;
    int lh = r.h / rows;

    for (int i = 0; i < rows; ++i) {
        Rect row = r.take_from_top(lh);

        Text_Painter tp;
        tp.rr = rr;

        tp.font = &font_fmdsp_medium;
        tp.pos = Point(row.x, row.y + 3);
        tp.fg = pal[Colors::metadata_label];

        switch (i) {
        case 0:
            tp.draw_utf8("Format:");
            break;
        case 1:
            tp.draw_utf8("Author:");
            break;
        case 2:
            tp.draw_utf8("Name:");
            break;
        case 3:
            tp.draw_utf8("Other:");
            break;
        }

        row.chop_from_left(60);

        tp.font = &font_s12;
        tp.pos = row.origin();
        tp.fg = pal[Colors::metadata_value];

        SDLpp_ClipState clip;
        SDLpp_SaveClipState(rr, clip);
        SDL_RenderSetClipRect(rr, &row);
        switch (i) {
        case 0:
            tp.draw_utf8(md.format);
            break;
        case 1:
            if (!md.author.empty())
                tp.draw_utf8(md.author);
            else
                tp.draw_utf8(u8"\u3010" "Unknown" u8"\u3011");
            break;
        case 2:
            if (!md.name.empty())
                tp.draw_utf8(md.name);
            else
                tp.draw_utf8(u8"\u3010" "Unknown" u8"\u3011");
            break;
        case 3:
            if (md.text.size() >= 1)
                tp.draw_utf8(md.text[0]);
            break;
        }
        SDLpp_RestoreClipState(rr, clip);

        if (i == 0) {
            row.chop_from_left(120);

            tp.font = &font_fmdsp_medium;
            tp.pos = Point(row.x, row.y + 3);
            tp.fg = pal[Colors::metadata_label];

            tp.draw_utf8("Tracks:");

            row.chop_from_left(60);

            tp.font = &font_s12;
            tp.pos = row.origin();
            tp.fg = pal[Colors::metadata_value];

            tp.draw_utf8(std::to_string(md.track_count));
        }
    }
}
