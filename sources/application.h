//          Copyright Jean Pierre Cimalando 2019-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "configuration.h"
#include "utility/geometry.h"
#include "utility/SDL++.h"
#include <SDL.h>
#include <nonstd/string_view.hpp>
#include <nonstd/span.hpp>
#include <mutex>
#include <vector>
#include <memory>
class File_Browser;
class File_Entry;
class Metadata_Display;
class Level_Meter;
class Modal_Box;
class Player;
struct Player_State;
class Main_Layout;
struct Midi_Output;
#if defined(HAVE_MPRIS)
class Mpris_Server;
#endif

class Application
{
public:
    static const Point size_;

    Application();
    ~Application();

    static nonstd::span<const char *> supported_file_extensions();
    static nonstd::span<const char *> supported_uri_schemes();
    static nonstd::span<const char *> supported_mime_types();

    SDL_Window *init_window();
    SDL_Renderer *init_renderer();
    void exec();
    void raise_window();

    void set_scale_factor(SDL_Window *win, unsigned sf);
    void paint(SDL_Renderer *rr, int paint);
    void paint_scaled(SDL_Renderer *rr, int paint, unsigned scale);
    void paint_cached_background(SDL_Renderer *rr);
    bool handle_key_pressed(const SDL_KeyboardEvent &event);
    bool handle_key_released(const SDL_KeyboardEvent &event);
    bool handle_mouse_pressed(const SDL_MouseButtonEvent &event);
    bool handle_mouse_released(const SDL_MouseButtonEvent &event);
    bool handle_text_input(const SDL_TextInputEvent &event);

    void play_file(const std::string &dir, const File_Entry *entries, size_t index, size_t count);
    void play_random(const std::string &dir, const File_Entry &entry);
    void play_full_path(const std::string &path);
    void advance_playlist_by(int play_offset);
    void seek_by(double time_offset);
    void seek_to(double time);
    void stop_playback();
    void pause_playback();
    void resume_playback();
    void toggle_pause_playback();
    void set_playback_speed(int speed, bool relative);
    void set_playback_volume(double volume);
    void set_repeat_mode(unsigned repeat_mode);
    void set_next_repeat_mode();
    void set_current_path(const std::string &path);
    static bool filter_file_name(const std::string &name);
    static bool filter_file_entry(const File_Entry &ent);

    void request_update();
    void update_modals();
    void open_help_dialog();
    void open_fx_dialog();
    void choose_midi_output(bool ask, nonstd::string_view choice);
    void choose_synth(bool ask, nonstd::string_view choice);
    void get_midi_outputs(std::vector<Midi_Output> &outputs);

    void choose_theme(nonstd::string_view choice);
    void get_themes(std::vector<std::string> &themes);

    void load_theme(nonstd::string_view theme);
    void load_default_theme();
    void load_theme_configuration(const CSimpleIniA &ini);
    void get_fx_parameters(const CSimpleIniA &ini, int *values) const;

    void engage_shutdown();
    void engage_shutdown_if_esc_key();
    void advance_shutdown();
    bool should_quit() const;

private:
    std::unique_ptr<CSimpleIniA> initialize_config();

private:
    void receive_state_in_other_thread(const Player_State &ps);

private:
#if defined(HAVE_MPRIS)
    std::unique_ptr<Mpris_Server> mpris_;
    volatile bool mpris_has_new_player_state_ = false;
#endif

    SDLpp_Window_u window_;
    SDLpp_Renderer_u renderer_;

    SDL_TimerID update_timer_ = 0;

    std::unique_ptr<Main_Layout> layout_;
    std::vector<std::unique_ptr<Modal_Box>> modal_;
    SDLpp_Texture_u cached_background_;

    SDLpp_Surface_u logo_image_;
    SDLpp_Surface_u wallpaper_image_;

    bool fadeout_engaged_ = false;
    int fadeout_time_ = 0;

    uint32_t esc_key_timer_ = 0;

    unsigned scale_factor_ = 1;
    std::unique_ptr<File_Browser> file_browser_;
    std::unique_ptr<Metadata_Display> metadata_display_;
    std::unique_ptr<Level_Meter> level_meter_[10];
    std::unique_ptr<Player> player_;

    std::string last_midi_output_choice_;
    std::string last_synth_choice_;
    std::string last_theme_choice_;

    std::unique_ptr<Player_State> ps_;
    std::mutex ps_mutex_;

    enum Info_Mode {
        Info_File,
        Info_Metadata,
        Info_Mode_Count,
    };
    Info_Mode info_mode_ = Info_File;
};
