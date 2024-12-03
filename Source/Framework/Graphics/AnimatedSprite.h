#pragma once

#include "SpriteAtlas.h"
#include <entt.hpp>
#include <string>
#include <vector>


namespace GameDev2D
{
    class AnimatedSprite : public Sprite
    {
    public:
        //Creates a AnimatedSprite for the given filename, all the frames in the atlas will 
        //be stored, you must specify which frames are to be used in the animation using the 
        //AddFrame method below, by supplying the atlas key for each frame
        AnimatedSprite(const std::string& filename);
        AnimatedSprite();
        ~AnimatedSprite();

        entt::sink<void(AnimatedSprite*)> AnimationFinishedSlot;
        entt::sink<void(AnimatedSprite*)> AnimationLoopedSlot;

        void SetTexture(std::string filename) override;
            
        //Adds a frame to the back of the animation, the frame must be in
        //the spriteAtlas that was loaded on creation
        void AddFrame(const std::string& atlasKey);

		//Removes all the frames in the animation
		void RemoveAllFrames();

        //Returns the frame count
        unsigned long GetFrameCount();

        //Returns the frame index
        int GetFrameIndex();

        //Sets the frame index
        void SetFrameIndex(unsigned int frameIndex);

        //Returns the frame per second of the sprite
        double GetFramesPerSecond();

        //Set the frame per second (framespeed) of an animated sprite
        void SetFramesPerSecond(double framesPerSecond);

        //Returns wether this Sprite loops, must be animated
        bool DoesLoop();

        //Sets if the Sprite loops, must be animated
        void SetDoesLoop(bool doesLoop);

        //Starts playing the animated sprite from the current frame
		void Play(); 

        //Pause's playback of the animated sprite
        void Pause();

        //Pause's playback of the animated sprite AND resets the animated sprite back to the first frame
        void Stop();

        //Returns wether the AnimatedSprite is playing or not
        bool IsPlaying();

    private:
        void OnUpdate(float delta);

        void NextFrame();
        void UseFrame(const std::string& atlasKey);

        //Member variables
        entt::sigh<void(AnimatedSprite*)> m_AnimationFinishedSignal;
        entt::sigh<void(AnimatedSprite*)> m_AnimationLoopedSignal;
        std::string m_Filename;
        std::vector<std::string> m_Frames;
        unsigned int m_FrameIndex;
        double m_FramesPerSecond;
        double m_FrameTime;
        double m_FrameDuration;
        bool m_DoesLoop;
		bool m_IsPlaying;
    };
}

