////////////////////////////////////////////////////////////////////////////////////////
// CopyRight(C),
// FileName:	TopListSystem.h
// Author:		李梦松
// Version:
// Date:		2015-11-10
// Description:	通用排行榜注册模块(包含排行榜更新、获取数据等接口)
// History:		[1] 2015-11-10 19:41 创建文件
//				[2] 2015-11-13 20:20 修正了获取的玩家排行有几率错误的BUG
//				[3] 2015-11-23 21:00 增加了获取排行榜指定KEY的数据的接口
//									 将排行榜注册接口重定义为公有, 交由个调用模块使用
////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TOPLIST_SYSTEM_H_
#define _TOPLIST_SYSTEM_H_

#include "zSingleton.h"
#include "TopListDataDef.h"
#include "zMemDB.h"

// 排行榜配置类
class TopListCfgData
{
public:
	TopListCfgData() 
	: funcGetData(nullptr)
	, funcCanUpdate(nullptr)
	, unListAmount(0)
	, unValueLen1(0)
	, unValueLen2(0)
	, unValueLen3(0)
	, bAsc1(true)
	, bAsc2(true)
	, bAsc3(true)
	{ }
	~TopListCfgData() { }
public:
	std::string szTableName;				// 排行榜的表名
	FUNC_TOPLIST_GETDATA funcGetData;		// 获取指定Key的数据
	FUNC_TOPLIST_CANUPDATE funcCanUpdate;	// 判断能否更新排行榜数据
	DWORD unListAmount;						// 排行榜的最大数量
	DWORD unValueLen1;						// 第1个排序数的十进制位数 
	DWORD unValueLen2;						// 第2个排序数的十进制位数
	DWORD unValueLen3;						// 第3个排序数的十进制位数 
	bool bAsc1;								// 第1个排序数是否升序
	bool bAsc2;								// 第2个排序数是否升序
	bool bAsc3;								// 第3个排序数是否升序
};

class TopListSystem : public Fir::Singleton<TopListSystem>
{
	public:
		TopListSystem();
		~TopListSystem();

	public:
		// 排行榜初始化
		bool init();
		// 排行榜销毁
		void Release();
		// 排行榜重新加载
		bool ReLoad();
	public:
		// 注册排行榜(1条排序规则)
		bool registerTopList1Value(TOPLIST_TYPE eType, FUNC_TOPLIST_GETDATA funcGetData, DWORD unAmount, bool bAsc, 
									FUNC_TOPLIST_CANUPDATE funcCanUpdate = nullptr);
		// 注册排行榜(2条排序规则)
		bool registerTopList2Value(TOPLIST_TYPE eType, FUNC_TOPLIST_GETDATA funcGetData, DWORD unAmount, bool bAsc1, bool bAsc2, DWORD unValueLen1, DWORD unValueLen2, 
									FUNC_TOPLIST_CANUPDATE funcCanUpdate = nullptr);
		// 注册排行榜(3条排序规则)
		bool registerTopList3Value(TOPLIST_TYPE eType, FUNC_TOPLIST_GETDATA funcGetData, DWORD unAmount, bool bAsc1, bool bAsc2, bool bAsc3, DWORD unValueLen1, DWORD unValueLen2, DWORD unValueLen3, 
									FUNC_TOPLIST_CANUPDATE funcCanUpdate = nullptr);
	protected:
		// 根据1条排序规则创建ZSet的Score
		QWORD makeZSetScoreBase(DWORD unValue, bool bAsc, DWORD unMaxValueLen = MAX_ZSET_SCORE_LEN);
		// 解析ZSet的Score
		QWORD unmakeZSetScoreBase(QWORD i64Score, bool bAsc, DWORD unLValueLen, DWORD unRValueLen, DWORD& unValue);
		// 根据多条排序规则创建ZSet的Score
		QWORD makeZSetScore(DWORD unValue1, bool bAsc1, DWORD unMaxValueLen1, DWORD unValue2 = 0, bool bAsc2 = true, DWORD unMaxValueLen2 = 0, DWORD unValue3 = 0, bool bAsc3 = true, DWORD unMaxValueLen3 = 0);
		// 更新排行榜对应的ZSet中的Score
		void updateTopListDataBase(const char* pcTableName, DWORD unAmount, DWORD unKey, QWORD i64Score);
		// 获取排行榜配置信息
		const TopListCfgData* getTopListCfgData(TOPLIST_TYPE eType);
		
	public:
		// 删除排行榜指定Key
		void delTopListData(TOPLIST_TYPE eType, DWORD unKey);
		// 清除排行榜
		void clearTopListData(TOPLIST_TYPE eType);
		// 更新排行榜
		void updateTopListData(TOPLIST_TYPE eType, DWORD unKey, DWORD unValue1, DWORD unValue2 = 0, DWORD unValue3 = 0);
		// 获取排行榜指定排名的Key
		DWORD getTopListKey(TOPLIST_TYPE eType, SDWORD nRank);
		// 获取排行榜指定位置的Key集合
		bool getTopListKey(TOPLIST_TYPE eType, SDWORD nBeginPos, SDWORD nEndPos, std::vector<DWORD>& vecKey);
		// 获取排行榜的排序数据
		bool getTopListScoreByKey(TOPLIST_TYPE eType, DWORD unKey, DWORD& unValue1, DWORD& unValue2, DWORD& unValue3);
		// 获取排行榜指定KEY的排名
		int getTopListRankByKey(TOPLIST_TYPE eType, DWORD unKey);
		// 获取排行榜指定KEY的数据
		bool getTopListDataByKey(TOPLIST_TYPE eType, DWORD unKey, UIN_TOPLIST_DATA_ST& uinData);
		// 获取排行榜指定位置的数据集合
		bool getTopListDataSet(TOPLIST_TYPE eType, SDWORD nBeginPos, SDWORD nEndPos, std::vector<UIN_TOPLIST_DATA_ST>& vecData);
		// 获取排行榜大小
		DWORD getTopListSize(TOPLIST_TYPE eType);
	private:
		// 储存指定排行榜
		void saveTopList(TOPLIST_TYPE eType);
		// 更新存储一次排行榜
		void updateOnce();

	public:
		// 储存所有排行榜
		void saveAllTopList();
		// 排行榜模块定时器(1分钟)
		void update();

private:
		std::map<TOPLIST_TYPE, TopListCfgData>	m_mapTopListCfgData;	// 排行榜注册信息存储MAP
		std::map<TOPLIST_TYPE, bool>			m_mapTopListUpdate;		// 排行榜更新信息
};
#endif
