#include "Entity.h"

namespace sa {

    Entity::Entity(const size_t id, const std::string& tag)
        :m_id(id), m_tag(tag) 
    { }

    bool Entity::isActive() const
    {
        return m_active;
    }

    const std::string& Entity::get_tag() const
    {
        return m_tag;
    }

    const size_t Entity::get_id() const
    {
        return m_id;
    }

    void Entity::destroy()
    {
        m_active = false;
    }
}
