//          Copyright Jean Pierre Cimalando 2019-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if defined(ADEV_RTAUDIO)
#include "adev_rtaudio.h"
#include "utility/logs.h"
#include <cmath>
#include <cstring>

bool Audio_Device_Rt::init(double desired_sample_rate, double desired_latency)
{
    (void)desired_sample_rate; // use the detected value

    shutdown();

    std::unique_ptr<RtAudio> audio(new RtAudio);
    Log::i("RtAudio interface: %s", audio->getApiDisplayName(audio->getCurrentApi()).c_str());

    unsigned audio_device = audio->getDefaultOutputDevice();
    RtAudio::DeviceInfo audio_devinfo = audio->getDeviceInfo(audio_device);

    Log::i("Audio device: %s", audio_devinfo.name.c_str());
    Log::i("Input channels: %u", audio_devinfo.inputChannels);
    Log::i("Output channels: %u", audio_devinfo.outputChannels);
    Log::i("Duplex channels: %u", audio_devinfo.duplexChannels);

    RtAudio::StreamParameters audio_param;
    audio_param.deviceId = audio_device;
    audio_param.nChannels = 2;

    RtAudio::StreamOptions audio_opt;
    audio_opt.streamName = PROGRAM_DISPLAY_NAME;

    double audio_rate = audio_devinfo.preferredSampleRate;
    unsigned audio_buffer_size = (unsigned)std::ceil(desired_latency * audio_rate);

    audio->openStream(&audio_param, nullptr, RTAUDIO_FLOAT32, audio_rate, &audio_buffer_size, &rtaudio_callback, this, &audio_opt);

    audio_ = std::move(audio);
    audio_rate_ = audio_rate;
    audio_latency_ = audio_buffer_size / audio_rate;

    return true;
}

void Audio_Device_Rt::shutdown()
{
    audio_.reset();
    active_ = false;
}

bool Audio_Device_Rt::start()
{
    if (active_)
        return true;

    audio_->startStream();

    active_ = true;
    return true;
}

double Audio_Device_Rt::latency() const
{
    return audio_latency_;
}

double Audio_Device_Rt::sample_rate() const
{
    return audio_rate_;
}

int Audio_Device_Rt::rtaudio_callback(void *output_buffer, void *, unsigned nframes, double, RtAudioStreamStatus, void *user_data)
{
    Audio_Device_Rt *self = reinterpret_cast<Audio_Device_Rt *>(user_data);
    self->process_cycle(reinterpret_cast<float *>(output_buffer), nframes);
    return 0;
}
#endif
