#include "GmManager.h"
#include "SessionServer.h"
#include "zDBConnPool.h"
#include "StringUtil.h"
#include "zCmdBuffer.h"
#include "SessionUser.h"
#include "SessionUserManager.h"
#include "RedisMgr.h"
#include "SessionTaskManager.h"

#include "zBILog.h"
#include "RecordCommand.h"
#include "RecordClient.h"
#include "zMisc.h"
#include "proto/monitor_protocol.pb.h"

#include <json/json.h>


/*
* \ 充值道具元宝ID的长度
*/
#define MAX_ITEM_ID_LENGTH (10)
/*
* \ 充值道具的数量，（角色ID为数字）
*/
#define MAX_ITEM_NUMBER_LENGTH (10)


void MakePbGmRquet(PbGmRequest &binary, int nroleid, int ngoodsid, int ngoodsnum, std::string amount, std::string orderid, std::string innerorderid, std::string paytype)
{
	//角色ID
	PbPropertyItem* pitem = NULL;
	pitem = binary.add_pb_items();
	pitem->set_name("charid");
	char charid[MAX_CHAR_ID_LENGTH+1];
	bzero(charid,sizeof(charid));
	pitem->set_value(StringUtil::ITOA(nroleid,charid,MAX_CHAR_ID_LENGTH));
	//购买的充值道具（元宝ID）
	pitem = binary.add_pb_items();
	pitem->set_name("goodsid");
	char goodsid[MAX_ITEM_ID_LENGTH+1];
	bzero(goodsid,sizeof(goodsid));
	pitem->set_value(StringUtil::ITOA(ngoodsid,goodsid,MAX_ITEM_ID_LENGTH));
	//购买的充值道具的数量，一般情况下，恒为1
	pitem = binary.add_pb_items();
	pitem->set_name("goodsnum");
	char goodsnum[MAX_ITEM_NUMBER_LENGTH+1];
	bzero(goodsnum,sizeof(goodsnum));
	pitem->set_value(StringUtil::ITOA(ngoodsnum,goodsnum,MAX_ITEM_NUMBER_LENGTH));
	//购买的元宝花费的人民币，单位为分
	pitem = binary.add_pb_items();
	pitem->set_name("amount");
	pitem->set_value(amount);
	//渠道订单流水号
	pitem = binary.add_pb_items();
	pitem->set_name("orderid");
	pitem->set_value(orderid);
	//内部订单流水号
	pitem = binary.add_pb_items();
	pitem->set_name("innerorderid");
	pitem->set_value(innerorderid);
	// 支付TYPE
	pitem = binary.add_pb_items();
	pitem->set_name("paytype");
	pitem->set_value(paytype);
}

void GmManager::send_query_rechage_super(DWORD http_id, std::string json)
{
	Fir::logger->info("%s,httpid=%u,json=%s", __PRETTY_FUNCTION__, http_id, json.c_str());
	
	zCmdBufferAdapterT<CMD::SUPER::t_PayQuery_Result_Session2Super> send;

	send->http_id = http_id;
	send->datasize = json.size();

	if (send->getSize() > zSocket::MAX_DATASIZE)
	{
		Fir::logger->error("[充值查询]发送数据过大, data size %u", send->getSize());
		return;
	}

	if (send.write(json.c_str(), send->datasize))
	{
		SessionService::getMe().sendCmdToSuperServer(send.data(),send.size());
	}
}

