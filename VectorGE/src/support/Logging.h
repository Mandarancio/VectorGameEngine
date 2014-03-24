/*
 * Logging.h
 *
 *  Created on: Mar 24, 2014
 *      Author: martino
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <ostream>

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg);
#endif /* LOGGING_H_ */
