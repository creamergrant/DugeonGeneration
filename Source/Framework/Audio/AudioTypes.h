#pragma once

#include "Audio.h"
#include <xaudio2.h>


namespace GameDev2D
{
	struct WaveData
	{
		WaveData()
		{
			ZeroMemory(&waveFormat, sizeof(waveFormat));
			ZeroMemory(&buffer, sizeof(buffer));
			data = nullptr;
		}

		~WaveData()
		{
			if (data != nullptr)
			{
				delete[] data;
				data = nullptr;
			}
		}

		WAVEFORMATEX waveFormat;
		XAUDIO2_BUFFER buffer;
		void* data;
	};

	struct AudioPlaybackCallback : public IXAudio2VoiceCallback
	{
		AudioPlaybackCallback() = default;
		~AudioPlaybackCallback() = default;

		__declspec(nothrow) void __stdcall OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
		__declspec(nothrow) void __stdcall OnVoiceProcessingPassEnd() {}
		__declspec(nothrow) void __stdcall OnStreamEnd() {}
		__declspec(nothrow) void __stdcall OnBufferStart(void* context)
		{
			if (context != nullptr)
			{
				Audio* audio = reinterpret_cast<Audio*>(context);
				//audio->DispatchEvent(AudioEvent(audio, AUDIO_PLAYBACK_STARTED));
			}
		}

		__declspec(nothrow) void __stdcall OnBufferEnd(void* context)
		{
			if (context != nullptr)
			{
				Audio* audio = reinterpret_cast<Audio*>(context);
				audio->m_State = Audio::State::Stopped;
				audio->m_PlaybackDidFinishSignal.publish(*audio);
			}
		}

		__declspec(nothrow) void __stdcall OnLoopEnd(void* context)
		{
			if (context != nullptr)
			{
				Audio* audio = reinterpret_cast<Audio*>(context);
				audio->m_PlaybackDidLoopSignal.publish(*audio);
			}
		}
		__declspec(nothrow) void __stdcall OnVoiceError(void* context, HRESULT) {}
	};
}