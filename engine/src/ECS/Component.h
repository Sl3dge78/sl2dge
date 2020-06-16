#pragma once

namespace ECS
{
class Entity;

class Component {
    friend class Entity;
  public:
        Component() = default;
        virtual ~Component() = 0;
};
inline Component::~Component() = default;

// Used by entities to refer to components with an unique id
struct ComponentID
{
  public:
    template <class T>
    static const int Get() {
        static int id = count++;
        return id;
    }
  private:
    static int count;
};
}
