#include "Scene.h"

namespace ENGINE
{
    DX12Scene::DX12Scene(Renderer* rend)
        : m_rend(rend), 
        m_device(rend->get_device()), 
        m_command_list(rend->get_command_list())
    {
        
    }
}