bool GmManager::handle_query_recharge(DWORD http_id, const std::string& content)
{
	Fir::logger->info("%s,content=%s", __PRETTY_FUNCTION__,content.c_str());
	
	Json::Value root;
	Json::FastWriter writer;
	std::string send_msg;
	
	Json::Value json_val;
	Json::Reader reader;
	if (!reader.parse(content, json_val))
	{
		send_msg = StringUtil::Format("[充值订单查询]JSON解析错粗");
		root["code"] = 1;
		root["msg"] = send_msg;
		std::string json = writer.write(root);
		send_query_rechage_super(http_id, json);
		Fir::logger->error(send_msg.c_str());
		return false;
	}

	if(!json_val.isMember("data"))
	{
		send_msg = StringUtil::Format("[充值订单查询]JSON中缺少data参数");
		root["code"] = 1;
		root["msg"] = send_msg;
		std::string json = writer.write(root);
		send_query_rechage_super(http_id, json);
		Fir::logger->error(send_msg.c_str());
		return false;
	}

	Json::Value jsData = json_val["data"];
	if(!jsData.isObject())
	{
		send_msg = StringUtil::Format("[充值订单查询]JSON中data参数无法获取");
		root["code"] = 1;
		root["msg"] = send_msg;
		std::string json = writer.write(root);
		send_query_rechage_super(http_id, json);
		Fir::logger->error(send_msg.c_str());
		return false;
	}

	if (!jsData["innerorderid"].isString())
	{
		send_msg = StringUtil::Format("[充值订单查询]JSON DATA中innerorderid参数必须是字符串");
		root["code"] = 1;
		root["msg"] = send_msg;
		std::string json = writer.write(root);
		send_query_rechage_super(http_id, json);
		Fir::logger->error(send_msg.c_str());
		return false;
	}

	// 内部订单号
	std::string innerorder_id = jsData["innerorderid"].asString();
	if (innerorder_id.length() != INNER_ORDER_LENGTH)
	{
		send_msg = StringUtil::Format("[充值订单查询]JSON DATA中innerorderid参数长度不符");
		root["code"] = 1;
		root["msg"] = send_msg;
		std::string json = writer.write(root);
		send_query_rechage_super(http_id, json);
		Fir::logger->error(send_msg.c_str());
		return false;
	}

	auto recharge_count = this->get62RechargeCountInt(innerorder_id);
	auto role_id = this->get62CharidInt(innerorder_id);

	// 查询订单状态
	std::string select_sql = StringUtil::Format("select * from t_inner_order_info where f_rechargecount = %d AND f_roleid = %d", recharge_count, role_id);
	RecordSetPtr pResult = SessionService::dbConnPool->exeSelect(select_sql);
	if (NULL == pResult)
	{
		send_msg = StringUtil::Format("[充值订单查询]查询数据库失败");
		root["code"] = 1;
		root["msg"] = send_msg;
		std::string json = writer.write(root);
		send_query_rechage_super(http_id, json);
		Fir::logger->error(send_msg.c_str());
		return false;
	}
	if(pResult->empty())
	{
		send_msg = StringUtil::Format("[充值订单查询]订单不存在");
		root["code"] = 1;
		root["msg"] = send_msg;
		std::string json = writer.write(root);
		send_query_rechage_super(http_id, json);
		Fir::logger->error(send_msg.c_str());

		Fir::logger->info("订单号 = %s，角色ID = %u，充值数量 = %u", innerorder_id.c_str(), role_id, recharge_count);
		return false;
	}
	Record* rec = pResult->get(0);
	if (NULL == rec)
	{
		send_msg = StringUtil::Format("[充值订单查询]获取数据库数据失败");
		root["code"] = 1;
		root["msg"] = send_msg;
		std::string json = writer.write(root);
		send_query_rechage_super(http_id, json);
		Fir::logger->error(send_msg.c_str());
		return false;
	}
	DWORD uid					= rec->get("f_roleid");						// 角色ID
	std::string id				= std::string(rec->get("f_orderid"));		// 充值单号
	DWORD amount				= rec->get("f_amount");						// 充值金额(单位:分)
	std::string createtime		= std::string(rec->get("f_paytime"));		// 充值时间
	DWORD itemid				= rec->get("f_goodsid");					// 道具id
	DWORD itemquantity			= rec->get("f_goodsnum");					// 道具数量
	DWORD status				= rec->get("f_status");						// 订单状态
	std::string info			= "";										// 备用字段

	root["code"] = 0;
	root["msg"] = "OK";
	root["uid"] = uid;
	root["id"] = id;
	root["innerorderid"] = innerorder_id;
	root["amount"] = amount;
	root["createtime"] = createtime;
	root["itemid"] = itemid;
	root["itemquantity"] = itemquantity;
	root["status"] = status;
	root["info"] = info;

	std::string json = writer.write(root);
	send_query_rechage_super(http_id, json);
	return true;
}

DWORD GmManager::get62CharidInt(std::string inner_order_id)
{
	if (inner_order_id.length() != INNER_ORDER_LENGTH)
		return 0;
	return zMisc::system62toi(inner_order_id.substr(0, INNER_ORDER_ROLEID_LENGTH));
}

DWORD GmManager::get62RechargeCountInt(std::string inner_order_id)
{
	if (inner_order_id.length() != INNER_ORDER_LENGTH)
		return 0;
	return zMisc::system62toi(inner_order_id.substr(INNER_ORDER_ROLEID_LENGTH, INNER_ORDER_RECHARGECOUNT_LENGTH));
}

