#include "IState.h"
#include "Engine.h"

void IState::update(float deltaTime, Engine &engine)
{
    // If window size changed, rescale UI.
    glm::ivec2 windowSize{ engine.getWindowSize() };
    Camera &camera{ engine.getCamera() };
    if (m_windowSize != windowSize)
    {
        m_windowSize = windowSize;

        resize(camera);
    }

    // Call state-specific update.
    update(deltaTime, camera);
}
