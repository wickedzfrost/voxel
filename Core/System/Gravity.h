#ifndef GRAVITY_H
#define GRAVITY_H

namespace Constants
{
    inline constexpr float terminalVelocity{ 80.0f };
    inline constexpr float gravityConstant{ 9.8f };
}

class Gravity
{
public:
    Gravity(const float gravityStrenght = Constants::gravityConstant);

    void update(const float deltatime);
    void toggle();
    bool isEnabled() const;
    void reset();
    void log() const;

private:
    bool m_enabled{ false };
    float m_velocity{ 0.0f };
    float m_gravityStrength{ Constants::gravityConstant };
};

#endif // !GRAVITY_H
