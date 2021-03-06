/*
 * Character.h
 *
 *  Created on: Apr 4, 2014
 *      Author: martino
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <game/PhysicGameEntity.h>
#include <game/physics/World.h>


class Character: public PhysicGameEntity {
public:
	Character(World * world);
	virtual ~Character();
	virtual void render(Painter * p);



};

#endif /* CHARACTER_H_ */