std::string GmManager::get62Charid(DWORD charid)
{
	return zMisc::itosystem62(charid, INNER_ORDER_ROLEID_LENGTH);
}

std::string GmManager::get62RechargeCount(DWORD num)
{
	return zMisc::itosystem62(num, INNER_ORDER_RECHARGECOUNT_LENGTH);
}

bool GmManager::handle_recharge(const std::string& content)
{
	Fir::logger->trace("%s,content=%s", __PRETTY_FUNCTION__,content.c_str());

	Json::Value json;
	Json::Reader reader;
	if (!reader.parse(content, json))
	{
		Fir::logger->info("[充值处理]失败，JSON参数解析错误，json=%s",content.c_str());
		return false;
	}

	if(!json.isObject())
	{
		Fir::logger->info("[充值处理]失败，JSON参数格式错误，json=%s", content.c_str());
		return false;
	}

	if (!json.isMember("appid") || !json.isMember("sdkid") || !json.isMember("channel") || !json.isMember("serverid"))
	{
		Fir::logger->info("[充值处理]失败，参数缺少必要成员，json=%s", content.c_str());
		return false;
	}

	if (!json["appid"].isString() || !json["sdkid"].isString() || !json["channel"].isString() || !json["serverid"].isString())
	{
		Fir::logger->info("[充值处理]失败，参数类型错误，json=%s", content.c_str());
		return false;
	}

	if(!json.isMember("data"))
	{
		Fir::logger->info("[充值处理]失败，参数data缺少，json=%s", content.c_str());
		return false;
	}

	Json::Value jsData = json["data"];
	if(!jsData.isObject())
	{
		Fir::logger->info("[充值处理]失败，参数data格式错误，json=%s", content.c_str());
		return false;
	}

	if (!jsData.isMember("id") || !jsData.isMember("innerorderid") || !jsData.isMember("paytype") || !jsData.isMember("amount") || !jsData.isMember("sign") || !jsData.isMember("paytime") || !jsData.isMember("placeordertime"))
	{
		Fir::logger->info("[充值处理]失败，参数data缺少必要成员，json=%s", content.c_str());
		return false;
	}

	if (!jsData["id"].isString() || !jsData["innerorderid"].isString() || !jsData["paytype"].isString() || !jsData["amount"].isString() || !jsData["sign"].isString() || !jsData["paytime"].isString() || !jsData["placeordertime"].isString())
	{
		Fir::logger->info("[充值处理]失败，参数data成员类型错误，json=%s", content.c_str());
		return false;
	}

	// APP标识
	std::string appid = json["appid"].asString();
	// SDK类型
	std::string sdkid = json["sdkid"].asString();
	// 渠道标识
	std::string channel = json["channel"].asString();
	// 游戏服ID
	std::string serverid = json["serverid"].asString();

	// 渠道订单号
	std::string orderid = jsData["id"].asString();
	// 内部订单号
	std::string innerorderid = jsData["innerorderid"].asString();
	// 支付类型
	std::string paytype = jsData["paytype"].asString();
	// 实际金额
	std::string amount = jsData["amount"].asString();
	// 签名参数
	std::string sign = jsData["sign"].asString();
	// 付款成功时间
	std::string paytime = jsData["paytime"].asString();
	// 下订单时间
	std::string placeordertime = jsData["placeordertime"].asString();

	if (innerorderid.length() != INNER_ORDER_LENGTH)
	{
		Fir::logger->info("[充值处理]失败，内部订单号非法，订单号 = %s", innerorderid.c_str());
		return false;
	}
	// 角色ID
	DWORD roleid = get62CharidInt(innerorderid);
	// 角色充值索引
	DWORD rechargecount = get62RechargeCountInt(innerorderid);

	// 更新订单信息
	std::string sql = StringUtil::Format("call do_order_info('%s', '%s', '%s', '%s', %d, %d, '%s', '%s', '%s', '%s', '%s', '%s')", 
		appid.c_str(), serverid.c_str(), sdkid.c_str(), channel.c_str(), rechargecount, roleid, orderid.c_str(), paytype.c_str(), amount.c_str(), sign.c_str(), paytime.c_str(), placeordertime.c_str());

	RecordSetPtr pResult = SessionService::dbConnPool->exeSelect(sql);
	if (NULL == pResult || pResult->empty())
	{
		Fir::logger->error("[充值处理]失败，数据库查询失败, 订单号 = %s，角色ID = %u", innerorderid.c_str(), roleid);
		return false;
	}

	Record* rec = pResult->get(0);
	if (NULL == rec)
	{
		Fir::logger->error("[充值处理]失败，数据库记录不存在, 订单号 = %s，角色ID = %u", innerorderid.c_str(), roleid);
		return false;
	}

	DWORD ret = rec->get("ret");
	if (0 == ret)
	{
		std::string retmsg = rec->get("msg").getString();

		Fir::logger->info("[充值处理]失败，数据库记录不存在, 订单号 = %s，原因 = %s，角色ID = %u", innerorderid.c_str(), retmsg.c_str(), roleid);
		return false;
	}

	// 商品ID
	int goodsid = rec->get("f_goodsid");
	// 商品数量
	int goodsnum = rec->get("f_goodsnum");

	// 构造指令
	zCmdBufferAdapterT<CMD::SESSION::t_RechargeNotify_SessionScene> send;
	PbGmRequest binary;
	MakePbGmRquet(binary, roleid, goodsid, goodsnum, amount, orderid, innerorderid, paytype);

	char buffer[zSocket::MAX_DATASIZE];
	bzero(buffer, sizeof(buffer));
	binary.SerializeToArray(buffer,binary.ByteSize());
	send->datasize = binary.ByteSize();
	if (!send.write(&buffer, send->datasize))
	{
		Fir::logger->error("[充值处理]失败，构造场景命令失败, 订单号 = %s，json = %s，角色ID = %u", innerorderid.c_str(), content.c_str(), roleid);
		return false;
	}

	// 转发到场景
	if (!sendCmdToSceneUser(roleid, send.data(), send.size()))
	{
		Fir::logger->error("[充值处理]失败，转发到场景玩家失败, 订单号 = %s，json = %s，角色ID = %u", innerorderid.c_str(), content.c_str(), roleid);
		return false;
	}
	Fir::logger->trace("[充值处理]成功，订单号 = %s，角色ID = %u", innerorderid.c_str(), roleid);
	
	return true;
}

