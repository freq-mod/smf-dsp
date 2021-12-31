//          Copyright Jean Pierre Cimalando 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "playlist.h"
#include <string>
#include <memory>
#include <mutex>
#include <condition_variable>
struct Midi_Output;

enum {
    PC_Play,
    PC_Next,
    PC_Stop,
    PC_Pause,
    PC_Rewind,
    PC_Seek_Cur,
    PC_Seek_Set,
    PC_Seek_End,
    PC_Speed,
    PC_Volume,
    PC_Set_Repeat_Mode,
    PC_Next_Repeat_Mode,
    PC_Channel_Enable,
    PC_Channel_Toggle,
    PC_Request_State,
    PC_Get_Midi_Outputs,
    PC_Set_Midi_Output,
    PC_Set_Synth,
    PC_Set_Fx_Parameter,
    PC_Shutdown,
};

struct Player_Command {
    virtual ~Player_Command() {}
    virtual int type() const noexcept = 0;
};

struct Pcmd_Play : Player_Command {
    int type() const noexcept override { return PC_Play; }
    std::unique_ptr<Play_List> play_list;
    size_t play_index = 0;
};

struct Pcmd_Next : Player_Command {
    int type() const noexcept override { return PC_Next; }
    int play_offset = 0;
};

struct Pcmd_Stop : Player_Command {
    int type() const noexcept override { return PC_Stop; }
};

struct Pcmd_Pause : Player_Command {
    int type() const noexcept override { return PC_Pause; }
    enum Mode { Mode_Toggle, Mode_Pause, Mode_Resume };
    Mode mode = Mode_Toggle;
};

struct Pcmd_Rewind : Player_Command {
    int type() const noexcept override { return PC_Rewind; }
};

struct Pcmd_Seek_Cur : Player_Command {
    int type() const noexcept override { return PC_Seek_Cur; }
    double time_offset = 0;
};

struct Pcmd_Seek_Set : Player_Command {
    int type() const noexcept override { return PC_Seek_Set; }
    double time = 0;
};

struct Pcmd_Seek_End : Player_Command {
    int type() const noexcept override { return PC_Seek_End; }
};

struct Pcmd_Speed : Player_Command {
    int type() const noexcept override { return PC_Speed; }
    int value = 0;
    bool relative = false;
};

struct Pcmd_Volume : Player_Command {
    int type() const noexcept override { return PC_Volume; }
    double value = 0;
};

struct Pcmd_Set_Repeat_Mode : Player_Command {
    int type() const noexcept override { return PC_Set_Repeat_Mode; }
    unsigned repeat_mode = 0;
};

struct Pcmd_Next_Repeat_Mode : Player_Command {
    int type() const noexcept override { return PC_Next_Repeat_Mode; }
};

struct Pcmd_Channel_Enable : Player_Command {
    int type() const noexcept override { return PC_Channel_Enable; }
    unsigned channel = 0;
    bool enable = false;
};

struct Pcmd_Channel_Toggle : Player_Command {
    int type() const noexcept override { return PC_Channel_Toggle; }
    unsigned channel = 0;
};

struct Pcmd_Request_State : Player_Command {
    int type() const noexcept override { return PC_Request_State; }
};

struct Pcmd_Get_Midi_Outputs : Player_Command {
    int type() const noexcept override { return PC_Get_Midi_Outputs; }
    std::vector<Midi_Output> *midi_outputs = nullptr;
    std::mutex *wait_mutex = nullptr;
    std::condition_variable *wait_cond = nullptr;
};

struct Pcmd_Set_Midi_Output : Player_Command {
    int type() const noexcept override { return PC_Set_Midi_Output; }
    std::string midi_output_id;
};

struct Pcmd_Set_Synth : Player_Command {
    int type() const noexcept override { return PC_Set_Synth; }
    std::string synth_plugin_id;
};

struct Pcmd_Set_Fx_Parameter : Player_Command {
    int type() const noexcept override { return PC_Set_Fx_Parameter; }
    size_t index {};
    int value {};
};

struct Pcmd_Shutdown : Player_Command {
    int type() const noexcept override { return PC_Shutdown; }
    std::mutex *wait_mutex = nullptr;
    std::condition_variable *wait_cond = nullptr;
};
