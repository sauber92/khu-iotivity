#ifndef MYBOX_H_
#define MYBOX_H_

#include <string>

class Mybox {
public:
	static const int OPEN;
	static const int CLOSE;
	static const std::string URI;
	static const std::string TYPE;
	static std::string getStateStr(int _status);
	int state;
	std::string name;

	Mybox();
	void setState(int _state);
	int getState();
	void setName(std::string _name);
	std::string getName();
};


#endif //MYBOX_H_
