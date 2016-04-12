//
//  TStamp.h
//  Student Support
//
//  Created by Xinyuan Liu

//
//  This tool simulates the usage of a Rubber Stamp.


#ifndef TSTAMP_H
#define TSTAMP_H

#include "Tool.h"

class TStamp : public Tool
{
public:
	TStamp();
	//void apply_stamp(PngFacade png);
	std::string getName() ;
};

#endif
