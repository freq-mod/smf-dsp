//          Copyright Jean Pierre Cimalando 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "player/instrument.h"
#include <memory>
class Synth_Host;

class Midi_Synth_Instrument : public Midi_Instrument {
public:
    Midi_Synth_Instrument();
    ~Midi_Synth_Instrument();

    void flush_events() override;

    void open_midi_output(gsl::cstring_span id) override;
    void close_midi_output() override;

    bool is_synth() const override { return true; }

protected:
    void handle_send_message(const uint8_t *data, unsigned len, double ts, uint8_t flags) override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