// 处理gm命令
bool GmManager::handle_gm_cmd(DWORD httpid, zProperties &paras, const std::string& content)
{
	Fir::logger->info("%s,httpid=%u,paras=%s", __PRETTY_FUNCTION__, httpid, paras.toString().c_str());

	if(!paras.find("CmdId"))
	{
		Fir::logger->error("[gm] 参数不对");
		return false;
	}

	DWORD cmdid = (DWORD)StringUtil::Atoi64(paras["CmdId"].c_str());
	auto iter = m_HandleFuncMap.find(cmdid);
	if (iter != m_HandleFuncMap.end())
	{
		(iter->second)(httpid,paras);    
		return true;
	}

	//全服公告，不需要验证角色ID
	// 禁言列表 封停列表 全服道具发放
	if (10017 == cmdid || 10030 == cmdid || 10031 == cmdid || 10014 == cmdid || 10015 == cmdid || 20016 == cmdid || 20017 == cmdid || 20018 == cmdid || 20023 == cmdid || 20024 == cmdid || 20025 == cmdid)
	{
		zCmdBufferAdapterT<CMD::SESSION::t_GmRequest_SessionScene> send;
		send->datasize = content.length();
		if (send.write(content.c_str(),send->datasize))
		{
			SessionTaskManager::getMe().broadcastByType(SCENESSERVER,send.data(),send.size());
		}
		
		return true;
	}
	else if (10009 == cmdid || 20026 == cmdid)
	{
		// 查询数据库的GM命令交给档案服务器处理
		BUFFER_CMD(CMD::RECORD::t_GmRequest_SessionRecord, cmd, zSocket::MAX_DATASIZE);
		cmd->datasize = content.length();
		bcopy(content.c_str(), &cmd->data, cmd->datasize);
		recordClient->sendCmd(cmd, sizeof(CMD::RECORD::t_GmRequest_SessionRecord) + cmd->datasize);
		return true;
	}

	if (!paras.find("RoleID") && !paras.find("roleId"))
	{
		// 转发GM命令到scene服务器
		zCmdBufferAdapterT<CMD::SESSION::t_GmRequest_SessionScene> send;
		send->datasize = content.length();
		if (send.write(content.c_str(),send->datasize))
		{
			SessionTaskManager::getMe().broadcastByType(SCENESSERVER,send.data(),send.size());
		}
		return true;
	}
	
	DWORD charid = 0;
	if (paras.find("RoleID"))
	{
		charid = (DWORD)StringUtil::Atoi64(paras["RoleID"].c_str());
	}
	else
	{
		charid = (DWORD)StringUtil::Atoi64(paras["roleId"].c_str());
	}

	// 构造指令
	zCmdBufferAdapterT<CMD::SESSION::t_GmRequest_SessionScene> send;
	send->datasize = content.length();
	if (!send.write(content.c_str(), send->datasize))
	{
		if (cmdid > 20000)
		{
			send_gm_error_new(httpid, "构造场景命令失败");
		}
		else
		{
			send_gm_error(httpid, "构造场景命令失败");
		}
		return false;
	}
	// 转发到场景
	if (!sendCmdToSceneUser(charid, send.data(), send.size()))
	{
		if (cmdid > 20000)
		{
			send_gm_error_new(httpid, "转发到场景失败");
		}
		else
		{
			send_gm_error(httpid, "转发到场景失败");
		}
		return false;
	}

	return true;
}

