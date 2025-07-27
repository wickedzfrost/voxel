#include "Gravity.h"
#include <iostream>

Gravity::Gravity(const float gravityStrength)
    : m_velocity{ 0.0f }
    , m_enabled{ false }
    , m_gravityStrength{ gravityStrength }
{
}

void Gravity::update(const float deltatime)
{
    if (!m_enabled)
        return;

    using namespace Constants;
    m_velocity += -gravityConstant * deltatime;

    if (std::abs(m_velocity) > terminalVelocity)
        m_velocity = (m_velocity > 0) ? terminalVelocity : -terminalVelocity;
}

void Gravity::toggle()
{
    if (m_enabled)
        m_velocity = 0.0f;

    m_enabled = !m_enabled;
}

bool Gravity::isEnabled() const
{
    return m_enabled;
}

void Gravity::reset()
{
    m_velocity = 0.0f;
}

void Gravity::log() const
{
    if (m_enabled)
    {
        std::cout << "Velocity: " << m_velocity << '\n';
    }
}