#pragma once
#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <string>

class Mediator
{
public:
	Mediator(std::string sURL);

private:
	std::string m_sURL;
};

#endif // MEDIATOR_H