void GmManager::send_gm_error(DWORD httpid,std::string msg)
{
	Json::Value root;
	Json::FastWriter writer;
	root["res"] = "-1";
	root["reason"] = msg;
	std::string result = writer.write(root);
	SessionService::getMe().sendGMContent(httpid, result);
}

void GmManager::send_gm_error_new(DWORD httpid, std::string msg)
{
	Json::Value root;
	Json::FastWriter writer;
	root["errorCode"] = 1008;
	root["errorMsg"] = msg;
	std::string result = writer.write(root);
	SessionService::getMe().sendGMContent(httpid, result);
}

bool GmManager::__RoleSearch(DWORD httpid, zProperties &paras)
{
	std::string nickname = paras.getProperty("Name");
	std::string strCharId = paras.getProperty("RoleID");
	UINT32 charId = StringUtil::Atoint(strCharId.c_str());
	//std::string strCmdId = nickname["CmdId"];
	std::string strOffset = paras.getProperty("Offset");
	std::string strRowNum = paras.getProperty("RowNum");

	Json::Value root;
	Json::FastWriter writer;

	root["count"] = get_fuzzy_role_count(nickname);
	std::vector<stFuzzyRoleItem> vecItems = get_fuzzy_roles(charId, nickname, strOffset, strRowNum);
	for (auto itr=vecItems.begin(); itr!=vecItems.end(); itr++)
	{
		const stFuzzyRoleItem& item = *itr;

		Json::Value temp;
		
		temp["PlayerID"] = item.charid;				// 角色ID
		temp["AccountID"] = item.account;			// 角色账号
		temp["name"] = item.nickname;				// 角色名
		temp["PlatformID"] = item.platform;			// 角色平台标识
		temp["level"] = item.level;					// 角色等级
		temp["vip"] = item.vip;						// 角色VIP等级
		temp["create_time"] = item.createtime;		// 角色创建时间
		temp["last_login"] = item.onlinetime;		// 角色登录时间
		temp["last_logout"] = item.offlinetime;		// 角色下线时间

		root["info"].append(temp);
	}
	root["count"] = StringUtil::Format("%d",vecItems.size());
	std::string result = writer.write(root);

	SessionService::getMe().sendGMContent(httpid, result);

	return true;
}

std::string GmManager::get_fuzzy_role_count(const std::string& nickname)
{
	Fir::logger->info("%s,step1", __PRETTY_FUNCTION__);

	std::string nicknameEscaped;
	SessionService::dbConnPool->escapeString(nickname, nicknameEscaped);

	std::string strsql = StringUtil::Format ("select COUNT(1) as f_count from t_charbase WHERE `f_nickname` = '%s';",nicknameEscaped.c_str());
	RecordSetPtr pResult = SessionService::dbConnPool->exeSelect(strsql);
	if (NULL==pResult || pResult->empty())
	{
		Fir::logger->error("%s,step2,sql=%s", __PRETTY_FUNCTION__,strsql.c_str());
		return "0";
	}

	Record* rec = pResult->get(0);
	if (NULL == rec)
	{
		Fir::logger->error("%s,step3,sql=%s", __PRETTY_FUNCTION__,strsql.c_str());
		return "0";
	}

	std::string strcount = rec->get("f_count").getString();

	return strcount;
}

