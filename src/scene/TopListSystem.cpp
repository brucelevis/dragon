////////////////////////////////////////////////////////////////////////////////////////
// CopyRight(C),
// FileName:	TopListSystem.cpp
// Author:		李梦松
// Version:
// Date:		2015-11-10
// Description:	通用排行榜注册模块(包含排行榜更新、获取数据等接口)
// History:		[1] 2015-11-10 19:41 创建文件
//				[2] 2015-11-13 20:20 修正了获取的玩家排行有几率错误的BUG
//				[3] 2015-11-23 21:00 增加了获取排行榜指定KEY的数据的接口
//									 将排行榜注册接口重定义为公有, 交由个调用模块使用
////////////////////////////////////////////////////////////////////////////////////////
#include "Fir.h"
#include "zType.h"
#include "zMemDB.h"
#include "zMemDBPool.h"
#include "StringUtil.h"
#include "SceneUserManager.h"
#include "RecordClient.h"
#include "TopListSystem.h"

#include "proto/serialize.pb.h"

const int UPDATE_NUM = 3;	// 每分钟更新排行榜的条数

////////////////////////////////////////////////////////////////////////////////////////
// Description:		构造函数
// Arguments:		 
// Autor：			李梦松
// Return：			
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
TopListSystem::TopListSystem()
{

}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		析构函数
// Arguments:		 
// Autor：			李梦松
// Return：			
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
TopListSystem::~TopListSystem()
{

}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		初始化排行榜模块
// Arguments:		 
// Autor：			李梦松
// Return：			bool 返回初始化结果
// Comment:			初始化失败则服务器启动失败
////////////////////////////////////////////////////////////////////////////////////////
bool TopListSystem::init()
{	
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		销毁排行榜模块
// Arguments:		 
// Autor：			李梦松
// Return：			
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
void TopListSystem::Release()
{
	m_mapTopListCfgData.clear();
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		重新加载排行榜
// Arguments:		 
// Autor：			李梦松
// Return：			bool 返回加载结果
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
bool TopListSystem::ReLoad()
{
	this->Release();
	return (this->init());
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		注册排行榜配置(1条排序规则)
// Arguments:		eType 排行榜Type
//					pcTableName 排行榜对应的表名
//					funcGetData 获取排行榜具体数据的回调
//					unAmount 排行榜总条数
//					bAsc 第1条排序规则是否升序
//					funcCanUpdate 判断数据能否更新的回调
// Autor：			李梦松
// Return：			bool 返回注册结果
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
bool TopListSystem::registerTopList1Value(TOPLIST_TYPE eType, FUNC_TOPLIST_GETDATA funcGetData, DWORD unAmount, bool bAsc, FUNC_TOPLIST_CANUPDATE funcCanUpdate)
{
	if (nullptr == funcGetData)
	{
		Fir::logger->error("[%s], nullptr == funcGetData", __PRETTY_FUNCTION__);
		return false;
	}
	if (unAmount <= 0)
	{
		Fir::logger->error("[%s], unAmount<= 0, unAmount = %d", __PRETTY_FUNCTION__, unAmount);
		return false;
	}
	TopListCfgData clCfgData;
	clCfgData.szTableName = StringUtil::Format("TopList%d", eType);
	clCfgData.funcGetData = funcGetData;
	clCfgData.funcCanUpdate = funcCanUpdate;
	clCfgData.unListAmount = unAmount;
	clCfgData.unValueLen1 = MAX_ZSET_SCORE_LEN;
	clCfgData.bAsc1 = bAsc;
	m_mapTopListCfgData[eType] = clCfgData;
	m_mapTopListUpdate[eType] = false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		注册排行榜配置(2条排序规则)
// Arguments:		eType 排行榜Type
//					pcTableName 排行榜对应的表名
//					funcGetData 获取排行榜具体数据的回调
//					unAmount 排行榜总条数
//					bAsc1 第1条排序规则是否升序
//					bAsc2 第2条排序规则是否升序
//					unValueLen1 第1条排序数据最大十进制长度
//					unValueLen2 第2条排序数据最大十进制长度
//					funcCanUpdate 判断数据能否更新的回调
// Autor：			李梦松
// Return：			bool 返回注册结果
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
bool TopListSystem::registerTopList2Value(TOPLIST_TYPE eType, FUNC_TOPLIST_GETDATA funcGetData, 
										  DWORD unAmount, bool bAsc1, bool bAsc2, DWORD unValueLen1, DWORD unValueLen2, FUNC_TOPLIST_CANUPDATE funcCanUpdate)
{
	if (nullptr == funcGetData)
	{
		Fir::logger->error("[%s], nullptr == funcGetData", __PRETTY_FUNCTION__);
		return false;
	}
	if (unAmount <= 0)
	{
		Fir::logger->error("[%s], unAmount<= 0, unAmount = %d", __PRETTY_FUNCTION__, unAmount);
		return false;
	}
	if (0 == unValueLen1 || 0 == unValueLen2)
	{
		Fir::logger->error("[%s], 0 == unValueLen1 || 0 == unValueLen2, unValueLen1 = %d, unValueLen2 = %d", __PRETTY_FUNCTION__, unValueLen1, unValueLen2);
		return false;
	}
	if ((unValueLen1 + unValueLen2) > MAX_ZSET_SCORE_LEN)
	{
		Fir::logger->error("[%s], (unValueLen1 + unValueLen2) > MAX_ZSET_SCORE_LEN, unValueLen1 = %d, unValueLen2 = %d", __PRETTY_FUNCTION__, unValueLen1, unValueLen2);
		return false;
	}
	unValueLen2 = MAX_ZSET_SCORE_LEN - unValueLen1;

	TopListCfgData clCfgData;
	clCfgData.szTableName = StringUtil::Format("TopList%d", eType);
	clCfgData.funcGetData = funcGetData;
	clCfgData.funcCanUpdate = funcCanUpdate;
	clCfgData.unListAmount = unAmount;
	clCfgData.unValueLen1 = unValueLen1;
	clCfgData.bAsc1 = bAsc1;
	clCfgData.unValueLen2 = unValueLen2;
	clCfgData.bAsc2 = bAsc2;
	m_mapTopListCfgData[eType] = clCfgData;
	m_mapTopListUpdate[eType] = false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		注册排行榜配置(3条排序规则)
// Arguments:		eType 排行榜Type
//					pcTableName 排行榜对应的表名
//					funcGetData 获取排行榜具体数据的回调
//					unAmount 排行榜总条数
//					bAsc1 第1条排序规则是否升序
//					bAsc2 第2条排序规则是否升序
//					bAsc3 第3条排序规则是否升序
//					unValueLen1 第1条排序数据最大十进制长度
//					unValueLen2 第2条排序数据最大十进制长度
//					unValueLen3 第3条排序数据最大十进制长度
//					funcCanUpdate 判断数据能否更新的回调
// Autor：			李梦松
// Return：			bool 返回注册结果
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
bool TopListSystem::registerTopList3Value(TOPLIST_TYPE eType, FUNC_TOPLIST_GETDATA funcGetData, 
										  DWORD unAmount, bool bAsc1, bool bAsc2, bool bAsc3, DWORD unValueLen1, DWORD unValueLen2, DWORD unValueLen3, FUNC_TOPLIST_CANUPDATE funcCanUpdate)
{
	if (nullptr == funcGetData)
	{
		Fir::logger->error("[%s], nullptr == funcGetData", __PRETTY_FUNCTION__);
		return false;
	}
	if (unAmount <= 0)
	{
		Fir::logger->error("[%s], unAmount<= 0, unAmount = %d", __PRETTY_FUNCTION__, unAmount);
		return false;
	}
	if (0 == unValueLen1 || 0 == unValueLen2 || 0 == unValueLen3)
	{
		Fir::logger->error("[%s], 0 == unValueLen1 || 0 == unValueLen2, unValueLen1 = %d, unValueLen2 = %d, unValueLen3 = %d", __PRETTY_FUNCTION__, unValueLen1, unValueLen2, unValueLen3);
		return false;
	}
	if ((unValueLen1 + unValueLen2 + unValueLen3) > MAX_ZSET_SCORE_LEN)
	{
		Fir::logger->error("[%s], (unValueLen1 + unValueLen2 + unValueLen3) > MAX_ZSET_SCORE_LEN, unValueLen1 = %d, unValueLen2 = %d, unValueLen3 = %d", 
							__PRETTY_FUNCTION__, unValueLen1, unValueLen2, unValueLen3);
		return false;
	}
	unValueLen3 = MAX_ZSET_SCORE_LEN - unValueLen1 - unValueLen2;
	TopListCfgData clCfgData;
	clCfgData.szTableName = StringUtil::Format("TopList%d", eType);
	clCfgData.funcGetData = funcGetData;
	clCfgData.funcCanUpdate = funcCanUpdate;
	clCfgData.unListAmount = unAmount;
	clCfgData.unValueLen1 = unValueLen1;
	clCfgData.bAsc1 = bAsc1;
	clCfgData.unValueLen2 = unValueLen2;
	clCfgData.bAsc2 = bAsc2;
	clCfgData.unValueLen3 = unValueLen3;
	clCfgData.bAsc3 = bAsc3;
	m_mapTopListCfgData[eType] = clCfgData;
	m_mapTopListUpdate[eType] = false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		生成ZSet的Score
// Arguments:		unValue 生成Score的实际数值
//					bAsc 实际数值的排序是否为升序
//					unMaxValueLen Score的最大十进制位数 
// Autor：			李梦松
// Return：			QWORD 返回生成的Score
// Comment:			unValue 〈 i64MaxValue
//					返回值 >= ZMEM_ZSET_INVAILDSCORE表示生成失败
////////////////////////////////////////////////////////////////////////////////////////
QWORD TopListSystem::makeZSetScoreBase(DWORD unValue, bool bAsc, DWORD unMaxValueLen)
{
	if (0 == unMaxValueLen)
	{
		Fir::logger->error("[%s], 0 == unMaxValueLen", __PRETTY_FUNCTION__);
		return false;
	}

	if (unMaxValueLen > MAX_ZSET_SCORE_LEN)
	{
		Fir::logger->error("[%s], inMaxValueLen > MAX_ZSET_SCORE_LEN, unMaxValueLen = %d", __PRETTY_FUNCTION__, unMaxValueLen);
		return ZMEM_ZSET_INVAILDSCORE;
	}

	QWORD i64MaxValue = pow(10, unMaxValueLen);
	if (i64MaxValue < static_cast<QWORD>(unValue))
	{
		Fir::logger->error("[%s], i64MaxValue < static_cast<QWORD>(unValue), unValue = %d, i64MaxValue = %llu", 
			__PRETTY_FUNCTION__, unValue, i64MaxValue);
		return ZMEM_ZSET_INVAILDSCORE;
	}

	QWORD i64Score = ZMEM_ZSET_INVAILDSCORE;
	if (!bAsc)
	{
		// 降序则使用最大值-实际值
		i64Score = i64MaxValue - static_cast<QWORD>(unValue);
	}
	else
	{
		i64Score = static_cast<QWORD>(unValue);
	}
	return i64Score;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		解析ZSet的Score
// Arguments:		i64Score 待解析的Score
//					bAsc 实际数值的排序是否为升序
//					unLValueLen Score左半部分的最大十进制位数
//					unRValueLen Score右半部分的最大十进制位数
//					unValue 实际排序数值[out]
// Autor：			李梦松
// Return：			被解析过一次后的Score
////////////////////////////////////////////////////////////////////////////////////////
QWORD TopListSystem::unmakeZSetScoreBase(QWORD i64Score, bool bAsc, DWORD unLValueLen, DWORD unRValueLen, DWORD& unValue)
{
	unValue = 0;
	if (i64Score <= 0)
	{
		return 0;
	}
	if (0 == unLValueLen)
	{
		Fir::logger->error("[%s], 0 == unMaxValueLen", __PRETTY_FUNCTION__);
		return 0;
	}

	if (unLValueLen > MAX_ZSET_SCORE_LEN)
	{
		Fir::logger->error("[%s], inMaxValueLen > MAX_ZSET_SCORE_LEN, unLValueLen = %d", __PRETTY_FUNCTION__, unLValueLen);
		return 0;
	}

	QWORD i64LMaxValue = pow(10, unLValueLen);
	QWORD i64RMaxValue = pow(10, unRValueLen);
	QWORD i64TempValue = 0;
	if (unRValueLen == 0)
	{
		i64TempValue = i64Score;
	}
	else
	{
		i64TempValue = i64Score / i64RMaxValue;
	}
	if (!bAsc)
	{
		// 降序则使用最大值-实际值
		unValue =  i64LMaxValue - i64TempValue;
	}
	else
	{
		unValue = i64TempValue;
	}

	if (unRValueLen == 0)
	{
		return 0;
	}
	else
	{
		return (i64Score % i64RMaxValue);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		生成ZSet的Score
// Arguments:		unValue1 生成Score的实际数值1
//					bAsc1 实际数值1的排序是否为升序
//					unMaxValueLen1 Score1的最大十进制位数
//					unValue2 生成Score的实际数值2
//					bAsc2 实际数值2的排序是否为升序
//					unMaxValueLen2 Score1的最大十进制位数
//					unValue3 生成Score的实际数值3
//					bAsc3 实际数值3的排序是否为升序
//					unMaxValueLen3 Score1的最大十进制位数
// Autor：			李梦松
// Return：			QWORD 返回生成的Score
// Comment:			[unValue1,unValue2,unValue3] 〈 10000000i64
//					返回值 >= ZMEM_ZSET_INVAILDSCORE表示生成失败
////////////////////////////////////////////////////////////////////////////////////////
QWORD TopListSystem::makeZSetScore(DWORD unValue1, bool bAsc1, DWORD unMaxValueLen1, DWORD unValue2, bool bAsc2, DWORD unMaxValueLen2, DWORD unValue3, bool bAsc3, DWORD unMaxValueLen3)
{
	if (0 == unMaxValueLen1)
	{
		Fir::logger->error("[%s], 0 == unMaxValueLen1", __PRETTY_FUNCTION__);
		return ZMEM_ZSET_INVAILDSCORE;
	}
	if ((unMaxValueLen1 + unMaxValueLen2 + unMaxValueLen3) > MAX_ZSET_SCORE_LEN)
	{
		Fir::logger->error("[%s], (unMaxValueLen1 + unMaxValueLen2 + unMaxValueLen3) > MAX_ZSET_SCORE_LEN, unMaxValueLen1 = %d, unMaxValueLen2 = %d, unMaxValueLen3 = %d", 
							__PRETTY_FUNCTION__, unMaxValueLen1, unMaxValueLen2, unMaxValueLen3);
		return ZMEM_ZSET_INVAILDSCORE;
	}
	
	QWORD i64Score1 = this->makeZSetScoreBase(unValue1, bAsc1, unMaxValueLen1);
	if (i64Score1 >= ZMEM_ZSET_INVAILDSCORE)
	{
		Fir::logger->error("[%s], i64Score1 >= ZMEM_ZSET_INVAILDSCORE, i64Score1 = %llu", __PRETTY_FUNCTION__, i64Score1);
		return ZMEM_ZSET_INVAILDSCORE;
	}
	if (0 == unMaxValueLen2)
	{
		// 1个排序条件
		return i64Score1;
	}
	else
	{
		QWORD i64Score2 = this->makeZSetScoreBase(unValue2, bAsc2, unMaxValueLen2);
		if (i64Score2 >= ZMEM_ZSET_INVAILDSCORE)
		{
			Fir::logger->error("[%s], i64Score2 >= ZMEM_ZSET_INVAILDSCORE, i64Score2 = %llu", __PRETTY_FUNCTION__, i64Score2);
			return ZMEM_ZSET_INVAILDSCORE;
		}
		QWORD i64MaxValue2 = pow(10, unMaxValueLen2);
		if (0 == unMaxValueLen3)
		{
			// 2个排序条件
			return (i64Score1 * i64MaxValue2 + i64Score2);
		}
		else
		{
			// 3个排序条件
			QWORD i64Score3 = this->makeZSetScoreBase(unValue3, bAsc3, unMaxValueLen3);
			if (i64Score3 >= ZMEM_ZSET_INVAILDSCORE)
			{
				Fir::logger->error("[%s], i64Score3 >= ZMEM_ZSET_INVAILDSCORE, i64Score3 = %llu", __PRETTY_FUNCTION__, i64Score3);
				return ZMEM_ZSET_INVAILDSCORE;
			}
			QWORD i64MaxValue3 = pow(10, unMaxValueLen3);
			return (i64Score1 * i64MaxValue2 * i64MaxValue3 + i64Score2 * i64MaxValue3 + i64Score3);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		更新排行榜数据
// Arguments:		pcTableName 排行榜对应的表名
//					unKey 排序数据的Key
//					i64Score 排序Score
// Autor：			李梦松
// Return：			bool 返回更新结果
////////////////////////////////////////////////////////////////////////////////////////
void TopListSystem::updateTopListDataBase(const char* pcTableName, DWORD unAmount, DWORD unKey, QWORD i64Score)
{
	if (NULL == pcTableName)
	{
		Fir::logger->error("[%s], NULL == pcTableName", __PRETTY_FUNCTION__);
		return;
	}

	if (unAmount <= 0)
	{
		Fir::logger->error("[%s], unAmount <= 0, unAmount = %d", __PRETTY_FUNCTION__, unAmount);
		return;
	}

	if (i64Score >= ZMEM_ZSET_INVAILDSCORE)
	{
		Fir::logger->error("[%s], i64Score >= ZMEM_ZSET_INVAILDSCORE, i64Score = %llu", __PRETTY_FUNCTION__, i64Score);
		return;
	}

	zMemDB* hdlMemDB = zMemDBPool::getMe().getMemDBHandle();
	if (NULL == hdlMemDB) 
	{    
		Fir::logger->error("[%s], NULL == hdlMemDB", __PRETTY_FUNCTION__);
		return;
	}    

	std::string szKey = StringUtil::Format("%u", unKey);

	if (hdlMemDB->addDataToZSet(pcTableName, szKey.c_str(), i64Score) == false)
	{
		Fir::logger->error("[%s], hdlMemDB->addDataToZSet error, pcTableName = %s, unKey = %u, i64Score = %llu", 
							__PRETTY_FUNCTION__, pcTableName, unKey, i64Score);
		return;
	}

	std::vector<DWORD> vecKey;
	if (hdlMemDB->getZSetSize(pcTableName) > unAmount)
	{
		// 删除超出上限的数据
		if (hdlMemDB->delDataFromZSetByRank(pcTableName, -1, -1) == false)
		{
			Fir::logger->error("[%s], hdlMemDB->delDataFromZSetByRank error, pcTableName = %s, unKey = %u, i64Score = %llu", 
				__PRETTY_FUNCTION__, pcTableName, unKey, i64Score);
			return;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		获取排行榜的注册信息
// Arguments:		eType 排行榜类型
// Autor：			李梦松
// Return：			bool 返回注册信息
////////////////////////////////////////////////////////////////////////////////////////
const TopListCfgData* TopListSystem::getTopListCfgData(TOPLIST_TYPE eType)
{
	auto itFind = m_mapTopListCfgData.find(eType);
	if (itFind == m_mapTopListCfgData.end())
	{
		return NULL;
	}
	return &itFind->second;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		删除排行榜指定Key
// Arguments:		eType 排行榜类型
//					unKey 排序数据的Key
// Autor：			李梦松
// Return：			bool 返回更新结果
////////////////////////////////////////////////////////////////////////////////////////
void TopListSystem::delTopListData(TOPLIST_TYPE eType, DWORD unKey)
{
	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData", __PRETTY_FUNCTION__);
		return;
	}
	zMemDB* hdlMemDB = zMemDBPool::getMe().getMemDBHandle();
	if (NULL == hdlMemDB) 
	{    
		Fir::logger->error("[%s], NULL == hdlMemDB", __PRETTY_FUNCTION__);
		return;
	}    
	
	// 删除对应的数据
	std::string szKey = StringUtil::Format("%u", unKey);
	if (hdlMemDB->delDataFromZSetByKey(pCfgData->szTableName.c_str(), szKey.c_str()) == false)
	{
		Fir::logger->error("[%s], hdlMemDB->delDataFromZSetByRank error, eType = %d, unKey = %u", __PRETTY_FUNCTION__, eType, unKey);
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		清除排行榜
// Arguments:		eType 排行榜类型
// Autor：			李梦松
// Return：			void
////////////////////////////////////////////////////////////////////////////////////////
void TopListSystem::clearTopListData(TOPLIST_TYPE eType)
{
	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData", __PRETTY_FUNCTION__);
		return;
	}
	zMemDB* hdlMemDB = zMemDBPool::getMe().getMemDBHandle();
	if (NULL == hdlMemDB) 
	{    
		Fir::logger->error("[%s], NULL == hdlMemDB", __PRETTY_FUNCTION__);
		return;
	}
	hdlMemDB->delDataFromZSetByRank(pCfgData->szTableName.c_str(), 0, -1);
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		更新排行榜数据
// Arguments:		eType 排行榜类型
//					unKey 排序数据的Key
//					unValue1 排序数值1
//					unValue2 排序数值2
//					unValue3 排序数值3
// Autor：			李梦松
// Return：			bool 返回更新结果
////////////////////////////////////////////////////////////////////////////////////////
void TopListSystem::updateTopListData(TOPLIST_TYPE eType, DWORD unKey, DWORD unValue1, DWORD unValue2, DWORD unValue3)
{
	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData", __PRETTY_FUNCTION__);
		return;
	}
	// 注册回调中判定数据是否需要更新
	ST_CALLBACK_TOPLIST_CANUPDATE_PARAM stParam = {0};
	stParam.nTopListType = eType;
	stParam.unKey = unKey;
	stParam.unValue1 = unValue1;
	stParam.unValue2 = unValue2;
	stParam.unValue3 = unValue3;
	if (pCfgData->funcCanUpdate && pCfgData->funcCanUpdate(stParam) == false)
	{
		return;
	}
	// 更新排行榜数据
	QWORD i64Score = this->makeZSetScore(unValue1,  pCfgData->bAsc1, pCfgData->unValueLen1, unValue2, pCfgData->bAsc2, pCfgData->unValueLen2, unValue3, pCfgData->bAsc3, pCfgData->unValueLen3);
	if (i64Score >= ZMEM_ZSET_INVAILDSCORE)
	{
		Fir::logger->error("[%s], i64Score > = ZMEM_ZSET_INVAILDSCORE, i64Score = %llu", __PRETTY_FUNCTION__, i64Score);
		return;
	}
	this->updateTopListDataBase(pCfgData->szTableName.c_str(), pCfgData->unListAmount, unKey, i64Score);
	m_mapTopListUpdate[eType] = true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		获取排行榜指定排名的Key
// Arguments:		eType 排行榜类型
//					nRank 排行Rank
//					nEndPos 结束Rank
// Autor：			李梦松
// Return：			DWORD 返回Key
// Comment:			nRank 从0开始
////////////////////////////////////////////////////////////////////////////////////////
DWORD TopListSystem::getTopListKey(TOPLIST_TYPE eType, SDWORD nRank)
{
	if (nRank < 0)
	{
		Fir::logger->error("[%s], nRank < 0, eType = %d, nRank = %d", __PRETTY_FUNCTION__, eType, nRank);
		return 0;
	}
	std::vector<DWORD> vecKey;
	if (this->getTopListKey(eType, nRank, nRank, vecKey) == false)
	{
		Fir::logger->error("[%s], !this->getTopListKey(eType, nRank, nRank, vecKey), eType = %d, nRank = %d", __PRETTY_FUNCTION__, eType, nRank);
		return 0;
	}

	if (vecKey.size() <= 0)
	{
		Fir::logger->error("[%s], vecKey.size() <= 0, eType = %d, nRank = %d", __PRETTY_FUNCTION__, eType, nRank);
		return 0;	
	}
	return vecKey[0];
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		按Rank范围获取排行榜Key
// Arguments:		eType 排行榜类型
//					nBeginPos 起始Rank
//					nEndPos 结束Rank
//					vecKey 获取的Key集合 [OUT参数]
// Autor：			李梦松
// Return：			bool 返回操作是否成功
// Comment:			nBeginPos 从0开始
//					nEndPos = -1时表示ZSet最后一个数据
////////////////////////////////////////////////////////////////////////////////////////
bool TopListSystem::getTopListKey(TOPLIST_TYPE eType, SDWORD nBeginPos, SDWORD nEndPos, std::vector<DWORD>& vecKey)
{
	if (nBeginPos < 0)
	{
		Fir::logger->error("[%s], nBeginPos < 0, eType = %d, nBeginPos = %d, nEndPos = %d", __PRETTY_FUNCTION__, eType, nBeginPos, nEndPos);
		return false;
	}
	if (nEndPos < -1)
	{
		Fir::logger->error("[%s], nEndPos >= -1, eType = %d, nBeginPos = %d, nEndPos = %d", __PRETTY_FUNCTION__, eType, nBeginPos, nEndPos);
		return false;
	}

	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData, eType = %d, nBeginPos = %d, nEndPos = %d", __PRETTY_FUNCTION__, eType, nBeginPos, nEndPos);
		return false;
	}
	zMemDB* hdlMemDB = zMemDBPool::getMe().getMemDBHandle();
	if (NULL == hdlMemDB) 
	{    
		Fir::logger->error("[%s], NULL == hdlMemDB, eType = %d, nBeginPos = %d, nEndPos = %d", __PRETTY_FUNCTION__, eType, nBeginPos, nEndPos);
		return false;
	}

	if (hdlMemDB->getKeyFromZSetByRank(pCfgData->szTableName.c_str(), nBeginPos, nEndPos, vecKey) == false)
	{
		Fir::logger->error("[%s], hdlMemDB->getKeyFromZSetByRank error, eType = %d, nBeginPos = %d, nEndPos = %d", 
							__PRETTY_FUNCTION__, eType, nBeginPos, nEndPos);
		return false;
	}
	return true;
}

bool TopListSystem::getTopListScoreByKey(TOPLIST_TYPE eType, DWORD unKey, DWORD& unValue1, DWORD& unValue2, DWORD& unValue3)
{
	unValue1 = 0;
	unValue2 = 0;
	unValue3 = 0;
	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData, eType = %d", __PRETTY_FUNCTION__, eType);
		return false;
	}
	zMemDB* hdlMemDB = zMemDBPool::getMe().getMemDBHandle();
	if (NULL == hdlMemDB) 
	{    
		Fir::logger->error("[%s], NULL == hdlMemDB", __PRETTY_FUNCTION__);
		return false;
	}
	std::string szKey = StringUtil::Format("%u", unKey);
	SQWORD zset_score = hdlMemDB->getScoreZSetByKey(pCfgData->szTableName.c_str(), szKey.c_str());
	if (zset_score <= -1)
	{
		return false;
	}
	zset_score = unmakeZSetScoreBase(zset_score, pCfgData->bAsc1, pCfgData->unValueLen1, pCfgData->unValueLen2 + pCfgData->unValueLen3, unValue1);
	zset_score = unmakeZSetScoreBase(zset_score, pCfgData->bAsc2, pCfgData->unValueLen2, pCfgData->unValueLen3, unValue2);
	zset_score = unmakeZSetScoreBase(zset_score, pCfgData->bAsc3, pCfgData->unValueLen3, 0, unValue3);
	return (true);
}

int TopListSystem::getTopListRankByKey(TOPLIST_TYPE eType, DWORD unKey)
{
	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData, eType = %d", __PRETTY_FUNCTION__, eType);
		return 0;
	}
	zMemDB* hdlMemDB = zMemDBPool::getMe().getMemDBHandle();
	if (NULL == hdlMemDB) 
	{    
		Fir::logger->error("[%s], NULL == hdlMemDB", __PRETTY_FUNCTION__);
		return 0;
	}
	std::string szKey = StringUtil::Format("%u", unKey);
	int nRank = hdlMemDB->getRankZSetByKey(pCfgData->szTableName.c_str(), szKey.c_str());
	if (nRank <= -1)
	{
		return 0;
	}
	return (nRank + 1);
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		按Rank范围获取排行榜数据
// Arguments:		eType 排行榜类型
//					nBeginPos 起始Rank
//					nEndPos 结束Rank
//					vecData 获取的数据集合 [OUT参数]
// Autor：			李梦松
// Return：			bool 返回操作是否成功
// Comment:			nBeginPos 从0开始
//					nEndPos = -1时表示ZSet最后一个数据
////////////////////////////////////////////////////////////////////////////////////////
bool TopListSystem::getTopListDataByKey(TOPLIST_TYPE eType, DWORD unKey, UIN_TOPLIST_DATA_ST& uinData)
{
	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData, eType = %d", __PRETTY_FUNCTION__, eType);
		return false;
	}

	ST_CALLBACK_TOPLIST_GETDATA_PARAM stParam;
	stParam.unKey = unKey;
	stParam.nTopListType = eType;
	stParam.nRank = this->getTopListRankByKey(eType, unKey);
	TopListSystem::getMe().getTopListScoreByKey(eType, unKey, stParam.unValue1, stParam.unValue2, stParam.unValue3);
	if (pCfgData->funcGetData != NULL && pCfgData->funcGetData(stParam, uinData))
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		按Rank范围获取排行榜数据
// Arguments:		eType 排行榜类型
//					nBeginPos 起始Rank
//					nEndPos 结束Rank
//					vecData 获取的数据集合 [OUT参数]
// Autor：			李梦松
// Return：			bool 返回操作是否成功
// Comment:			nBeginPos 从0开始
//					nEndPos = -1时表示ZSet最后一个数据
////////////////////////////////////////////////////////////////////////////////////////
bool TopListSystem::getTopListDataSet(TOPLIST_TYPE eType, SDWORD nBeginPos, SDWORD nEndPos, std::vector<UIN_TOPLIST_DATA_ST>& vecData)
{
	if (nBeginPos < 0)
	{
		Fir::logger->error("[%s], nBeginPos < 0, nBeginPos = %d", __PRETTY_FUNCTION__, nBeginPos);
		return false;
	}
	if (nEndPos < -1)
	{
		Fir::logger->error("[%s], nEndPos >= -1, nEndPos = %d", __PRETTY_FUNCTION__, nEndPos);
		return false;
	}
	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData, eType = %d", __PRETTY_FUNCTION__, eType);
		return false;
	}
	if (nullptr == pCfgData->funcGetData)
	{
		Fir::logger->error("[%s], nullptr == pCfgData->funcGetData", __PRETTY_FUNCTION__);
		return false;
	}

	std::vector<DWORD> vecKey;
	if (this->getTopListKey(eType, nBeginPos, nEndPos, vecKey) == false)
	{
		Fir::logger->error("[%s], getTopListKey error, eType = %d, nBeginPos = %d, nEndPos = %d", __PRETTY_FUNCTION__, eType, nBeginPos, nEndPos);
		return false;
	}
	vecData.clear();
	int nRankDiff = 0;
	for (auto iter : vecKey)
	{
		// 调用注册的函数获取排行榜的显示数据
		ST_CALLBACK_TOPLIST_GETDATA_PARAM stParam;
		stParam.unKey = iter;
		stParam.nTopListType = eType;
		stParam.nRank = nBeginPos + nRankDiff + 1;
		TopListSystem::getMe().getTopListScoreByKey(eType, iter, stParam.unValue1, stParam.unValue2, stParam.unValue3);
		UIN_TOPLIST_DATA_ST uinTopListData;
		if (pCfgData->funcGetData != NULL && pCfgData->funcGetData(stParam, uinTopListData))
		{
			vecData.push_back(uinTopListData);
		}
		nRankDiff++;	// History:[2] 
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////
// Description:		获取排行榜大小
// Arguments:		eType 排行榜类型
// Autor：			李梦松
// Return：			
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
DWORD TopListSystem::getTopListSize(TOPLIST_TYPE eType)
{
	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData", __PRETTY_FUNCTION__);
		return 0;
	}
	zMemDB* hdlMemDB = zMemDBPool::getMe().getMemDBHandle();
	if (NULL == hdlMemDB) 
	{    
		Fir::logger->error("[%s], NULL == hdlMemDB", __PRETTY_FUNCTION__);
		return 0;
	}

	DWORD nSize = hdlMemDB->getZSetSize(pCfgData->szTableName.c_str());
	return nSize;
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		储存指定排行榜
// Arguments:		eType 排行榜类型
// Autor：			李梦松
// Return：			
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
void TopListSystem::saveTopList(TOPLIST_TYPE eType)
{
	if (m_mapTopListUpdate[eType] == false)
	{
		// 如果排行榜未更新则不存库
		return;
	}
	const TopListCfgData* pCfgData = this->getTopListCfgData(eType);
	if (NULL == pCfgData)
	{
		Fir::logger->error("[%s], NULL == pCfgData", __PRETTY_FUNCTION__);
		return;
	}
	
	// 通知RecordServer存储该条排行榜
	BUFFER_CMD(CMD::RECORD::t_WriteToplist_SceneRecord, sendCmd, zSocket::MAX_DATASIZE);
	sendCmd->nTopListType = eType;
	recordClient->sendCmd(sendCmd, sizeof(CMD::RECORD::t_WriteToplist_SceneRecord));
	// 更新完成
	m_mapTopListUpdate[eType] = false;
	Fir::logger->info("[通知存储排行榜], 成功, eType = %d", eType);
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		更新存储一次排行榜
// Arguments:		
// Autor：			李梦松
// Return：			
// Comment:			
////////////////////////////////////////////////////////////////////////////////////////
void TopListSystem::updateOnce()
{
	static int TOPLIST_ONTIMER_SAVE_COUNT = 0;

	// 保存一个排行榜
	int nCount = 0;

	// 更新当前轮询到的排行榜
	for (auto iter : m_mapTopListCfgData)
	{
		if (nCount == TOPLIST_ONTIMER_SAVE_COUNT)
		{
			this->saveTopList(iter.first);
		}
		nCount++;
	}

	// 移动计数到下一个排行榜
	TOPLIST_ONTIMER_SAVE_COUNT = (TOPLIST_ONTIMER_SAVE_COUNT + 1) % m_mapTopListCfgData.size();
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		储存所有排行榜
// Arguments:		
// Autor：			李梦松
// Return：			
// Comment:			在服务关闭前调用
////////////////////////////////////////////////////////////////////////////////////////
void TopListSystem::saveAllTopList()
{
	for (auto iter : m_mapTopListCfgData)
	{
		this->saveTopList(iter.first);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Description:		排行榜模块定时器(1分钟)
// Arguments:		
// Autor：			李梦松
// Return：			
// Comment:			一分钟更新一条排行榜
////////////////////////////////////////////////////////////////////////////////////////
void TopListSystem::update()
{
	for (int i = 0; i < UPDATE_NUM; i++)
	{
		this->updateOnce();
	}
}
