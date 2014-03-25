/*
 * Painter.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: martino
 */

#include "Painter.h"
#include "../support/Logging.h"
#include "primitives/SolidColorMethod.h"
#include <SDL2/SDL_surface.h>
#include <math.h>
#include <iostream>


Painter::Painter(SDL_Renderer * rend,SDL_Size size) :
renderer(rend), displaySize(size)
{
	transformation = new Transformation();
	font = new Font();
	fill=new SolidColorMethod(Color(255,255,255));
	pen=new SolidColorMethod(Color(0,0,0));
	displayCenter.x=size.width/2;
	displayCenter.y=size.height/2;
}

Painter::~Painter() {
	// TODO Auto-generated destructor stub
}

void Painter::setPen(Color c) {
	pen =  new SolidColorMethod(c);
}

void Painter::setPen(ColorMethod * cm){
	pen=cm;
}

void Painter::setFill(Color c) {
	fill = new SolidColorMethod(c);
}


void Painter::setFill(ColorMethod * cm){
	fill=cm;
}


void Painter::setFont(Font *f) {
	delete font;
	font = new Font(f->getName(),f->getSize()*transformation->getScale());
}

ColorMethod* Painter::getFill(){
	return fill;
}
ColorMethod* Painter::getPen(){
	return pen;
}

void Painter::paintRect(Sint16 x, Sint16 y, Uint16 w, Uint16 h) {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;

	paintRect(r);
}

void Painter::paintRect(SDL_Rect rect) {

	transformation->applyTransformation(rect.x, rect.y);
	transformation->applySizeTransformation(rect.w, rect.h);
	for (int x=rect.x;x<=rect.x+rect.w;x++){
		for (int y=rect.y;y<=rect.y+rect.h;y++){
			if (x==rect.x || x==rect.x+rect.w||y==rect.y || y==rect.y+rect.h)
				paintPoint(x,y,pen);
			else
				paintPoint(x,y,fill);
		}
	}
}

void Painter::paintLine(int x1, int y1, int x2, int y2) {
	paintLine(Line(x1,y1,x2,y2));
}

void Painter::paintLine(Line  l){
	Line *l1=l.transformLine(*transformation);
	float d=l1->lenght();
	if (d==0)
		return;
	float kx=(l1->p2().x-l1->p1().x)/d;
	float ky=(l1->p2().y-l1->p1().y)/d;
	int x,y;
	for (float t=0;t<=d;t+=0.5){
		x=round(kx*t)+l1->p1().x;
		y=round(ky*t)+l1->p1().y;
		paintPoint(x,y,pen);
	}
}

void Painter::paintPoint(int x, int y) {
	transformation->applyTransformation(x, y);
	Color p=pen->colorAt(x,y);
	SDL_SetRenderDrawColor(renderer,  p.red(), p.green(), p.blue(), p.alpha());
	SDL_RenderDrawPoint(renderer, x, y);
}

void Painter::paintText(std::string text, int x, int y) {
	font->scale(transformation->getScale());
	SDL_Surface * surface = font->toSurface(text, pen->colorAt(0,0).getSDLColor());
	SDL_Rect r = font->textBounds(text);
	r.x = x;
	r.y = y;
	transformation->applyTransformation(r.x, r.y);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == 0) {
		logSDLError(std::cout, "CreateTexture");
	}
	SDL_RenderCopy(renderer, texture, NULL, &r);
}

void Painter::paintImage(Image img,SDL_Rect bounds){
	paintTexture(img.getTexture(renderer),bounds);
}

void Painter::paintImage(Image img,SDL_Point pos){
	SDL_Rect r;
	r.x=pos.x;
	r.y=pos.y;
	r.w=img.getSize().width;
	r.h=img.getSize().height;
	paintImage(img,r);
}

void Painter::paintImage(Image img,int x,int y){
	SDL_Rect r;
	r.x=x;
	r.y=y;
	r.w=img.getSize().width;
	r.h=img.getSize().height;
	paintImage(img,r);
}

void Painter::paintTexture(SDL_Texture *texture,SDL_Rect bounds){
	transformation->applyTransformation(bounds.x,bounds.y);
	transformation->applySizeTransformation(bounds.w,bounds.h);
	SDL_RenderCopy(renderer,texture,NULL,&bounds);
}

void Painter::paintShape(Shape * shape){
	Shape * s=shape->transform(*transformation);
	for (int y=s->getBoundingBox().y;y<=s->getBoundingBox().y+s->getBoundingBox().h;y++){
		for (int x=s->getBoundingBox().x;x<=s->getBoundingBox().x+s->getBoundingBox().w;x++){
			if (s->contains(x,y)){
				if (!s->contains(x-1,y)||!s->contains(x,y-1)|| !s->contains(x+1,y) || !s->contains(x,y+1))
					paintPoint(x,y,pen);
				else
					paintPoint(x,y,fill);
			}
		}
	}

}


void Painter::paintPolygon(Polygon p){
	Shape * s=p.transform(*transformation);
	for (int y=s->getBoundingBox().y;y<=s->getBoundingBox().y+s->getBoundingBox().h;y++){
		for (int x=s->getBoundingBox().x;x<=s->getBoundingBox().x+s->getBoundingBox().w;x++){
			if (s->contains(x,y))
				paintPoint(x,y,fill);
		}
	}
	for (int i=0;i<p.lines().size();i++){
		paintLine(p.lines()[i]);
	}
}

void Painter::clearWindow() {
	SDL_RenderClear(renderer);

}
void Painter::renderToScreen() {
	SDL_RenderPresent(renderer);
}

void Painter::translate(int x, int y) {
	transformation->translate(x, y);
}

void Painter::scale(double s) {
	transformation->scale(s);
}

void Painter::save(){
	transformationHistory.push_back(transformation->clone());
}
void Painter::restore(){
	if (transformationHistory.size()>0){
		transformation=transformationHistory.back()->clone();
		transformationHistory.pop_back();
	}
}

void Painter::clearTransaltion(){
	transformation->reset();
}


SDL_Size Painter::getDisplaySize(){
	return displaySize;
}

SDL_Point Painter::getDisplayCenter(){
	return displayCenter;
}

void Painter::paintPoint(int x,int y, ColorMethod *cm){

	Color p=cm->colorAt(x,y);

	SDL_SetRenderDrawColor(renderer,  p.red(), p.green(), p.blue(), p.alpha());
	SDL_RenderDrawPoint(renderer, x, y);
}
