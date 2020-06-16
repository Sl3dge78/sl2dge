#include "World.h"

namespace ECS
{

World::~World() {
    DeleteAllSystems();
    DeleteAllEntities();
}

// == ENTITIES ==

Entity* World::CreateEntity() {
    Entity* e = new Entity();
    entity_list_.push_back(e);
    
    is_systems_entities_list_dirty_ = true;
    
    return e;
}

void World::DeleteAllEntities() {
    for(Entity* e : entity_list_) {
        delete e;
    }
    
    is_systems_entities_list_dirty_ = true;
    
    entity_list_.clear();
}
    
void World::DeleteEntity(ECS::Entity* e) {
    for (auto it = entity_list_.begin(); it < entity_list_.end(); ++it) {
        if (*it == e) {
            for (auto child : e->GetChildren()) {
                this->DeleteEntity(child);
            }
            delete e;
            entity_list_.erase(it);
            is_systems_entities_list_dirty_ = true;
            return;
        }
    }
}

// == SYSTEMS ==

ISystem* World::AddSystem(ISystem* sys) {
    
    if (dynamic_cast<InitSystem*>(sys) != nullptr) {
        init_systems_.push_front(dynamic_cast<InitSystem*>(sys));
    }
    
    if (dynamic_cast<InputSystem*>(sys) != nullptr) {
        input_systems_.push_front(dynamic_cast<InputSystem*>(sys));
    }
    
    if (dynamic_cast<UpdateSystem*>(sys) != nullptr) {
        update_systems_.push_front(dynamic_cast<UpdateSystem*>(sys));
    }
    
    if (dynamic_cast<DrawSystem*>(sys) != nullptr) {
        draw_systems_.push_front(dynamic_cast<DrawSystem*>(sys));
        dynamic_cast<DrawSystem*>(sys)->bitmap_ = main_bitmap_;
        dynamic_cast<DrawSystem*>(sys)->font_ = main_font_;
        
        draw_systems_.sort([](const DrawSystem* sys1, const DrawSystem* sys2)
                       {
                           return sys1->pos_z > sys2->pos_z;
                       });
        
    }
    
    if (dynamic_cast<WorldSetSystem*>(sys) != nullptr) {
        dynamic_cast<WorldSetSystem*>(sys)->world_ = this;
    }
    
    systems_.push_front(sys);
    return sys;
}

void World::DeleteAllSystems() {
    for(ISystem* sys : systems_) {
        delete sys;
    }
    systems_.clear();
    init_systems_.clear();
    input_systems_.clear();
    update_systems_.clear();
    draw_systems_.clear();
}

void World::Init() {
    for(InitSystem* system: init_systems_) {
        system->Init();
    }
}

void World::Input(ALLEGRO_EVENT* const ev) {
    for(InputSystem* system: input_systems_) {
        system->Input(ev);
    }
}

void World::Update() {
    for(UpdateSystem* system: update_systems_) {
        system->Update();
    }
}

void World::Draw() {
    for(DrawSystem* system: draw_systems_) {
        system->Draw();
    }
}
    
void World::UpdateSystemsEntities() {

    if(is_systems_entities_list_dirty_) {
        for (auto& system : systems_) { // for each system
            if (system->filter_.size() > 0) {
                system->entities_.clear(); // TODO : Optimize this
                
                for (auto entity : entity_list_) {  // Compare each entity to the filter list
                    if (entity->components_.empty() || !entity)
                        continue;
                    
                    bool add = false;
                    for (auto& id : system->filter_) {
                        if(!entity->HasComponent(id)) {
                            if(system->filter_type_ == FILTER_AND){
                                add = false;
                                break;
                            }
                        } else {
                            add = true;
                            if(system->filter_type_ == FILTER_OR)
                                break;
                        }
                    }
                    if(add)
                        system->entities_.push_front(entity);
                }
                system->OnEntityListChanged();
            }
        }
        is_systems_entities_list_dirty_ = false;
    }
}
    
}