std::vector<GmManager::stFuzzyRoleItem> GmManager::get_fuzzy_roles(UINT32 charid, const std::string& nickname, const std::string& offset, const std::string& limit)
{
	Fir::logger->info("%s,step1", __PRETTY_FUNCTION__);

	std::vector<stFuzzyRoleItem> vecItems;

	std::string strsql = "SELECT f_charid,f_nickname,f_level,f_vip, f_createtime, f_onlinetime, f_offlinetime,f_accountid,f_platform,f_account FROM t_charbase  WHERE ";
	if (charid > 0)
	{
		strsql.append(StringUtil::Format("`f_charid` = %d", charid).c_str());
	}
	else if(!nickname.empty())
	{
		/*strsql = StringUtil::Format("SELECT f_charid,f_nickname,f_level,f_vip,f_offlinetime,f_accountid,f_platform,f_account FROM t_charbase  WHERE `f_nickname` LIKE '%%%s%%' limit %s, %s",
			nickname.c_str(), offset.c_str(), limit.c_str());*/

		std::string nicknameEscaped;
		SessionService::dbConnPool->escapeString(nickname, nicknameEscaped);
		strsql.append(StringUtil::Format("`f_nickname`  = '%s'", nicknameEscaped.c_str()).c_str());
	}
	else
	{
		return vecItems;
	}
	
	Fir::logger->info("%s,step2,sql=%s", __PRETTY_FUNCTION__,strsql.c_str());

	RecordSetPtr pResult = SessionService::dbConnPool->exeSelect(strsql);
	if (NULL == pResult || pResult->empty())
	{
		Fir::logger->info("%s,step3,sql=%s", __PRETTY_FUNCTION__,strsql.c_str());
		return vecItems;
	}

	for (DWORD i=0; i<pResult->size(); i++)
	{
		Record* rec = pResult->get(i);
		if (NULL==rec)
		{
			continue;
		}
		
		stFuzzyRoleItem t;
		t.charid = rec->get("f_charid").getString();
		t.nickname = rec->get("f_nickname").getString();
		t.level = rec->get("f_level").getString();
		t.vip = rec->get("f_vip").getString();
		t.createtime = rec->get("f_createtime").getString();
		t.onlinetime = rec->get("f_onlinetime").getString();
		t.offlinetime = rec->get("f_offlinetime").getString();
		t.accountid = rec->get("f_accountid").getString();
		t.platform = rec->get("f_platform").getString();
		t.account = rec->get("f_account").getString();
		Fir::logger->info("%s,step5,line", __PRETTY_FUNCTION__);
		vecItems.push_back(t);
	}
	
	return vecItems;
}

bool GmManager::init()
{
	__RegisterHandleFunc(10000, std::bind(&GmManager::__RoleSearch, this, std::placeholders::_1, std::placeholders::_2));	

	return true;
}

void GmManager::__RegisterHandleFunc (int id, HANDLE_FUNC func)
{
	m_HandleFuncMap.insert (make_pair (id, func));
}

bool GmManager::sendCmdToSceneUser(DWORD charid, const void *cmd, const int cmdlen)
{
	SessionUser *u = SessionUserManager::getMe().getUserByCharID(charid);
	// 在线玩家
	if (u)
	{
		return u->sendCmdToScene(cmd, cmdlen);
	}

	// 离线玩家
	CharBase charbase;
	if (!RedisMgr::getMe().get_charbase(charid, charbase))
	{
		Fir::logger->info("[GM通知玩家]失败，获取离线玩家数据失败，角色ID = %u", charid);
		return false;
	}
	//查找最小负载服务器
	SessionTask* scene = SessionTaskManager::getMe().getMinLoad();
	if (!scene)
	{
		Fir::logger->error("[GM通知玩家]失败，获取场景失败，角色ID = %u", charid);
		return false;
	}

	u = SessionUserManager::getMe().regGMUser(charbase.accountid, charbase.charid, scene);
	if (NULL != u)
	{
		u->sendCmdToScene(cmd, cmdlen);
		SessionUserManager::getMe().unregGMUser(u);
	}
	else
	{
		Fir::logger->error("[GM通知玩家]失败，注册GM场景玩家失败，角色ID = %u", charid);
		return false;
	}

	return true;
}