#include "Mybox.h"


const int Mybox::OPEN = 1;
const int Mybox::CLOSE = 0;
const std::string Mybox::URI = "/a/light";
const std::string Mybox::TYPE = "core.light";

std::string Mybox::getStateStr(int _status){
	switch(_status){
	case Mybox::OPEN :
		return "OPEN";
	case Mybox::CLOSE :
		return "CLOSE";
	}
	return "UNDEFINED";
}

Mybox::Mybox(){state=CLOSE;}
void Mybox::setState(int _state){state = _state;}
int Mybox::getState(){return state;}
void Mybox::setName(std::string _name){ name = _name;}
std::string Mybox::getName(){return name;}


