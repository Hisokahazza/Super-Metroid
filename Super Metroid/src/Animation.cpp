#include "Animation.h"

#include <iostream>

Animation::Animation(const sf::Texture& spriteSheet, const std::string& sheetTag, const sf::Vector2u& rowsColumns, unsigned int keyFrames, bool isStatic, float switchTime, int timesPlayed, bool loop) :
    m_SpriteSheet(spriteSheet), m_SheetTag(sheetTag), m_RowsColumns(rowsColumns), m_KeyFrames(keyFrames), m_IsStatic(isStatic), m_SwitchTime(switchTime), m_TimesPlayed(timesPlayed), m_Loop(loop)
{
}

void Animation::begin()
{
    m_SheetDimensions = { m_SpriteSheet.getSize().x, m_SpriteSheet.getSize().y };
    m_InitialRect = { 0, 0, (int)(m_SheetDimensions.x / m_RowsColumns.x), (int)(m_SheetDimensions.y / m_RowsColumns.y) };
}

void Animation::update(float deltaTime)
{
    if (m_RowsColumns == sf::Vector2u(1, 1))
    {
        m_CurrentRect = m_InitialRect;
        m_TotalTime = 0;
    }

    if (m_Loop == true)
    {
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

        if (m_FrameCount == m_RowsColumns.x * m_RowsColumns.y)
        {
            m_AnimDirection = !m_AnimDirection;
            m_FrameCount = ((m_RowsColumns.x * m_RowsColumns.y) - m_KeyFrames) + 1;
        }
    }

    if (m_Loop == false)
    {
        m_TotalTime += deltaTime;

        if (m_FrameCount == 0)
        {
            m_CurrentRect = m_InitialRect;
            m_FrameCount++;
        }
        else if (m_TotalTime >= m_SwitchTime && m_Playing == true)
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

        if (m_FrameCount == (m_TimesPlayed * m_RowsColumns.x * m_RowsColumns.y))
        {
            m_Playing = false;
        }
    }
}

void Animation::reset()
{
    m_FrameCount = 0;
    m_TotalTime = 0;
    m_CurrentRect = m_InitialRect;
    m_Playing = true;
}

sf::Texture Animation::getCurrentFrame()
{
    if (m_IsStatic == true)
    {
        m_CurrentFrame.loadFromFile("res/textures/" + m_SheetTag, m_InitialRect);
        return m_CurrentFrame;
    }

    m_CurrentFrame.loadFromFile("res/textures/" + m_SheetTag, m_CurrentRect);
    return m_CurrentFrame;
}

SheetlessAnimation::SheetlessAnimation(std::vector<sf::Texture> animationTextures, float switchTime, bool loop, int timesplayed, bool reverse) : m_SwitchTime(switchTime), m_AnimationTextures(animationTextures), m_Loop(loop), m_TimesPlayed(timesplayed), m_Reverse(reverse)
{
}

void SheetlessAnimation::begin()
{
    m_NumTextures = m_AnimationTextures.size() - 1;

    if (m_Reverse == true)
    {
        m_NextTextureIndex = m_NumTextures;
        m_CurrentTexture = m_AnimationTextures[m_NumTextures];
    }
    else
    {
        m_NextTextureIndex = -1;
        m_CurrentTexture = m_AnimationTextures[0];

    }
}

void SheetlessAnimation::update(float deltaTime)
{
    if (m_FrameCount == 0)
    {
        m_Playing = true;
    }

    if (m_Loop == false)
    {
        if (m_FrameCount == m_NumTextures * m_TimesPlayed)
        {
            m_Playing = false;
        }

        m_TotalTime += deltaTime;

        if (m_TotalTime >= m_SwitchTime && m_Playing == true)
        {
            m_TotalTime -= m_SwitchTime;

            if (m_NextTextureIndex != m_NumTextures && m_Reverse == false)
            {
                m_NextTextureIndex++;
            }
            else if (m_NextTextureIndex != 0 && m_Reverse == true)
            {
                m_NextTextureIndex--;
            }

            m_CurrentTexture = m_AnimationTextures[m_NextTextureIndex];
            
            if (m_NextTextureIndex == m_NumTextures && m_Reverse == false)
            {
                m_NextTextureIndex = -1;
            }
            else if (m_NextTextureIndex == 0 && m_Reverse == true)
            {
                m_NextTextureIndex = m_NumTextures;
            }

            m_FrameCount++;
        }
    }

    if (m_Loop == true)
    {
        m_TotalTime += deltaTime;

        if (m_TotalTime >= m_SwitchTime)
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

            if (m_NextTextureIndex == m_NumTextures - 1 || m_NextTextureIndex <= 0)
            {
                m_AnimDirection = !m_AnimDirection;
            }

            m_CurrentTexture = m_AnimationTextures[m_NextTextureIndex];
            std::cout << m_NextTextureIndex << std::endl;
        }
    }
}

sf::Texture SheetlessAnimation::getCurrentFrame()
{
    return m_CurrentTexture;
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

    m_Playing = true;
}
