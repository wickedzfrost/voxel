#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

class Player
{
public:
    Player();
    Player(glm::vec3 pos);

    glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
};

#endif // !PLAYER_H
