#include "AudioEngine.h"
#include "../Application/Application.h"
#include <assert.h>
#include <xaudio2.h>
#pragma comment(lib, "Xaudio2")

namespace GameDev2D
{
	AudioEngine::AudioEngine() :
		m_Engine(nullptr),
		m_MasteringVoice(nullptr),
		m_Callback()
	{
		HRESULT result = XAudio2Create(&m_Engine);
		assert(SUCCEEDED(result));

		result = m_Engine->CreateMasteringVoice(&m_MasteringVoice);
		assert(SUCCEEDED(result));

		Application::Get().WindowFocusChangedSlot.connect<&AudioEngine::OnWindowFocusChange>(this);
	}

	AudioEngine::~AudioEngine()
	{
		Application::Get().WindowFocusChangedSlot.disconnect<&AudioEngine::OnWindowFocusChange>(this);
		m_MasteringVoice->DestroyVoice();
		m_Engine->Release();
	}

	void AudioEngine::CreateAudioVoice(IXAudio2SourceVoice** aSource, WAVEFORMATEX* aWaveFormat)
	{
		HRESULT result = m_Engine->CreateSourceVoice(aSource, aWaveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &m_Callback, nullptr, nullptr);
		assert(SUCCEEDED(result));
	}

	void AudioEngine::DestroyAudioVoice(IXAudio2SourceVoice* aSource)
	{
		if (aSource != nullptr)
		{
			aSource->DestroyVoice();
			aSource = nullptr;
		}
	}

	void AudioEngine::SetVolume(float aVolume)
	{
		m_MasteringVoice->SetVolume(aVolume);
	}

	float AudioEngine::GetVolume()
	{
		float volume = 0.0f;
		m_MasteringVoice->GetVolume(&volume);
		return volume;
	}

	void AudioEngine::OnWindowFocusChange(bool isFocused)
	{
		if (isFocused)
		{
			m_Engine->StartEngine();
		}
		else
		{
			m_Engine->StopEngine();
		}
	}
}