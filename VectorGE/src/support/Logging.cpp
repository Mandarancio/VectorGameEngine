/*
 * Logging.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: martino
 */

#include "Logging.h"

#include <SDL2/SDL_error.h>

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}
