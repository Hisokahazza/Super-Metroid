#include "Animation.h"

#include <iostream>

SheetAnimation::SheetAnimation(const sf::Texture& spriteSheet, const std::string& sheetTag, const sf::Vector2u& rowsColumns, unsigned int keyFrames, bool isStatic, float switchTime, int timesPlayed, bool loop) :
    m_SpriteSheet(spriteSheet), m_SheetTag(sheetTag), m_RowsColumns(rowsColumns), m_KeyFrames(keyFrames), m_IsStatic(isStatic), m_SwitchTime(switchTime), m_TimesPlayed(timesPlayed), m_Loop(loop)
{
}

void SheetAnimation::begin()
{
    // set sheet dimensions and initial rect locally
    m_SheetDimensions = { m_SpriteSheet.getSize().x, m_SpriteSheet.getSize().y };
    m_InitialRect = { 0, 0, (int)(m_SheetDimensions.x / m_RowsColumns.x), (int)(m_SheetDimensions.y / m_RowsColumns.y) };
}

void SheetAnimation::update(float deltaTime)
{
    // handle single frame animations (still animations)
    if (m_RowsColumns == sf::Vector2u(1, 1))
    {
        m_CurrentRect = m_InitialRect;
        m_TotalTime = 0;
    }

    if (m_Loop == true)
    {
        // increment total time in accordance with frame rate (deltaTime is time between frames)
        m_TotalTime += deltaTime;

        if (m_FrameCount == 0)
        {
            m_CurrentRect = m_InitialRect;
            m_FrameCount++;
        }
        else if (m_TotalTime >= m_SwitchTime)
        {
            m_FrameCount++;
            // Subtract the switch time from total time
            m_TotalTime -= m_SwitchTime;

            if (m_AnimDirection == true)
            {
                // Move to the next frame
                m_CurrentRect.left += m_InitialRect.width;
            }
            else
            {
                // Move to the previous frame
                m_CurrentRect.left -= m_InitialRect.width;
            }
        }

        // flip animation direction for looping animations
        if (m_FrameCount == m_RowsColumns.x * m_RowsColumns.y)
        {
            m_AnimDirection = !m_AnimDirection;
            m_FrameCount = ((m_RowsColumns.x * m_RowsColumns.y) - m_KeyFrames) + 1;
        }
    }

    if (m_Loop == false)
    {

        // increment total time in accordance with frame rate (deltaTime is time between frames)
        m_TotalTime += deltaTime;

        if (m_FrameCount == 0)
        {
            m_CurrentRect = m_InitialRect;
            m_FrameCount++;
        }
        else if (m_TotalTime >= m_SwitchTime && playing == true)
        {
            // Subtract the switch time from total time
            m_TotalTime -= m_SwitchTime;

            if (m_FrameCount != m_RowsColumns.x * m_RowsColumns.y)
            {
                m_CurrentRect.left += m_InitialRect.width;
            }
            else if (m_FrameCount == m_RowsColumns.x * m_RowsColumns.y)
            {
                m_CurrentRect.left = 0;
            }

            m_FrameCount++;
        }

        // Halt animation when spritesheet limit is reached
        if (m_FrameCount == (m_TimesPlayed * m_RowsColumns.x * m_RowsColumns.y))
        {
            playing = false;
        }
    }
}

void SheetAnimation::reset()
{
    // Reset key animation attributes
    m_FrameCount = 0;
    m_TotalTime = 0;
    m_CurrentRect = m_InitialRect;
    playing = true;
}

sf::Texture SheetAnimation::getCurrentFrame()
{
    if (m_IsStatic == true)
    {
        m_CurrentFrame.loadFromFile("res/textures/" + m_SheetTag, m_InitialRect);
        return m_CurrentFrame;
    }

    m_CurrentFrame.loadFromFile("res/textures/" + m_SheetTag, m_CurrentRect);
    return m_CurrentFrame;
}

SheetlessAnimation::SheetlessAnimation(std::vector<sf::Texture> AnimationTextures, float switchTime, bool loop, bool reverse, int timesplayed, std::vector<sf::Vector2f> frameSizes) :
    m_SwitchTime(switchTime), m_AnimationTextures(AnimationTextures), m_Loop(loop),  m_Reverse(reverse), m_TimesPlayed(timesplayed), m_FrameSizes(frameSizes)
{
}

void SheetlessAnimation::begin()
{
    m_NumTextures = m_AnimationTextures.size() - 1;

    // Set initial frames and texture indicies
    if (m_Reverse == true)
    {
        m_NextTextureIndex = m_NumTextures;
        m_CurrentTexture = m_AnimationTextures[m_NumTextures];
    }
    else
    {
        if (m_Loop == true)
        {
            m_NextTextureIndex = 0;
        }
        else
        {
            m_NextTextureIndex = 0;
        }
        
        m_CurrentTexture = m_AnimationTextures[0];

    }
}

void SheetlessAnimation::update(float deltaTime)
{
    // increment texture sizes on a frame by frame basis
    if (m_FrameSizes.size() > 0)
    {
        if (m_Reverse == true)
        {
            m_CurrentTextureSize = m_FrameSizes[m_NextTextureIndex];
        }
        else if (m_Reverse == false)
        {
            m_CurrentTextureSize = m_FrameSizes[m_NextTextureIndex];
        }
    }
  
    // Halt playing when animation is over (reverse)
    if (m_FrameCount == 0)
    {
        playing = true;
    }

    // Halt playing when animation is over (not-reverse)
    if (m_FrameCount == (m_NumTextures) * m_TimesPlayed && m_TimesPlayed != -1)
    {
        playing = false;
    }

    if (m_Loop == false)
    {
        m_TotalTime += deltaTime;

        m_CurrentTexture = m_AnimationTextures[m_NextTextureIndex];
        
        if (m_TotalTime >= m_SwitchTime && playing == true)
        {
            m_TotalTime -= m_SwitchTime;

            if (m_NextTextureIndex != m_NumTextures + 1 && m_Reverse == false)
            {
                m_NextTextureIndex++;
            }
            else if (m_NextTextureIndex >= 0 && m_Reverse == true)
            {
                m_NextTextureIndex--;
            }
            
            if (m_NextTextureIndex == m_NumTextures + 1 && m_Reverse == false)
            {
                m_NextTextureIndex = 0;
            }
            else if (m_NextTextureIndex < 0 && m_Reverse == true)
            {
                m_NextTextureIndex = m_NumTextures;
            }

            m_FrameCount++;
        }
    }

    if (m_Loop == true)
    {
        m_TotalTime += deltaTime;

        if (m_TotalTime >= m_SwitchTime && playing == true)
        {
            m_TotalTime -= m_SwitchTime;

            if (m_AnimDirection == true)
            {
                m_NextTextureIndex++;
            }
            else if (m_AnimDirection == false)
            {
                m_NextTextureIndex--;
            }

            if (m_NextTextureIndex == m_NumTextures || m_NextTextureIndex <= 0)
            {
                m_AnimDirection = !m_AnimDirection;
            }

            m_FrameCount++;

            m_CurrentTexture = m_AnimationTextures[m_NextTextureIndex];
        }
    }
}

void SheetlessAnimation::reset()
{
    m_FrameCount = 0;

    if (m_Reverse == true)
    {
        m_NextTextureIndex = m_NumTextures;
        m_CurrentTexture = m_AnimationTextures[m_NumTextures];
    }
    else if (m_Reverse == false)
    {
        m_NextTextureIndex = 0;
        m_CurrentTexture = m_AnimationTextures[0];
    }

    playing = true;
}
