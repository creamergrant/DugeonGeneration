#include "AnimatedSprite.h"
#include "../Application/Application.h"


namespace GameDev2D
{
    AnimatedSprite::AnimatedSprite(const std::string& aFilename) : Sprite(aFilename),
        AnimationFinishedSlot(m_AnimationFinishedSignal),
        AnimationLoopedSlot(m_AnimationLoopedSignal),
        m_Filename(aFilename),
        m_FrameIndex(0),
        m_FramesPerSecond(15.0),
        m_FrameTime(0.0),
        m_FrameDuration(1.0 / m_FramesPerSecond),
        m_DoesLoop(false),
        m_IsPlaying(false)
    {
        Application::Get().UpdateSlot.connect<&AnimatedSprite::OnUpdate>(this);
    }

    AnimatedSprite::AnimatedSprite() : Sprite(),
        AnimationFinishedSlot(m_AnimationFinishedSignal),
        AnimationLoopedSlot(m_AnimationLoopedSignal),
        m_Filename(""),
        m_FrameIndex(0),
        m_FramesPerSecond(15.0),
        m_FrameTime(0.0),
        m_FrameDuration(1.0 / m_FramesPerSecond),
        m_DoesLoop(false),
        m_IsPlaying(false)
    {
        m_FrameDuration = 1.0 / m_FramesPerSecond;
        Application::Get().UpdateSlot.connect<&AnimatedSprite::OnUpdate>(this);
    }

    AnimatedSprite::~AnimatedSprite()
    {
        Application::Get().UpdateSlot.disconnect<&AnimatedSprite::OnUpdate>(this);
    }

    void AnimatedSprite::SetTexture(std::string filename)
    {
        m_Filename = filename;
        Sprite::SetTexture(m_Filename);
    }

    void AnimatedSprite::AddFrame(const std::string& aAtlasKey)
    {
        //Add the frame
        m_Frames.push_back(aAtlasKey);

        //If that was the first frame, set the frame index
        if (m_Frames.size() == 1)
        {
            SetFrameIndex(0);
        }
    }

	void AnimatedSprite::RemoveAllFrames()
	{
		m_Frames.clear();
		SetFrameIndex(0);
	}

    unsigned long AnimatedSprite::GetFrameCount()
    {
        return m_Frames.size();
    }

    int AnimatedSprite::GetFrameIndex()
    {
        return m_FrameIndex;
    }

    void AnimatedSprite::SetFrameIndex(unsigned int frameIndex)
    {
        //Safety check the frame index.
        if (frameIndex < GetFrameCount())
        {
            m_FrameIndex = frameIndex;
            m_FrameTime = 0.0f;
            UseFrame(m_Frames.at(m_FrameIndex));
        }
    }

    double AnimatedSprite::GetFramesPerSecond()
    {
        return m_FramesPerSecond;
    }

    void AnimatedSprite::SetFramesPerSecond(double framesPerSecond)
    {
        m_FramesPerSecond = framesPerSecond;
        m_FrameDuration = 1.0 / m_FramesPerSecond;
    }

    bool AnimatedSprite::DoesLoop()
    {
        return m_DoesLoop;
    }

    void AnimatedSprite::SetDoesLoop(bool aDoesLoop)
    {
        m_DoesLoop = aDoesLoop;
    }

    void AnimatedSprite::Play()
    {
        m_IsPlaying = true;
    }

	void AnimatedSprite::Pause()
	{
		m_IsPlaying = false;
	}

    void AnimatedSprite::Stop()
    {
        Pause();
        SetFrameIndex(0);
    }

    bool AnimatedSprite::IsPlaying()
    {
        return m_IsPlaying;
    }

    void AnimatedSprite::OnUpdate(float delta)
    {
        //Check to see if the animation is paused or not
        if (m_IsPlaying)
        {
            //Increment the elapsed time.
            m_FrameTime += delta;

            if (m_FrameTime >= m_FrameDuration)
            {
                m_FrameTime -= m_FrameDuration;
                NextFrame();
            }
        }
    }

    void AnimatedSprite::NextFrame()
    {
        unsigned int nextFrameIndex = m_FrameIndex + 1;
        if (nextFrameIndex >= GetFrameCount())
        {
            if (m_DoesLoop)
            {
                SetFrameIndex(0);
                m_AnimationLoopedSignal.publish(this);
            }
            else
            {
                m_IsPlaying = false;
                m_AnimationFinishedSignal.publish(this);
            }
        }
        else
        {
            SetFrameIndex(nextFrameIndex);
        }
    }

    void AnimatedSprite::UseFrame(const std::string& atlasKey)
    {
        //Get the atlas frame for the key
        Rect frame = Application::Get().GetResourceManager().GetAtlasCache().Get(m_Filename)->Frame(atlasKey);

        if (frame.origin == Vector2::Zero && frame.size == Vector2::Zero)
        {
            Texture* defaultTexture = Application::Get().GetResourceManager().GetTextureCache().GetDefaultResource();
            Sprite::SetTexture(defaultTexture);
        }
        else
        {
            SetFrame(frame);
        }
    }
}
