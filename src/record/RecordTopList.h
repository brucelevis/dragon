////////////////////////////////////////////////////////////////////////////////////////
// CopyRight(C),
// FileName:	RecordTopList.h
// Author:		李梦松
// Version:
// Date:		2015-11-12
// Description:	数据库服务器处理排行榜
// History:		[1] 2015-11-12 11:19 创建文件
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RECORD_TOPLILST_H
#define _RECORD_TOPLILST_H

#include "zSingleton.h"
#include "RecordCommand.h"

class RecordTopList : public Singleton<RecordTopList>
{
	friend class Singleton<RecordTopList>;
public:
	virtual ~RecordTopList(){}
public:
		// 排行榜存储模块初始化
		bool init();
		// 导入数据库中的排行榜到redis中
		void loadTopList(Record* pRecord);
		// 向mysql数据库中保存排行榜
		bool saveDataToDB(const CMD::RECORD::t_WriteToplist_SceneRecord* pRevData);
private:
	RecordTopList(){}
};

#endif

