#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"

#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"
#include <SDL.h>

#include <algorithm>

struct RenderableEntity {
	TransformComponent transformComponent;
	SpriteComponent spriteComponent;
};

class RenderSystem : public System {
public:
	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
		
		
		std::vector<Entity> entities = GetSystemEntities();

		std::vector<RenderableEntity> renderableEntities = ExtractRenderableEntities(entities);

		std::vector<RenderableEntity> sortedRenderableEntities = SortRenderableEntities(renderableEntities);

		for (RenderableEntity entity : sortedRenderableEntities) {

			
			const auto transform = entity.transformComponent;
			const auto sprite = entity.spriteComponent;

			/*SDL_Rect objRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				sprite.width,
				sprite.height
			};

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &objRect);*/

			SDL_Rect srcRect = sprite.srcRect;
			

			SDL_Rect dstRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y),
			};

			SDL_RenderCopyEx(
				renderer,
				assetStore->GetTexture(sprite.assetId),
				&srcRect,
				&dstRect,
				transform.rotation,
				NULL,
				SDL_FLIP_NONE
			);
		}
	}
private:
	std::vector<RenderableEntity> ExtractRenderableEntities(std::vector<Entity> entities) {

		std::vector<RenderableEntity> renderableEntities;

		std::transform(entities.begin(), entities.end(),
			std::back_inserter(renderableEntities),
			[](Entity entity) {
				RenderableEntity renderableEntity;
				renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
				renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
				return renderableEntity;
			});
		
		return renderableEntities;

	}

	std::vector<RenderableEntity> SortRenderableEntities(std::vector<RenderableEntity> entities) {

		std::vector<RenderableEntity> sortedEntities(entities.size());
		
		std::copy(entities.begin(), entities.end(), sortedEntities.begin());

		
		std::sort(sortedEntities.begin(), sortedEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
			return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
		});
		

		return sortedEntities;

	}
};


#endif
