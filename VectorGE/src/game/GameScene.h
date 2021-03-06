/*
 * GameScene.h
 *
 *  Created on: Mar 21, 2014
 *      Author: martino
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "../core/Entity.h"
#include "GameEntity.h"
#include "Camera.h"

#include "events/MouseListener.h"
#include "events/KeyListener.h"
#include "events/WindowListener.h"

#include "physics/World.h"

#include "gui/MainContainer.h"

#include <vector>
#include <string>


class GameScene: public Entity {
public:
	explicit GameScene(Camera * camera);
	explicit GameScene(Camera * camera, World *world);
	explicit GameScene(Camera * camera, float gx,float gy);

	virtual ~GameScene();
	void gameLoop(Uint16 dt=0);

	void addEntity(GameEntity *ge);
	void removeEntity(GameEntity *ge);

	void addGUIMainComponent(MainContainer * mc);

	World * getWorld();

	void addMouseListener(MouseListener * ml);
	void removeMouseListener(MouseListener *ml);
	void removeMouseListener(int ind);

	void addKeyListener(KeyListener *kl);
	void removeKeyListener(KeyListener *kl);
	void removeKeyListener(int ind);

	void addWindowListener(WindowListener *l);
	void removeWindowListener(WindowListener *l);
	void removeWindowListener(int ind);

	void quit();
	void clear();

	SDL_Rect getDisplayBounds();

private:
	void triggerMouseListener(SDL_Event e);
	void triggerKeyListener(SDL_Event e);
	void triggerWindowListener(SDL_Event e);

	void __removeEntities();
	void __removeEntity(GameEntity *ge);

	std::string sceneName;
	std::vector<GameEntity*> gameEntities;
	std::vector<MouseListener*> mouseListeners;
	std::vector<KeyListener*> keyListeners;
	std::vector<WindowListener*> windowListeners;

	Camera * camera;
	World * world;



	std::vector<GameEntity *> __toRemove;
	int __quit;
};

#endif /* GAMESCENE_H_ */
