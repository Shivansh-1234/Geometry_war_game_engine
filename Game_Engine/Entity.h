#pragma once

#include "Dependencies.h"
#include "Components.h"

namespace sa {

	class Entity
	{
		size_t m_id = 0;
		std::string m_tag = "default";
		bool m_active = true;

		Entity(const size_t id, const std::string& tag);

		friend class EntityManager;

	public:

		std::shared_ptr<CTransform>  cTransform;
		std::shared_ptr<CShape>      cShape;
		std::shared_ptr<CCollision>  cCollision;
		std::shared_ptr<CScore>      cScore;
		std::shared_ptr<CLifespan>   cLifespan;
		std::shared_ptr<CInput>      cInput;

		bool  isActive() const;
		const std::string& get_tag() const;
		const size_t get_id() const;
		
		void destroy();
	};
}
