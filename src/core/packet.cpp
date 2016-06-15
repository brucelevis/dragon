/********************************************************************
	created:	2013/03/27
	created:	27:3:2013   21:36
	filename: 	E:\work\winne\Shooting\src\Common\src\net\packet.cpp
	file path:	E:\work\winne\Shooting\src\Common\src\net
	file base:	packet
	file ext:	cpp
	author:		piyejun
	
	purpose:	
*********************************************************************/

#include "packet.h"
#include "StringUtil.h"

namespace FIGHTING
{
	void Packet::swap( Packet& _Right )
	{
		this->std::vector<char>::swap(_Right);
		std::swap(this->m_Position, _Right.m_Position);
	}
}
