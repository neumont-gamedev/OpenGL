﻿#pragma once
#include "Renderer/GUI.h"
#include "Core/Serializable.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace neu {

    struct Transform : public ISerializable, GUI {
        glm::vec3 position{ 0, 0, 0 };
        glm::vec3 rotation{ 0, 0, 0 };
        glm::vec3 scale{ 1, 1, 1 };

        Transform() = default;
        Transform(const glm::vec3& position, const glm::vec3& rotation = { 0, 0, 0 }, const glm::vec3& scale = { 1, 1, 1 }) :
            position{ position },
            rotation{ rotation },
            scale{ scale }
        {
        }

        glm::mat4 GetMatrix() const {
            glm::mat4 mx = glm::mat4(1.0f);

            mx = glm::translate(mx, position);
            mx *= glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
            mx = glm::scale(mx, scale);

            return mx;
        }

        operator glm::mat4 () const {
            return GetMatrix();
        }

        glm::vec3 Forward() const { return glm::quat{ glm::radians(rotation) } * glm::vec3{ 0, 0, 1 }; }
        glm::vec3 Up() const { return glm::quat{ glm::radians(rotation) } * glm::vec3{ 0, 1, 0 }; }
        glm::vec3 Right() const { return glm::quat{ glm::radians(rotation) } * glm::vec3{ 1, 0, 0 }; }


        void Read(const serial_data_t& value) override;
        void UpdateGui() override;
    };
}