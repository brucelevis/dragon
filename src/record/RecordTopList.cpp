////////////////////////////////////////////////////////////////////////////////////////
// CopyRight(C),
// FileName:	RecordTopList.cpp
// Author:		李梦松
// Version:
// Date:		2015-11-12
// Description:	数据库服务器处理排行榜
// History:		[1] 2015-11-12 11:19 创建文件
////////////////////////////////////////////////////////////////////////////////////////

#include "RecordServer.h"
#include "StringUtil.h"
#include "RecordTopList.h"
#include "proto/serialize.pb.h"

////////////////////////////////////////////////////////////////////////////////////////
// Description:		排行榜存储模块初始化
// Arguments:		
// Autor：			李梦松
// Return：			bool 初始化是否成功
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
bool RecordTopList::init()
{
	// 填充表的信息
	FieldSet* fieldsetTopList = RecordService::metaData->getFields("t_toplist");
	if(NULL == fieldsetTopList)
	{
		Fir::logger->error("找不到t_toplist的FieldSet");
		return false;
	}
	// 填充要查询表的列名
	Record recordFindCol;
	recordFindCol.put("f_type");
	recordFindCol.put("f_data");
	// 填充查询条件
	Record recordWhere;
	recordWhere.put("f_type","f_type > 0");
	// 执行查询
	RecordSetPtr pRecordSet = RecordService::dbConnPool->exeSelect(fieldsetTopList, &recordFindCol, &recordWhere);

	// 将mysql中的数据导入redis中
	if(pRecordSet != NULL) 
	{
		for(unsigned int i = 0; i < pRecordSet->size(); i++)
		{   
			Record *pRecord = pRecordSet->get(i);
			this->loadTopList(pRecord);
		}
		Fir::logger->trace("[预加载排行榜]初始化成功，一共读取: %u 个排行榜", pRecordSet->size());
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		导入数据库中的排行榜到redis中
// Arguments:		pRecord 排行榜的数据库记录
// Autor：			李梦松
// Return：			
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
void RecordTopList::loadTopList(Record* pRecord)
{
	if (NULL == pRecord) 
	{    
		Fir::logger->error("[%s], NULL == pRecord", __PRETTY_FUNCTION__);
		return;
	}

	zMemDB* hdlMemDB = zMemDBPool::getMe().getMemDBHandle();
	if (NULL == hdlMemDB) 
	{    
		Fir::logger->error("[%s], NULL == hdlMemDB", __PRETTY_FUNCTION__);
		return;
	} 

	// 从数据库中获取数据
	SDWORD nType = pRecord->get("f_type");
	std::string strbinary = pRecord->get("f_data").getBinString();

	// 反序列化获得的二进制数据
	PbTopListRecord pbTopListRecord;
	if (!pbTopListRecord.ParseFromString(strbinary))
	{
		Fir::logger->error("[预加载排行榜]，信息记录解析失败， nType = %d", nType);
		return;
	}
	int nDataSize = pbTopListRecord.infolist_size();
	for (int i = 0; i < nDataSize; i++)
	{
		// 解析proto
		const PbTopListRecord::PbInfo& rInfo = pbTopListRecord.infolist(i);
		DWORD unKey = rInfo.unkey();
		QWORD i64Score = rInfo.i64score();
		std::string szKey = StringUtil::Format("%u", unKey);
		// 将数据加入Redis
		if (hdlMemDB->addDataToZSet(StringUtil::Format("TopList%d", nType).c_str(), szKey.c_str(), i64Score) == false)
		{
			Fir::logger->error("[%s], hdlMemDB->addDataToZSet error, nType = %d, unKey = %u, i64Score = %llu", __PRETTY_FUNCTION__, nType, unKey, i64Score);
			return;
		}
	}
	Fir::logger->info("[预加载排行榜], 初始化排行榜成功, nType=%d, nDataSize = %d", nType, nDataSize);
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		向mysql数据库中保存排行榜
// Arguments:		pRecord 排行榜的数据库记录
// Autor：			李梦松
// Return：			
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
bool RecordTopList::saveDataToDB(const CMD::RECORD::t_WriteToplist_SceneRecord* pRevData)
{
	// 入参检测
	if (NULL == pRevData)
	{
		Fir::logger->error("[%s], NULL == pRevData",__PRETTY_FUNCTION__);
		return false;
	}
	
	// 获取Redis句柄
	zMemDB* hdlMemDB = zMemDBPool::getMe().getMemDBHandle();
	if (NULL == hdlMemDB) 
	{    
		Fir::logger->error("[%s], NULL == hdlMemDB", __PRETTY_FUNCTION__);
		return false;
	}

	// 获取排行榜数据
	std::vector<ST_ZSET_DATA> vecData;
	if (hdlMemDB->getDataFromZSetByRank(StringUtil::Format("TopList%d", pRevData->nTopListType).c_str(), 0, -1, vecData) == false)
	{
		Fir::logger->error("[%s], hdlMemDB->getDataFromZSetByRank error, eType = %d", __PRETTY_FUNCTION__,  pRevData->nTopListType);
		return false;
	}

	// 空排行榜不存储
	int nDataSize = vecData.size();
	if (nDataSize == 0)
	{
		return true;
	}

	// 将排行榜数据序列化
	PbTopListRecord pbTopListRecord;
	for (auto iter : vecData)
	{
		PbTopListRecord::PbInfo* pPbInfo = pbTopListRecord.add_infolist();
		if (NULL == pPbInfo)
		{
			Fir::logger->error("[%s], NULL == pPbInfo, eType = %d", __PRETTY_FUNCTION__, pRevData->nTopListType);
			return false;
		}
		pPbInfo->set_unkey(iter.unKey);
		pPbInfo->set_i64score(iter.i64Score);
	}
	std::string szPbStr = pbTopListRecord.SerializeAsString();
	std::string strbinary;
	RecordService::dbConnPool->escapeString(szPbStr, strbinary);

	// 拼写排行榜更新SQL
	std::ostringstream oss;
	oss << "replace into t_toplist (f_type, f_data) values(" << pRevData->nTopListType << ", \'" << strbinary << "\')";
	std::string strsql = oss.str();

	// 执行更新SQL
	unsigned int unSqlRes = RecordService::dbConnPool->exeUpdate(strsql);
	if (((unsigned int)-1) == unSqlRes)
	{
		Fir::logger->error("[排行榜存档], 保存档案失败, Type=%d", pRevData->nTopListType);
		return false;
	}
	Fir::logger->trace("[排行榜存档], 保存档案成功, Type=%d", pRevData->nTopListType);
	return true;
}
