//This file is auto generated. DO NOT MODIFY IT!

#ifndef _HMessages_h_
#define _HMessages_h_

#include <CommonStruct.h>
#include <BasePacket.h>
namespace FIGHTING
{

////////////////////////////////////////////////////////
//
const unsigned short ID_Ci2Ls_MIN_SCENE_CMD_ID=2001;
//
const unsigned short ID_Ci2Ls_FLServerVersionId=2002;
//
const unsigned short ID_Ci2Ls_LoginFLServer=2003;
//
const unsigned short ID_Ls2Ci_LoginFLServer_Reply=2004;
//
const unsigned short ID_Ci2Gs_EnterGame=2005;
//
const unsigned short ID_Ci2Gs_PlayerLoginQuick=2006;
//
const unsigned short ID_Ci2Gs_PlayerLoginCommon=2007;
//
const unsigned short ID_Gs2Ci_PlayerLogout=2008;
//AgentServer tell LoginServer
const unsigned short ID_As2Ls_AgentServerConnLogin=2009;
//
const unsigned short ID_Ci2As_SendPlayerId2=2010;
//
const unsigned short ID_Ci2As_ReConnectAgentServer=2011;
//
const unsigned short ID_As2Ci_ServerNotifyClientReLogin=2012;
//
const unsigned short ID_As2Gs_AgentServerConnLobby=2013;
//
const unsigned short ID_Gs2Ds_LobbyServerConnDBServer=2014;
//
const unsigned short ID_Rs2Ds_RoomServerConnDBServer=2015;
//
const unsigned short ID_Rs2Gs_RoomServerConnLobby=2016;
//
const unsigned short ID_Ls2Wd_LoginServerConnWorld=2017;
//
const unsigned short ID_Gs2As_ServerToServer=2018;
//
const unsigned short ID_Gs2Ci_ServerToClient=2019;
//
const unsigned short ID_Ci2Gs_ClientToServer=2020;
//client Tcp keep Heart
const unsigned short ID_Ci2As_ClientTcpKeepHeart=2021;
//use for platform
const unsigned short ID_Ci2Ls_PlayerLoginUsePlatformId=2022;
//use for iphone login
const unsigned short ID_Ci2Ls_PlayerLoginUseMacAddr=2023;
//
const unsigned short ID_Ls2Ci_PlayerLoginQuick_Reply=2024;
//
const unsigned short ID_Ls2Ci_PlayerLoginCommon_Reply=2025;
//
const unsigned short ID_Ci2Ls_REGISTER_ACCOUNT=2026;
//use for bot
const unsigned short ID_Ci2Ls_REGISTER_ACCOUNT_Reply=2027;
//Player s Account has been Login at other place
const unsigned short ID_As2Ci_PlayerAccountOtherPlaceLogin=2028;
//
const unsigned short ID_Gs2Ci_SendPlayerData2Ci=2029;
//
const unsigned short ID_Gs2Ci_SendPlayerItems2Ci=2030;
//
const unsigned short ID_Ci2Ls_RequestCreateRole=2031;
//
const unsigned short ID_Ls2Ci_RequestCreateRole_Reply=2032;
//
const unsigned short ID_Ci2Ls_RequestRemoveRole=2033;
//
const unsigned short ID_Ls2Ci_RequestRemoveRole_Reply=2034;
//
const unsigned short ID_Ci2Gs_PassLevelGetReward=2035;
//
const unsigned short ID_Gs2Ci_PassLevelGetReward_Reply=2036;
//
const unsigned short ID_Ci2Gs_RequestPartEnhance=2037;
//
const unsigned short ID_Gs2Ci_RequestPartEnhance_Replay=2038;
//
const unsigned short ID_Ci2Gs_RequestChangeEquip=2039;
//
const unsigned short ID_Gs2Ci_RequestChangeEquip_Replay=2040;
//
const unsigned short ID_Ci2Gs_EquipInlayGem=2041;
//
const unsigned short ID_Gs2Ci_EquipInlayGem_Reply=2042;
//
const unsigned short ID_Ci2Gs_UnloadInlayGem=2043;
//
const unsigned short ID_Gs2Ci_UnloadInlayGem_Reply=2044;
//skill position is 0~2; is on true is equip
const unsigned short ID_Ci2Gs_ChangeCurrentSkill=2045;
//reply when appear error
const unsigned short ID_Gs2Ci_ChangeCurrentSkill_Reply=2046;
//
const unsigned short ID_Ci2Gs_RequestUpgradeSkill=2047;
//
const unsigned short ID_Gs2Ci_RequestUpgradeSkill_Reply=2048;
//
const unsigned short ID_Ci2Gs_RequestStudySkill=2049;
//
const unsigned short ID_Gs2Ci_RequestStudySkill_Reply=2050;
//Enhance Type: 1 attribute type is exist. 2 attribute type is not exist
const unsigned short ID_Ci2Gs_RequestEquipEnhance=2051;
//
const unsigned short ID_Gs2Ci_RequestEquipEnhance_Reply=2052;
//
const unsigned short ID_Gs2Ci_LobbyRefreshDailyInfo=2053;
//
const unsigned short ID_Gs2Ci_LobbySyncDailyInfo=2054;
//
const unsigned short ID_Ci2Gs_RequestGetChapterBox=2055;
//
const unsigned short ID_Gs2Ci_RequestGetChapterBox_Reply=2056;
//tell player level upgrade!
const unsigned short ID_Gs2Ci_TellPlayerLevelUpgrade=2057;
//tell client error information!
const unsigned short ID_Gs2Ci_TellClientErrorInformation=2058;
//can not compound multi item
const unsigned short ID_Ci2Gs_RequestCompoundItem=2059;
//
const unsigned short ID_Gs2Ci_RequestCompoundItem_Reply=2060;
//
const unsigned short ID_Ci2Gs_RequestUpgradeEquip=2061;
//
const unsigned short ID_Gs2Ci_RequestUpgradeEquip_Reply=2062;
//
const unsigned short ID_Ci2Gs_PassCutSceneGetReward=2063;
//
const unsigned short ID_Gs2Ci_PassCutSceneGetReward_Reply=2064;
//
const unsigned short ID_Ci2Gs_RequestSellMaterial=2065;
//
const unsigned short ID_Gs2Ci_RequestSellMaterial_Reply=2066;
//
const unsigned short ID_Ci2Gs_RequestDecomposeEquip=2067;
//
const unsigned short ID_Gs2Ci_RequestDecomposeEquip_Reply=2068;
//
const unsigned short ID_Ci2Gs_RequestEnergyRestore=2069;
//
const unsigned short ID_Gs2Ci_RequestEnergyRestore_Reply=2070;
//
const unsigned short ID_Ci2Gs_RequestPauseOrRestore=2071;
//
const unsigned short ID_Ci2Gs_RequestUseMaterial=2072;
//
const unsigned short ID_Gs2Ci_RequestUseMaterial_Reply=2073;
//
const unsigned short ID_Ci2Gs_RequestUseSaoDangJuan=2074;
//
const unsigned short ID_Gs2Ci_RequestUseSaoDangJuan_Reply=2075;
//
const unsigned short ID_Ci2Gs_CompoundEquipGem=2076;
//
const unsigned short ID_Gs2Ci_CompoundEquipGem_Reply=2077;
//
const unsigned short ID_Ci2Gs_ResetFightingTimes=2078;
//Client record reset times
const unsigned short ID_Gs2Ci_ResetFightingTimes_Reply=2079;
//
const unsigned short ID_Ci2Gs_RequestDirectBuyEnergy=2080;
//Buy the energy
const unsigned short ID_Gs2Ci_RequestDirectBuyEnergy_Reply=2081;
//
const unsigned short ID_Ci2Gs_RequestGetSaoDangJuan=2082;
//
const unsigned short ID_Gs2Ci_RequestGetSaoDangJuan_Reply=2083;
//
const unsigned short ID_Ci2Gs_RequestDirectBuyGold=2084;
//Buy the energy
const unsigned short ID_Gs2Ci_RequestDirectBuyGold_Reply=2085;
//
const unsigned short ID_Ci2Gs_RequestRandomSymbol=2086;
//
const unsigned short ID_Gs2Ci_RequestRandomSymbol_Reply=2087;
//use gold symbol
const unsigned short ID_Ci2Gs_RequestUseGoldSymbol=2088;
//
const unsigned short ID_Gs2Ci_RequestUseGoldSymbol_Reply=2089;
//eat symbol
const unsigned short ID_Ci2Gs_RequestEatSymbol=2090;
//
const unsigned short ID_Gs2Ci_RequestEatSymbol_Reply=2091;
//eat symbol
const unsigned short ID_Ci2Gs_CompoundSymbolOneKey=2092;
//
const unsigned short ID_Gs2Ci_CompoundSymbolOneKey_Reply=2093;
//
const unsigned short ID_Ci2Gs_TidySymbolOneKey=2094;
//
const unsigned short ID_Gs2Ci_TidySymbolOneKey_Reply=2095;
//
const unsigned short ID_Ci2Gs_RequestEquipSymbol=2096;
//
const unsigned short ID_Gs2Ci_RequestEquipSymbol_Reply=2097;
//
const unsigned short ID_Ci2Gs_RequestChatMessage=2098;
//success not send message
const unsigned short ID_Gs2Ci_RequestChatMessage_Reply=2099;
//
const unsigned short ID_Gs2Ci_ReceiveChatMessage=2100;
//mail list
const unsigned short ID_Ci2Gs_PlayerQueryMailList=2101;
//mail list
const unsigned short ID_Gs2Ci_PlayerQueryMailList_Reply=2102;
//system mail on player on line
const unsigned short ID_Gs2Ci_PlayerReceiveMail=2103;
//player read mail
const unsigned short ID_Ci2Gs_PlayerReadMail=2104;
//when occurrence error
const unsigned short ID_Gs2Ci_PlayerReadMail_Reply=2105;
//player get item on mail
const unsigned short ID_Ci2Gs_PlayerGetGiftOnMail=2106;
//
const unsigned short ID_Gs2Ci_PlayerGetGiftOnMail_Reply=2107;
//player enter level
const unsigned short ID_Ci2Gs_PlayerEnterLevel=2108;
//
const unsigned short ID_Gs2Ci_PlayerEnterLevel_Reply=2109;
//buy shop item
const unsigned short ID_Ci2Gs_RequestBuyShopItem=2110;
//
const unsigned short ID_Gs2Ci_RequestBuyShopItem_Reply=2111;
//
const unsigned short ID_Ci2Gs_RequestRandomOpponents=2112;
//for add player
const unsigned short ID_Gs2Ci_RequestRandomOpponents_Reply=2113;
//
const unsigned short ID_Ci2Gs_RandomOpponentForMyList=2114;
//for old player
const unsigned short ID_Gs2Ci_RandomOpponentForMyList_Reply=2115;
//the player logout who is in your lobby
const unsigned short ID_Gs2Ci_ILookPlayerLogout=2116;
//
const unsigned short ID_Ci2Gs_PlayerMoveNotifyServer=2117;
//
const unsigned short ID_Gs2Ci_PlayerMoveSynchronize=2118;
//when players count is less 20, new enter player
const unsigned short ID_Gs2Ci_RandomCurrentEnterPlayer=2119;
//player enter lobby
const unsigned short ID_Ci2Gs_PlayerReturnLobby=2120;
//player lottery
const unsigned short ID_Ci2Gs_RequestLottery=2121;
//
const unsigned short ID_Gs2Ci_RequestLottery_Reply=2122;
//player gamble
const unsigned short ID_Ci2Gs_RequestOneGamble=2123;
//
const unsigned short ID_Gs2Ci_RequestOneGamble_Reply=2124;
//player gamble
const unsigned short ID_Ci2Gs_RequestTenGamble=2125;
//
const unsigned short ID_Gs2Ci_RequestTenGamble_Reply=2126;
//level rewards
const unsigned short ID_Ci2Gs_RequestGetLevelRewards=2127;
//
const unsigned short ID_Gs2Ci_RequestGetLevelRewards_Reply=2128;
//login rewards
const unsigned short ID_Ci2Gs_RequestGetLoginRewards=2129;
//
const unsigned short ID_Gs2Ci_RequestGetLoginRewards_Reply=2130;
//pay rewards
const unsigned short ID_Ci2Gs_RequestGetPayRewards=2131;
//
const unsigned short ID_Gs2Ci_RequestGetPayRewards_Reply=2132;
//query other player fight attribute
const unsigned short ID_Ci2Gs_RequestQueryPlayerFightAttr=2133;
//
const unsigned short ID_Gs2Ci_RequestQueryPlayerFightAttr_Reply=2134;
//update player self fight attribute
const unsigned short ID_Ci2Gs_RequestUpdatePlayerFightAttr=2135;
//add friend
const unsigned short ID_Ci2Gs_PlayerAddFriend=2136;
//
const unsigned short ID_Gs2Ci_PlayerAddFriend_Reply=2137;
//delete friend
const unsigned short ID_Ci2Gs_PlayerDelFriend=2138;
//query friend info
const unsigned short ID_Ci2Gs_PlayerQueryFriendInfo=2139;
//
const unsigned short ID_Gs2Ci_PlayerQueryFriendInfo_Reply=2140;
//query friends data
const unsigned short ID_Ci2Gs_PlayerQueryFriendsData=2141;
//
const unsigned short ID_Gs2Ci_PlayerQueryFriendsData_Reply=2142;
//
const unsigned short ID_Gs2Ci_SendFriendsID2Ci=2143;
//query player info by name
const unsigned short ID_Ci2Gs_QueryPlayerInfoByName=2144;
//
const unsigned short ID_Gs2Ci_QueryPlayerInfoByName_Reply=2145;
//request recommend friend
const unsigned short ID_Ci2Gs_RequestRecommendFriend=2146;
//
const unsigned short ID_Gs2Ci_RequestRecommendFriend_Reply=2147;
//request present energy
const unsigned short ID_Ci2Gs_RequestPresentEnergy=2148;
//
const unsigned short ID_Gs2Ci_RequestPresentEnergy_Reply=2149;
//
const unsigned short ID_Gs2Ci_NoticeAddRecieveEnergy_Reply=2150;
//request recieve energy
const unsigned short ID_Ci2Gs_RequestRecieveEnergy=2151;
//
const unsigned short ID_Gs2Ci_RequestRecieveEnergy_Reply=2152;
//request recieve energy list
const unsigned short ID_Ci2Gs_RequestRecieveEnergyList=2153;
//
const unsigned short ID_Gs2Ci_RequestRecieveEnergyList_Reply=2154;
//request recieve energy list
const unsigned short ID_Ci2Gs_RequestPlayerEnergyInfo=2155;
//
const unsigned short ID_Gs2Ci_RequestPlayerEnergyInfo_Reply=2156;
//request add blacks
const unsigned short ID_Ci2Gs_PlayerAddBlack=2157;
//
const unsigned short ID_Gs2Ci_PlayerAddBlack_Reply=2158;
//request add blacks
const unsigned short ID_Ci2Gs_PlayerQueryBlackInfo=2159;
//
const unsigned short ID_Gs2Ci_PlayerQueryBlackInfo_Reply=2160;
//
const unsigned short ID_Gs2Ci_ServerNotityActiveStart=2161;
//
const unsigned short ID_Gs2Ci_ServerNotityActiveEnd=2162;
//
const unsigned short ID_Ci2Gs_RequestEnterActivityBossFight=2163;
//
const unsigned short ID_Gs2Ci_RequestEnterActivityBossFight_Reply=2164;
//
const unsigned short ID_Ci2Gs_RequestExitActivityBossFight=2165;
//
const unsigned short ID_Gs2Ci_ServerNotityBossFightEnd=2166;
//
const unsigned short ID_Ci2Gs_RequestSubmitBossFightDamage=2167;
//
const unsigned short ID_Ci2Gs_BossFightPlayerDie=2168;
//
const unsigned short ID_Gs2Ci_BossFightPlayerDie_Reply=2169;
//
const unsigned short ID_Gs2Ci_SynchronizeBossFightBossBlood=2170;
//
const unsigned short ID_Ci2Gs_RequestQueryBossFightRank=2171;
//
const unsigned short ID_Gs2Ci_RequestQueryBossFightRank_Reply=2172;
//
const unsigned short ID_Ci2Gs_RequestGetRankRewardsBossFight=2173;
//
const unsigned short ID_Gs2Ci_RequestGetRankRewardsBossFight_Reply=2174;
//
const unsigned short ID_Ci2Gs_RequestGetTargetRewardsBossFight=2175;
//
const unsigned short ID_Gs2Ci_RequestGetTargetRewardsBossFight_Reply=2176;
//
const unsigned short ID_Ci2Gs_RequestBuyTimesBossFight=2177;
//after buy success, direct enter active
const unsigned short ID_Gs2Ci_RequestBuyTimesBossFight_Reply=2178;
//
const unsigned short ID_Ci2Gs_RequestQueryActiveIsPlaying=2179;
//after buy success, direct enter active
const unsigned short ID_Gs2Ci_RequestQueryActiveIsPlaying_Reply=2180;
//server notify error
const unsigned short ID_Gs2Ci_ServerNotifyError=2181;
//server notify your friend Enter game
const unsigned short ID_Gs2Ci_ServerNotifyFriendEnter=2182;
//can not self query self
const unsigned short ID_Ci2Gs_PlayerQueryOtherInfo=2183;
//
const unsigned short ID_Gs2Ci_PlayerQueryOtherInfo_Reply=2184;
//
const unsigned short ID_Gs2Ci_ServerNotifyPvpRankOpen=2185;
//PVP rank active
const unsigned short ID_Ci2Gs_RequestPvpRandomInfo=2186;
//
const unsigned short ID_Gs2Ci_RequestPvpRandomInfo_Reply=2187;
//day task reward
const unsigned short ID_Ci2Gs_RequestGetDayTaskReward=2188;
//
const unsigned short ID_Gs2Ci_RequestGetDayTaskReward_Reply=2189;
//day task reward
const unsigned short ID_Ci2Gs_RequestPlayerDayTask=2190;
//
const unsigned short ID_Gs2Ci_RequestPlayerDayTask_Reply=2191;
//
const unsigned short ID_Gs2Ci_ModifyPlayerDayTask2Ci=2192;
//server timer refresh
const unsigned short ID_Gs2Ci_ServerNotifyPvpRankRefreshing=2193;
//PVP rank active
const unsigned short ID_Ci2Gs_RequestPvpRankChallengeOpponent=2194;
//
const unsigned short ID_Gs2Ci_RequestPvpRankChallengeOpponent_Reply=2195;
//PVP rank active
const unsigned short ID_Ci2Gs_PvpRankFightResult=2196;
//
const unsigned short ID_Gs2Ci_PvpRankFightResult_Reply=2197;
//the fight record
const unsigned short ID_Gs2Ci_ServerNotifyPvpRankAddInfo=2198;
//when player in top100 change
const unsigned short ID_Gs2Ci_NotifyPvpRankChangeRank=2199;
//PVP rank active, get feat 30 minutes one time
const unsigned short ID_Ci2Gs_PvpRankTimerGetFeat=2200;
//
const unsigned short ID_Gs2Ci_PvpRankTimerGetFeat_Reply=2201;
//buy challenge times
const unsigned short ID_Ci2Gs_PvpRankBuyChallengeTimes=2202;
//
const unsigned short ID_Gs2Ci_PvpRankBuyChallengeTimes_Reply=2203;
//
const unsigned short ID_Ci2Gs_PvpRankCleanChallengeCD=2204;
//
const unsigned short ID_Gs2Ci_PvpRankCleanChallengeCD_Reply=2205;
//Challenge CD time out of date
const unsigned short ID_Ci2Gs_PvpRankChallengeCDOutOfDate=2206;
//
const unsigned short ID_Ci2Gs_PvpRankRequestQueryTop100=2207;
//
const unsigned short ID_Gs2Ci_PvpRankRequestQueryTop100_Reply=2208;
//
const unsigned short ID_Ci2Gs_PvpRankRequestPlayerInfo=2209;
//
const unsigned short ID_Gs2Ci_PvpRankRequestPlayerInfo_Reply=2210;
//
const unsigned short ID_Ci2Gs_RequestPayShopData=2211;
//
const unsigned short ID_Gs2Ci_RequestPayShopData_Reply=2212;
//
const unsigned short ID_Ci2Gs_RequestPayMoneyByIndex=2213;
//
const unsigned short ID_Gs2Ci_RequestPayMoneyByIndex_Reply=2214;
//
const unsigned short ID_Ci2Gs_RequestGetFirstPayReward=2215;
//
const unsigned short ID_Gs2Ci_RequestGetFirstPayReward_Reply=2216;
//
const unsigned short ID_Gs2Ci_TellPlayerVipLevelUpgrade=2217;
//
const unsigned short ID_Ci2Gs_PvpRankRequestTradeFeat=2218;
//
const unsigned short ID_Gs2Ci_PvpRankRequestTradeFeat_Reply=2219;
//
const unsigned short ID_Ci2Gs_PvpRankRefreshRandomOpponent=2220;
//
const unsigned short ID_Gs2Ci_PvpRankRefreshRandomOpponent_Reply=2221;
//
const unsigned short ID_Ci2Gs_PvpRankRequestGetFeat=2222;
//
const unsigned short ID_Gs2Ci_PvpRankRequestGetFeat_Reply=2223;
//
const unsigned short ID_Ci2Gs_RequestEnterActivityGoldLevel=2224;
//
const unsigned short ID_Gs2Ci_RequestEnterActivityGoldLevel_Reply=2225;
//
const unsigned short ID_Ci2Gs_RequestBuyGoldLevelTimes=2226;
//
const unsigned short ID_Gs2Ci_RequestBuyGoldLevelTimes_Reply=2227;
//
const unsigned short ID_Ci2Gs_RequestGetGoldLevelRewards=2228;
//
const unsigned short ID_Gs2Ci_RequestGetGoldLevelRewards_Reply=2229;
//
const unsigned short ID_Ci2Gs_RequestEnterActivityExpLevel=2230;
//
const unsigned short ID_Gs2Ci_RequestEnterActivityExpLevel_Reply=2231;
//
const unsigned short ID_Ci2Gs_RequestBuyExpLevelTimes=2232;
//
const unsigned short ID_Gs2Ci_RequestBuyExpLevelTimes_Reply=2233;
//
const unsigned short ID_Ci2Gs_RequestGetExpLevelRewards=2234;
//
const unsigned short ID_Gs2Ci_RequestGetExpLevelRewards_Reply=2235;
//
const unsigned short ID_Gs2Ci_ServerNotityAddMarqueeInfo=2236;
//
const unsigned short ID_Ci2Gs_RequestExchangeShopItem=2237;
//
const unsigned short ID_Gs2Ci_RequestExchangeShopItem_Reply=2238;
//
const unsigned short ID_Ci2Gs_RequestPreRemoveEquipEnhance=2239;
//
const unsigned short ID_Gs2Ci_RequestPreRemoveEquipEnhance_Reply=2240;
//
const unsigned short ID_Ci2Gs_RequestEquipSkillId=2241;
//
const unsigned short ID_Gs2Ci_RequestEquipSkillId_Reply=2242;
//
const unsigned short ID_Ci2Gs_RequestSymbolEnhance=2243;
//
const unsigned short ID_Gs2Ci_RequestSymbolEnhance_Reply=2244;
//
const unsigned short ID_Ci2Gs_RequestOneKeySymbolRandom=2245;
//
const unsigned short ID_Gs2Ci_RequestOneKeySymbolRandom_Reply=2246;
//
const unsigned short ID_Ci2Gs_RequestUpgradeSymbol=2247;
//
const unsigned short ID_Gs2Ci_RequestUpgradeSymbol_Reply=2248;
//
const unsigned short ID_Ci2Gs_RequestQuerySymbol=2249;
//
const unsigned short ID_Gs2Ci_RequestQuerySymbol_Reply=2250;
//
const unsigned short ID_Ci2Gs_RequestQueryActiveEverBattle=2251;
//
const unsigned short ID_Gs2Ci_RequestQueryActiveEverBattle_Reply=2252;
//
const unsigned short ID_Ci2Gs_RequestEnterActivityEverBattle=2253;
//
const unsigned short ID_Gs2Ci_RequestEnterActivityEverBattle_Reply=2254;
//
const unsigned short ID_Ci2Gs_RequestBuyEverBattleTimes=2255;
//
const unsigned short ID_Gs2Ci_RequestBuyEverBattleTimes_Reply=2256;
//
const unsigned short ID_Ci2Gs_RequestGetEverBattleRewards=2257;
//
const unsigned short ID_Gs2Ci_RequestGetEverBattleRewards_Reply=2258;
//
const unsigned short ID_Ci2Gs_RequestContinueEnterEverBattle=2259;
//
const unsigned short ID_Gs2Ci_RequestContinueEnterEverBattle_Reply=2260;
//
const unsigned short ID_Ci2Gs_RequestSaoDangEverBattle=2261;
//
const unsigned short ID_Gs2Ci_RequestSaoDangEverBattle_Reply=2262;
//
const unsigned short ID_Ci2Gs_RequestResetEverBattle=2263;
//
const unsigned short ID_Gs2Ci_RequestResetEverBattle_Reply=2264;
//
const unsigned short ID_Ci2Gs_RequestGetBattleRankRewards=2265;
//
const unsigned short ID_Gs2Ci_RequestGetBattleRankRewards_Reply=2266;
//
const unsigned short ID_Ci2Gs_RequestGetBattleStageRewards=2267;
//
const unsigned short ID_Gs2Ci_RequestGetBattleStageRewards_Reply=2268;
//
const unsigned short ID_Ci2Gs_RequestQueryEverBattleTop100=2269;
//
const unsigned short ID_Gs2Ci_RequestEverBattleTop100_Reply=2270;
//
const unsigned short ID_Gs2Ci_ServerNotifyBattleRankRefreshing=2271;
//
const unsigned short ID_Ci2Gs_RequestExchangeGiftItem=2272;
//
const unsigned short ID_Gs2Ci_RequestExchangeGiftItem_Reply=2273;
//
const unsigned short ID_Ci2Gs_RequestPlayerUpdateGuideId=2274;
//
const unsigned short ID_Gs2Ci_RequestPlayerUpdateGuideId_Reply=2275;
//
const unsigned short ID_Ci2Gs_RequestPlayerPracticeTime=2276;
//
const unsigned short ID_Gs2Ci_RequestPlayerPracticeTime_Reply=2277;
//
const unsigned short ID_Ci2Gs_RequestTakePracticeExp=2278;
//
const unsigned short ID_Gs2Ci_RequestTakePracticeExp_Reply=2279;
//
const unsigned short ID_Ci2Gs_RequestGetActivenessRewards=2280;
//
const unsigned short ID_Gs2Ci_RequestGetActivenessRewards_Reply=2281;
//
const unsigned short ID_Ci2Gs_RequestContinueLoginRewards=2282;
//
const unsigned short ID_Gs2Ci_RequestContinueLoginRewards_Reply=2283;
//
const unsigned short ID_Ci2Gs_RequesQueryTreasureLevel=2284;
//
const unsigned short ID_Gs2Ci_RequestQueryTreasureLevel_Reply=2285;
//
const unsigned short ID_Ci2Gs_RequestPlayerGetTreasure=2286;
//
const unsigned short ID_Gs2Ci_RequestPlayerGetTreasure_Reply=2287;
//
const unsigned short ID_Ci2Gs_RequestPlayerGrabTreasure=2288;
//
const unsigned short ID_Gs2Ci_RequestPlayerGrabTreasure_Reply=2289;
//
const unsigned short ID_Ci2Gs_RequestPlayerQueryTreasure=2290;
//
const unsigned short ID_Gs2Ci_RequestPlayerQueryTreasure_Reply=2291;
//
const unsigned short ID_Ci2Gs_RequestPlayerChangeTreasure=2292;
//
const unsigned short ID_Gs2Ci_RequestPlayerChangeTreasure_Reply=2293;
//
const unsigned short ID_Ci2Gs_RequestPlayerShowTreasureInfo=2294;
//
const unsigned short ID_Gs2Ci_RequestPlayerShowTreasureInfo_Reply=2295;
//
const unsigned short ID_Ci2Gs_RequestUseTreasureItem=2296;
//
const unsigned short ID_Gs2Ci_RequestUseTreasureItem_Reply=2297;
//
const unsigned short ID_Ci2Gs_RequestQueryBountyTask=2298;
//
const unsigned short ID_Gs2Ci_RequestQueryBountyTask_Reply=2299;
//
const unsigned short ID_Ci2Gs_RequestRefreshBountyTask=2300;
//
const unsigned short ID_Gs2Ci_RequestRefreshBountyTask_Reply=2301;
//
const unsigned short ID_Ci2Gs_RequestEnterBountyTask=2302;
//
const unsigned short ID_Gs2Ci_RequestEnterBountyTask_Reply=2303;
//
const unsigned short ID_Ci2Gs_RequestGetBountyTaskRewards=2304;
//
const unsigned short ID_Gs2Ci_RequestGetBountyTaskRewards_Reply=2305;
//
const unsigned short ID_Ci2Gs_RequestPlayerRank=2306;
//
const unsigned short ID_Gs2Ci_RequestPlayerRank_Reply=2307;
//
const unsigned short ID_Ci2Gs_RefreshMysticalShopItem=2308;
//
const unsigned short ID_Gs2Ci_RefreshMysticalShopItem_Reply=2309;
//
const unsigned short ID_Ci2Gs_BuyMysticalShopItem=2310;
//
const unsigned short ID_Gs2Ci_BuyMysticalShopItem_Reply=2311;
//
const unsigned short ID_Ci2Gs_RequestUpgradePlayerSpLevel=2312;
//
const unsigned short ID_Gs2Ci_RequestUpgradePlayerSpLevel_Reply=2313;
//
const unsigned short ID_Ci2Gs_RequestPlayerGetAchieveRewards=2314;
//
const unsigned short ID_Gs2Ci_RequestPlayerGetAchieveRewards_Reply=2315;
//
const unsigned short ID_Ci2Gs_RequestGetAchieveRewardsBox=2316;
//
const unsigned short ID_Gs2Ci_RequestGetAchieveRewardsBox_Reply=2317;
//
const unsigned short ID_Ci2Gs_RequestQueryPlayerAchieve=2318;
//
const unsigned short ID_Gs2Ci_RequestQueryPlayerAchieve_Reply=2319;
//player buy month card
const unsigned short ID_Ci2Gs_RequestBuyMonthCard=2320;
//player buy month card reply
const unsigned short ID_Gs2Ci_RequestBuyMonthCard_Reply=2321;
//player buy month card
const unsigned short ID_Ci2Gs_RequestMonthCardReward=2322;
//player buy month card reply
const unsigned short ID_Gs2Ci_RequestMonthCardReward_Reply=2323;
//player del mail
const unsigned short ID_Ci2Gs_PlayerDelMail=2324;
//
const unsigned short ID_Ci2Gs_PlayerDelMail_Reply=2325;
//
const unsigned short ID_Gs2Ci_RequestBossFightDamSortInScene_Reply=2326;
//
const unsigned short ID_Gs2Ci_RequestBossFightHistoryDam=2327;
//
const unsigned short ID_Gs2Ci_RequestBossFightHistoryDam_Reply=2328;
//
const unsigned short ID_Ci2Gs_RequestPlayerPartUpgrade=2329;
//
const unsigned short ID_Gs2Ci_RequestPlayerPartUpgrade_Reply=2330;
//
const unsigned short ID_Ci2Gs_RequestEnterActivityBossLevel=2331;
//
const unsigned short ID_Gs2Ci_RequestEnterActivityBossLevel_Reply=2332;
//
const unsigned short ID_Ci2Gs_RequestBuyBossLevelTimes=2333;
//
const unsigned short ID_Gs2Ci_RequestBuyBossLevelTimes_Reply=2334;
//
const unsigned short ID_Ci2Gs_RequestGetBossLevelRewards=2335;
//
const unsigned short ID_Gs2Ci_RequestGetBossLevelRewards_Reply=2336;
//
const unsigned short ID_Ci2Gs_RequestInterShopItem=2337;
//
const unsigned short ID_Gs2Ci_RequestInterShopItem_Reply=2338;
//
const unsigned short ID_Ci2Gs_RequestBuyInterShop=2339;
//
const unsigned short ID_Gs2Ci_RequestBuyInterShop_Reply=2340;
//
const unsigned short ID_Ci2Gs_RequestPlayerSellGem=2341;
//
const unsigned short ID_Gs2Ci_RequestPlayerSellGem_Reply=2342;
//
const unsigned short ID_Ci2Gs_RequestBuyVipShopItem=2343;
//
const unsigned short ID_Gs2Ci_RequestBuyVipShopItem_Reply=2344;
//
const unsigned short ID_Ci2Gs_RequestBuyDiscountShopItem=2345;
//
const unsigned short ID_Gs2Ci_RequestBuyDiscountShopItem_Reply=2346;
//
const unsigned short ID_Ci2Gs_RequestDiscountShopItem=2347;
//
const unsigned short ID_Gs2Ci_RequestDiscountShopItem_Reply=2348;
//
const unsigned short ID_Ci2Gs_RequestPlayerEquipEhance=2349;
//
const unsigned short ID_Gs2Ci_RequestPlayerEquipEhance_Reply=2350;
//
const unsigned short ID_Ci2Gs_RequestPlayerAffirmEquipEhance=2351;
//
const unsigned short ID_Gs2Ci_RequestPlayerAffirmEquipEhance_Reply=2352;
//
const unsigned short ID_Gs2Ci_RequestGodWeapons=2353;
//
const unsigned short ID_Gs2Ci_RequestGodWeapons_Reply=2354;
//
const unsigned short ID_Gs2Ci_RequestGodWeaponActive=2355;
//
const unsigned short ID_Gs2Ci_RequestGodWeaponActive_Reply=2356;
//
const unsigned short ID_Gs2Ci_RequestGodWeaponUpRank=2357;
//
const unsigned short ID_Gs2Ci_RequestGodWeaponUpRank_Reply=2358;
//
const unsigned short ID_Gs2Ci_RequestGodWeaponAutoUpRank=2359;
//
const unsigned short ID_Gs2Ci_RequestGodWeaponWear=2360;
//
const unsigned short ID_Gs2Ci_RequestGodWeaponWear_Reply=2361;
//
const unsigned short ID_Gs2Ci_RequestGodWeaponAutoUpRank_Reply=2362;
//
const unsigned short ID_Gs2Ci_RequestPlayerAddMoney_Reply=2363;
//
const unsigned short ID_Ci2Gs_RequestPlayerAttrBossFight=2364;
//
const unsigned short ID_Gs2Ci_RequestPlayerAttrBossFight_Reply=2365;
//
const unsigned short ID_Ci2Gs_RequestPlayerCompleteTitle=2366;
//
const unsigned short ID_Gs2Ci_RequestPlayerCompleteTitle_Reply=2367;
//请求活动xml
const unsigned short ID_Ci2Gs_RequestActive_Xml_Update=2368;
//请求活动xml回应
const unsigned short ID_Gs2Ci_RequestActive_Xml_Update_Reply=2369;
//注册今日首充弹出次数
const unsigned short ID_Ci2Gs_RegisterTodayShowRecharge=2370;
//获取首充信息
const unsigned short ID_Ci2Gs_GetSelfFirstRechargeInfo=2371;
//获取首充信息回应
const unsigned short ID_Gs2Ci_GetSelfFirstRechargeInfo_Reply=2372;
//可以领取首充奖励
const unsigned short ID_Gs2Ci_CanTakeRirstRechargeReward_Reply=2373;
//领取首充奖励
const unsigned short ID_Ci2Gs_TakeRirstRechargeReward=2374;
//领取首充奖励回应
const unsigned short ID_Gs2Ci_TakeRirstRechargeReward_Reply=2375;
//
const unsigned short ID_Ci2Gs_RegisterLoseInfo=2376;
//
const unsigned short ID_Gs2Ci_ReConnectAgentServer_Reply=2377;
//
const unsigned short ID_Ci2Gs_RequestPlayerQueryTitle=2378;
//
const unsigned short ID_Gs2Ci_RequestPlayerQueryTitle_Reply=2379;
//
const unsigned short ID_Ci2Gs_RequestPlayerUseTitle=2380;
//
const unsigned short ID_Gs2Ci_RequestPlayerUseTitle_Reply=2381;
//
const unsigned short ID_Gs2Ci_RequestLookPlayerUseTitle_Reply=2382;
//获取某个消耗活动信息
const unsigned short ID_Ci2Gs_GetActivityConsumeInfo=2383;
//获取某个消耗活动信息回应
const unsigned short ID_Gs2Ci_GetActivityConsumeInfo_Reply=2384;
//领取消耗类奖励
const unsigned short ID_Ci2Gs_TakeActivityConsumeReward=2385;
//领取消耗类奖励回应
const unsigned short ID_Gs2Ci_TakeActivityConsumeReward_Reply=2386;
//红包开抢通知
const unsigned short ID_Gs2Ci_RedEnvelopeComming_Reply=2387;
//抢红包
const unsigned short ID_Ci2Gs_RequestSnatchEnvelope=2388;
//抢红包回应
const unsigned short ID_Gs2Ci_RequestSnatchEnvelope_Reply=2389;
//
const unsigned short ID_Ci2Gs_ReqRechargeInnerOrderid=2390;
//
const unsigned short ID_Gs2Ci_RetRechargeInnerOrderid_Reply=2391;
//发送挑战请求
const unsigned short ID_Ci2Gs_RequestFriendPvpChallenge=2392;
//发送挑战请求错误回复
const unsigned short ID_Gs2Ci_RequestFriendPvpChallenge_Reply=2393;
//转发玩家的挑战请求
const unsigned short ID_Gs2Ci_RequestFriendPvpChallengeSync=2394;
//回复挑战请求
const unsigned short ID_Ci2Gs_RequestFriendPvpChallenge_Reply=2395;
//转发玩家的回复
const unsigned short ID_Gs2Ci_RequestFriendPvpChallengeSync_Reply=2396;
//通知进入战斗
const unsigned short ID_Gs2Ci_RequestFriendPvpEnterFighting=2397;
//扣除对手血量
const unsigned short ID_Ci2Gs_RequestFriendPvpReduceBlood=2398;
//服务器转发扣血消息
const unsigned short ID_Gs2Ci_RequestFriendPvpReduceBloodSync=2399;
//结算消息
const unsigned short ID_Gs2Ci_RequestFriendPvpFightResult=2400;
//其他战斗信息
const unsigned short ID_Ci2Gs_RequestFriendPvpFightInfo=2401;
//服务器转发其他战斗信息
const unsigned short ID_Gs2Ci_RequestFriendPvpFightInfoSync=2402;
//
const unsigned short ID_Ci2Gs_RequestQueryDrinkWine=2403;
//
const unsigned short ID_Gs2Ci_RequestQueryDrinkWine_Reply=2404;
//
const unsigned short ID_Ci2Gs_RequestCreateDrinkWine=2405;
//
const unsigned short ID_Gs2Ci_RequestCreateDrinkWine_Reply=2406;
//
const unsigned short ID_Ci2Gs_RequestInviteDrinkWine=2407;
//
const unsigned short ID_Gs2Ci_RequestInviteDrinkWine_Reply=2408;
//
const unsigned short ID_Ci2Gs_RequestEnterDrinkWine=2409;
//
const unsigned short ID_Gs2Ci_RequestEnterDrinkWine_Reply=2410;
//
const unsigned short ID_Ci2Gs_RequestExitDrinkWine=2411;
//
const unsigned short ID_Gs2Ci_RequestExitDrinkWine_Reply=2412;
//
const unsigned short ID_Ci2Gs_RequestGetDrinkWineRewards=2413;
//
const unsigned short ID_Gs2Ci_RequestGetDrinkWineRewards_Reply=2414;
//
const unsigned short ID_Ci2Gs_RequestOpenDrinkWine=2415;
//
const unsigned short ID_Gs2Ci_RequestOpenDrinkWine_Reply=2416;
//获取条件类活动信息
const unsigned short ID_Ci2Gs_RequestActivityRequireInfo=2417;
//获取条件类活动信息回应
const unsigned short ID_Gs2Ci_RequestActivityRequireInfo_Reply=2418;
//领取条件类活动奖励
const unsigned short ID_Ci2Gs_TakeActivityRequireReward=2419;
//领取条件类活动奖励回应
const unsigned short ID_Gs2Ci_TakeActivityRequireReward_Reply=2420;
//查询排行榜玩家信息
const unsigned short ID_Ci2Gs_QueryPlayerRankInfo=2421;
//查询排行榜玩家信息返回
const unsigned short ID_Gs2Ci_QueryPlayerRankInfo_Reply=2422;
//请求抽卡
const unsigned short ID_Ci2Gs_RequestDrawCard=2423;
//请求抽卡回应
const unsigned short ID_Gs2Ci_RequestDrawCard_Reply=2424;
//登录游戏失败(账号被封之类的)
const unsigned short ID_Gs2Ci_EnterGameFailed_Reply=2425;
//购买宝藏争夺次数
const unsigned short ID_Ci2Gs_PlayerBuyTreasureTimes=2426;
//购买宝藏争夺次数返回
const unsigned short ID_Gs2Ci_PlayerBuyTreasureTimes_Reply=2427;
//
const unsigned short ID_Ci2Gs_RequestOneKeyTidySymbol=2428;
//
const unsigned short ID_Gs2Ci_RequestOneKeyTidySymbol_Reply=2429;
//
const unsigned short ID_Ci2Gs_RequestOneKeyGetSymbol=2430;
//
const unsigned short ID_Gs2Ci_RequestOneKeyGetSymbol_Reply=2431;
//
const unsigned short ID_Ci2Gs_RequestBountyTaskUpStar=2432;
//
const unsigned short ID_Gs2Ci_RequestBountyTaskUpStar_Reply=2433;
//
const unsigned short ID_Ci2Gs_RequestCompleteAllBountyTask=2434;
//
const unsigned short ID_Gs2Ci_RequestCompleteAllBountyTask_Reply=2435;
//请求玩家金币副本信息
const unsigned short ID_Ci2Gs_RequestPlayerGoldLevel=2436;
//请求玩家金币副本信息回应
const unsigned short ID_Gs2Ci_RequestPlayerGoldLevel_Reply=2437;
//请求领取金币副本宝箱
const unsigned short ID_Ci2Gs_RequestGetGoldLevelBox=2438;
//请求领取金币副本宝箱回应
const unsigned short ID_Gs2Ci_RequestGetGoldLevelBox_Reply=2439;
//请求玩家经验副本信息
const unsigned short ID_Ci2Gs_RequestPlayerExpLevel=2440;
//请求玩家经验副本信息回应
const unsigned short ID_Gs2Ci_RequestPlayerExpLevel_Reply=2441;
//请求领取经验副本宝箱
const unsigned short ID_Ci2Gs_RequestGetExpLevelBox=2442;
//请求领取经验副本宝箱回应
const unsigned short ID_Gs2Ci_RequestGetExpLevelBox_Reply=2443;
//请求玩家困难副本信息
const unsigned short ID_Ci2Gs_RequestPlayerBossLevel=2444;
//请求玩家困难副本信息回应
const unsigned short ID_Gs2Ci_RequestPlayerBossLevel_Reply=2445;
//请求玩家真气副本信息
const unsigned short ID_Ci2Gs_RequestPlayerSpExpLevel=2446;
//请求玩家真气副本信息回应
const unsigned short ID_Gs2Ci_RequestPlayerSpExpLevel_Reply=2447;
//请求领取真气副本奖励
const unsigned short ID_Ci2Gs_RequestGetSpExpLevelRewards=2448;
//请求领取真气副本奖励回应
const unsigned short ID_Gs2Ci_RequestGetSpExpLevelRewards_Reply=2449;
//请求玩家购买真气副本次数
const unsigned short ID_Ci2Gs_RequestBuySpExpLevelTimes=2450;
//请求玩家购买真气副本次数回应
const unsigned short ID_Gs2Ci_RequestBuySpExpLevelTimes_Reply=2451;
//请求玩家真气副本奖励
const unsigned short ID_Ci2Gs_RequestGetSpExpLevelBox=2452;
//请求玩家真气副本奖励回应
const unsigned short ID_Gs2Ci_RequestGetSpExpLevelBox_Reply=2453;
//
const unsigned short ID_Gs2Ci_SendAchieveTaskData=2454;
//
const unsigned short ID_Ci2Gs_RequestQueryAchieveTask=2455;
//
const unsigned short ID_Gs2Ci_RequestQueryAchieveTask_Reply=2456;
//
const unsigned short ID_Ci2Gs_RequestGetAchieveTaskRewards=2457;
//
const unsigned short ID_Gs2Ci_RequestGetAchieveTaskRewards_Reply=2458;
//
const unsigned short ID_Ci2Gs_RequestGeneralShopBuyTimes=2459;
//
const unsigned short ID_Gs2Ci_RequestGeneralShopBuyTimes_Reply=2460;
//
const unsigned short ID_Ci2Gs_RequestPlayerBuyItem=2461;
//
const unsigned short ID_Gs2Ci_RequestPlayerBuyItem_Reply=2462;
//
const unsigned short ID_Ci2Gs_RequestPlayerClothes=2463;
//
const unsigned short ID_Gs2Ci_RequestPlayerClothes_Reply=2464;
//
const unsigned short ID_Ci2Gs_RequestPlayerClothesLevel=2465;
//
const unsigned short ID_Gs2Ci_RequestPlayerClothesLevel_Reply=2466;
//
const unsigned short ID_Ci2Gs_RequestPlayerWearClothes=2467;
//
const unsigned short ID_Gs2Ci_RequestPlayerWearClothes_Reply=2468;
//
const unsigned short ID_Ci2Gs_RequestPlayerSubmitProblem=2469;
//
const unsigned short ID_Gs2Ci_RequestPlayerSubmitProblem_Reply=2470;
//一键镶嵌
const unsigned short ID_Ci2Gs_RequestOneKeyInlayGem=2471;
//一键镶嵌
const unsigned short ID_Gs2Ci_RequestOneKeyInlayGem_Reply=2472;
//抽奖数据
const unsigned short ID_Ci2Gs_RequestPlayerGambleData=2473;
//抽奖数据
const unsigned short ID_Gs2Ci_RequestPlayerGambleData_Reply=2474;
//抽奖
const unsigned short ID_Ci2Gs_RequestPlayerGamble=2475;
//抽奖
const unsigned short ID_Gs2Ci_RequestPlayerGamble_Reply=2476;
//同步数据
const unsigned short ID_Gs2Ci_SendChangeAchieveTask=2477;
//运营活动预览csv文件请求
const unsigned short ID_Ci2Gs_RequestOAPandectCsv=2478;
//运营活动预览csv文件返回
const unsigned short ID_Gs2Ci_RequestOAPandectCsv_Reply=2479;
//运营活动csv文件请求
const unsigned short ID_Ci2Gs_RequestOACsv=2480;
//运营活动csv文件返回
const unsigned short ID_Gs2Ci_RequestOACsv_Reply=2481;
//运营活动-首次充值进度请求
const unsigned short ID_Ci2Gs_RequestOAFirstPay=2482;
//运营活动-首次充值进度返回
const unsigned short ID_Gs2Ci_RequestOAFirstPay_Reply=2483;
//运营活动-每日累充进度请求
const unsigned short ID_Ci2Gs_RequestOADailyPay=2484;
//运营活动-每日累充进度返回
const unsigned short ID_Gs2Ci_RequestOADailyPay_Reply=2485;
//运营活动-多日累充进度请求
const unsigned short ID_Ci2Gs_RequestOADaysPay=2486;
//运营活动-多日累充进度返回
const unsigned short ID_Gs2Ci_RequestOADaysPay_Reply=2487;
//运营活动-累计充值进度请求
const unsigned short ID_Ci2Gs_RequestOATotalPay=2488;
//运营活动-累计充值进度返回
const unsigned short ID_Gs2Ci_RequestOATotalPay_Reply=2489;
//运营活动-累计消耗进度请求
const unsigned short ID_Ci2Gs_RequestOATotalConsume=2490;
//运营活动-累计消耗进度返回
const unsigned short ID_Gs2Ci_RequestOATotalConsume_Reply=2491;
//运营活动-连续充值进度请求
const unsigned short ID_Ci2Gs_RequestOASuccessivePay=2492;
//运营活动-连续充值进度返回
const unsigned short ID_Gs2Ci_RequestOASuccessivePay_Reply=2493;
//运营活动-连续消耗进度请求
const unsigned short ID_Ci2Gs_RequestOASuccessiveConsume=2494;
//运营活动-连续消耗进度返回
const unsigned short ID_Gs2Ci_RequestOASuccessiveConsume_Reply=2495;
//运营活动-在线进度请求
const unsigned short ID_Ci2Gs_RequestOAOnline=2496;
//运营活动-在线进度请求返回
const unsigned short ID_Gs2Ci_RequestOAOnline_Reply=2497;
//运营活动-购买基金请求
const unsigned short ID_Ci2Gs_RequestOABuyFund=2498;
//运营活动-购买基金请求返回
const unsigned short ID_Gs2Ci_RequestOABuyFund_Reply=2499;
//运营活动-基金进度请求
const unsigned short ID_Ci2Gs_RequestOAFund=2500;
//运营活动-基金进度请求返回
const unsigned short ID_Gs2Ci_RequestOAFund_Reply=2501;
//运营活动-前n次登录进度请求
const unsigned short ID_Ci2Gs_RequestOAEarlyLogin=2502;
//运营活动-前n次登录进度请求返回
const unsigned short ID_Gs2Ci_RequestOAEarlyLogin_Reply=2503;
//运营活动-领取奖励
const unsigned short ID_Ci2Gs_RequestTakeOAReward=2504;
//运营活动-领取奖励返回
const unsigned short ID_Gs2Ci_RequestTakeOAReward_Reply=2505;
//请求红包csv文件
const unsigned short ID_Ci2Gs_RequestRedEnvelopeCsv=2506;
//红包csv文件内容返回
const unsigned short ID_Gs2Ci_RequestRedEnvelopeCsv_Reply=2507;
//验证红包是否开启
const unsigned short ID_Ci2Gs_RequestVerifyRedEnvelope=2508;
//验证红包是否开启返回
const unsigned short ID_Gs2Ci_RequestVerifyRedEnvelope_Reply=2509;
//请求领取红包
const unsigned short ID_Ci2Gs_RequestOpenRedEnvelope=2510;
//领取红包返回
const unsigned short ID_Gs2Ci_RequestOpenRedEnvelope_Reply=2511;
//夺宝数据
const unsigned short ID_Ci2Gs_RequestPlayerSnatchInfo=2512;
//夺宝数据
const unsigned short ID_Gs2Ci_RequestPlayerSnatchInfo_Reply=2513;
//夺宝
const unsigned short ID_Ci2Gs_RequestPlayerSnatch=2514;
//夺宝
const unsigned short ID_Gs2Ci_RequestPlayerSnatch_Reply=2515;
//夺宝5连
const unsigned short ID_Ci2Gs_RequestPlayerSnatchFive=2516;
//夺宝5连
const unsigned short ID_Gs2Ci_RequestPlayerSnatchFive_Reply=2517;
//夺宝碎片合成
const unsigned short ID_Ci2Gs_RequestPlayerSnatchCompound=2518;
//夺宝碎片合成
const unsigned short ID_Gs2Ci_RequestPlayerSnatchCompound_Reply=2519;
//夺宝免战
const unsigned short ID_Ci2Gs_RequestPlayerSnatchFreeWar=2520;
//夺宝免战
const unsigned short ID_Gs2Ci_RequestPlayerSnatchFreeWar_Reply=2521;
//夺宝精力
const unsigned short ID_Ci2Gs_RequestPlayerBuyVigour=2522;
//夺宝精力
const unsigned short ID_Gs2Ci_RequestPlayerBuyVigour_Reply=2523;
//夺宝匹配
const unsigned short ID_Ci2Gs_RequestPlayerSnatchMate=2524;
//夺宝匹配
const unsigned short ID_Gs2Ci_RequestPlayerSnatchMate_Reply=2525;
//
const unsigned short ID_Ci2Gs_RequestVigourRestore=2526;
//
const unsigned short ID_Gs2Ci_RequestVigourRestore_Reply=2527;
//
const unsigned short ID_Gs2Ci_RequestVigourWar_Reply=2528;
//神兵进阶
const unsigned short ID_Ci2Gs_RequestGodWeaponUpgradeStage=2529;
//神兵进阶
const unsigned short ID_Gs2Ci_RequestGodWeaponUpgradeStage_Reply=2530;
//神兵十次升级
const unsigned short ID_Ci2Gs_RequestGodWeaponTenUpgrade=2531;
//神兵十次升级
const unsigned short ID_Gs2Ci_RequestGodWeaponTenUpgrade_Reply=2532;
//时间同步
const unsigned short ID_Ci2Gs_RequestServerSysTime=2533;
//时间同步
const unsigned short ID_Gs2Ci_RequestServerSysTime_Reply=2534;
//一键合成
const unsigned short ID_Ci2Gs_RequestOneKeyCompound=2535;
//一键合成
const unsigned short ID_Gs2Ci_RequestOneKeyCompound_Reply=2536;
//请求VIP特权商店购买信息
const unsigned short ID_Ci2Gs_RequestVipItemShopData=2537;
//返回VIP特权商店购买信息
const unsigned short ID_Gs2Ci_RequestVipItemShopData_Reply=2538;
//请求购买VIP特权商店
const unsigned short ID_Ci2Gs_RequestBuyVipItemShop=2539;
//返回购买VIP特权商店
const unsigned short ID_Gs2Ci_RequestBuyVipItemShop_Reply=2540;
//请求商城购买信息
const unsigned short ID_Ci2Gs_RequestShopGameData=2541;
//返回V商城购买信息
const unsigned short ID_Gs2Ci_RequestShopGameData_Reply=2542;
//请求购买商城道具
const unsigned short ID_Ci2Gs_RequestBuyShopGameItem=2543;
//返回请求购买商城道具
const unsigned short ID_Gs2Ci_RequestBuyShopGameItem_Reply=2544;
//请求添加好友
const unsigned short ID_Ci2Gs_RequestInviteAddFriend=2545;
//请求添加好友
const unsigned short ID_Gs2Ci_RequestInviteAddFriend_Reply=2546;
//拒绝添加好友
const unsigned short ID_Ci2Gs_RefuseAddFriend=2547;
//拒绝添加好友
const unsigned short ID_Gs2Ci_RefuseAddFriend_Reply=2548;
//删除好友
const unsigned short ID_Gs2Ci_PlayerDelFriend_Reply=2549;
//请求申请列表消息
const unsigned short ID_Ci2Gs_QueryInvitedFriendData=2550;
//请求申请列表消息
const unsigned short ID_Gs2Ci_QueryInvitedFriendData_Reply=2551;
//请求购买扫荡卷
const unsigned short ID_Ci2Gs_RequestBuySaoDangJuan=2552;
//返回购买扫荡卷
const unsigned short ID_Gs2Ci_RequestBuySaoDangJuan_Reply=2553;
//请求创建工会战房间
const unsigned short ID_Ci2Gs_RequestGuildWarCreateRoom=2554;
//返回创建工会战房间
const unsigned short ID_Gs2Ci_RequestGuildWarCreateRoom_Reply=2555;
//请求查询工会战信息
const unsigned short ID_Ci2Gs_RequestGuildWarInfo=2556;
//返回查询工会战信息
const unsigned short ID_Gs2Ci_RequestGuildWarInfo_Reply=2557;
//请求加入工会战房间
const unsigned short ID_Ci2Gs_RequestGuildWarJoinRoom=2558;
//请求加入工会战房间
const unsigned short ID_Gs2Ci_RequestGuildWarJoinRoom_Reply=2559;
//刷新工会战房间
const unsigned short ID_Gs2Ci_RefreshGuildWarRoom_Reply=2560;
//请求离开工会战房间
const unsigned short ID_Ci2Gs_RequestGuildWarLeaveRoom=2561;
//返回离开工会战房间
const unsigned short ID_Gs2Ci_RequestGuildWarLeaveRoom_Reply=2562;
//请求战斗开始
const unsigned short ID_Ci2Gs_RequestGuildWarStart=2563;
//返回战斗开始
const unsigned short ID_Gs2Ci_RequestGuildWarStart_Reply=2564;
//战斗结束
const unsigned short ID_Ci2Gs_RequestGuildWarEnd=2565;
//返回战斗结束
const unsigned short ID_Gs2Ci_RequestGuildWarEnd_Reply=2566;
//购买无尽战斗阶段奖励
const unsigned short ID_Ci2Gs_BuyEverBattleStageRewards=2567;
//购买无尽战斗阶段奖励
const unsigned short ID_Gs2Ci_BuyEverBattleStageRewards_Reply=2568;
//请求月卡剩余天数
const unsigned short ID_Ci2Gs_RequestMonthCardRemainDay=2569;
//请求月卡剩余天数返回
const unsigned short ID_Gs2Ci_RequestMonthCardRemainDay_Reply=2570;
//运营活动-连续七日登录进度请求
const unsigned short ID_Ci2Gs_RequestOASuccessiveLogin=2571;
//运营活动-连续七日登录进度返回
const unsigned short ID_Gs2Ci_RequestOASuccessiveLogin_Reply=2572;
//查询公会列表
const unsigned short ID_Ci2Gs_RequestQueryGuildList=2573;
//返回查询公会列表
const unsigned short ID_Gs2Ci_RequestQueryGuildList_Reply=2574;
//查询公会会长信息
const unsigned short ID_Ci2Gs_RequestQueryGuildMaster=2575;
//返回查询公会信息
const unsigned short ID_Gs2Ci_RequestQueryGuildMaster_Reply=2576;
//创建公会
const unsigned short ID_Ci2Gs_RequestCreateGuild=2577;
//返回创建公会
const unsigned short ID_Gs2Ci_RequestCreateGuild_Reply=2578;
//查询自己公会信息
const unsigned short ID_Ci2Gs_RequestQueryGuild=2579;
//返回查询自己公会信息
const unsigned short ID_Gs2Ci_RequestQueryGuild_Reply=2580;
//查询公会成员列表
const unsigned short ID_Ci2Gs_RequestQueryGuildPlayer=2581;
//返回查询公会成员列表
const unsigned short ID_Gs2Ci_RequestQueryGuildPlayer_Reply=2582;
//删除公会
const unsigned short ID_Ci2Gs_RequestDeleteGuild=2583;
//返回删除公会
const unsigned short ID_Gs2Ci_RequestDeleteGuild_Reply=2584;
//查询公会申请列表
const unsigned short ID_Ci2Gs_RequestQueryGuildInviList=2585;
//返回查询公会申请列表
const unsigned short ID_Gs2Ci_RequestQueryGuildInviList_Reply=2586;
//查询玩家公会数据
const unsigned short ID_Ci2Gs_RequestQueryPlayerGuild=2587;
//查询玩家公会数据
const unsigned short ID_Gs2Ci_RequestQueryPlayerGuild_Reply=2588;
//加入公会
const unsigned short ID_Ci2Gs_RequestAddGuild=2589;
//返回加入公会
const unsigned short ID_Gs2Ci_RequestAddGuild_Reply=2590;
//退出公会
const unsigned short ID_Ci2Gs_RequestQuitGuild=2591;
//返回退出公会
const unsigned short ID_Gs2Ci_RequestQuitGuild_Reply=2592;
//踢出公会
const unsigned short ID_Ci2Gs_RequestKickGuild=2593;
//返回踢出公会
const unsigned short ID_Gs2Ci_RequestKickGuild_Reply=2594;
//公会捐献
const unsigned short ID_Ci2Gs_RequestGuildDonate=2595;
//返回公会捐献
const unsigned short ID_Gs2Ci_RequestGuildDonate_Reply=2596;
//公会同意加入
const unsigned short ID_Ci2Gs_RequestGuildAgreeAdd=2597;
//返回公会同意加入
const unsigned short ID_Gs2Ci_RequestGuildAgreeAdd_Reply=2598;
//公会拒绝加入
const unsigned short ID_Ci2Gs_RequestGuildRefuse=2599;
//返回公会拒绝加入
const unsigned short ID_Gs2Ci_RequestGuildRefuse_Reply=2600;
//公会一键拒绝加入
const unsigned short ID_Ci2Gs_RequestGuildRefuseAll=2601;
//返回公会一键拒绝加入
const unsigned short ID_Gs2Ci_RequestGuildRefuseAll_Reply=2602;
//公会转让会长
const unsigned short ID_Ci2Gs_RequestGuildTransferCaption=2603;
//返回公会转让会长
const unsigned short ID_Gs2Ci_RequestGuildTransferCaption_Reply=2604;
//公会升职降职
const unsigned short ID_Ci2Gs_RequestGuildChangeOccupation=2605;
//返回公会升职降职
const unsigned short ID_Gs2Ci_RequestGuildChangeOccupation_Reply=2606;
//公会修改公告
const unsigned short ID_Ci2Gs_RequestGuildModifyNotify=2607;
//返回公会修改公告
const unsigned short ID_Gs2Ci_RequestGuildModifyNotify_Reply=2608;
//公会修改宣言
const unsigned short ID_Ci2Gs_RequestGuildModifyNote=2609;
//返回公会修改宣言
const unsigned short ID_Gs2Ci_RequestGuildModifyNote_Reply=2610;
//查询公会名字
const unsigned short ID_Ci2Gs_RequestSearchGuildName=2611;
//返回查询公会名字
const unsigned short ID_Gs2Ci_RequestSearchGuildName_Reply=2612;
//查询组队副本信息
const unsigned short ID_Ci2Gs_RequestActivityTeamLevelData=2613;
//查询组队副本信息
const unsigned short ID_Gs2Ci_RequestActivityTeamLevelData_Reply=2614;
//进入组队副本
const unsigned short ID_Ci2Gs_RequestEnterActivityTeamLevel=2615;
//进入组队副本
const unsigned short ID_Gs2Ci_RequestEnterActivityTeamLevel_Reply=2616;
//获取组队副本奖励
const unsigned short ID_Ci2Gs_RequestGetTeamLevelRewards=2617;
//获取组队副本奖励
const unsigned short ID_Gs2Ci_RequestGetTeamLevelRewards_Reply=2618;
//匹配对手
const unsigned short ID_Ci2Gs_RequestStartTeamLevelOpponent=2619;
//匹配对手
const unsigned short ID_Gs2Ci_RequestStartTeamLevelOpponent_Reply=2620;
//请求七日嘉年华进度信息
const unsigned short ID_Ci2Gs_RequestSevenDayCarnivalProcess=2621;
//请求七日嘉年华进度信息返回
const unsigned short ID_Gs2Ci_RequestSevenDayCarnivalProcess_Reply=2622;
//领取七日嘉年华奖励
const unsigned short ID_Ci2Gs_RequestTakeSevenDayCarnivalReWard=2623;
//领取七日嘉年华奖励返回
const unsigned short ID_Gs2Ci_RequestTakeSevenDayCarnivalReWard_Reply=2624;
//领取关卡宝箱
const unsigned short ID_Ci2Gs_RequestGetLevelsBox=2625;
//领取关卡宝箱返回
const unsigned short ID_Gs2Ci_RequestGetLevelsBox_Reply=2626;
//获取淬炼的解锁状态
const unsigned short ID_Ci2Gs_RequestEquipEnhanceStatus=2627;
//获取淬炼的解锁状态
const unsigned short ID_Gs2Ci_RequestEquipEnhanceStatus_Reply=2628;
//碎片合成
const unsigned short ID_Ci2Gs_RequestCompoundChipItem=2629;
//碎片合成
const unsigned short ID_Gs2Ci_RequestCompoundChipItem_Reply=2630;
//碎片分解
const unsigned short ID_Ci2Gs_RequestDecomposeChipItem=2631;
//碎片分解
const unsigned short ID_Gs2Ci_RequestDecomposeChipItem_Reply=2632;
//请求指定任务信息
const unsigned short ID_Ci2Gs_RequestSevenDayCarnivalTaskInfo=2633;
//指定任务信息返回
const unsigned short ID_Gs2Ci_RequestSevenDayCarnivalTaskInfo_Reply=2634;
//请求限时购物品情况
const unsigned short ID_Ci2Gs_RequestBuyLimitTimeInfo=2635;
//请求限时购物品情况返回
const unsigned short ID_Gs2Ci_RequestBuyLimitTimeInfo_Reply=2636;
//购买限时购物品
const unsigned short ID_Ci2Gs_RequestBuyLimitTimePurchaseItem=2637;
//购买限时购物品返回
const unsigned short ID_Gs2Ci_RequestBuyLimitTimePurchaseItem_Reply=2638;
//符文分解
const unsigned short ID_Ci2Gs_RequestDecomposeSymbol=2639;
//符文分解
const unsigned short ID_Gs2Ci_RequestDecomposeSymbol_Reply=2640;
//请求进行道具兑换
const unsigned short ID_Ci2Gs_RequestItemExchange=2641;
//请求进行道具兑换返回
const unsigned short ID_Gs2Ci_RequestItemExchange_Reply=2642;
//激活膜拜信息
const unsigned short ID_Ci2Gs_RequestActiveWorship=2643;
//激活膜拜信息返回
const unsigned short ID_Gs2Ci_RequestActiveWorship_Reply=2644;
//获取激活膜拜信息
const unsigned short ID_Ci2Gs_RequestActiveWorshipData=2645;
//获取激活膜拜信息返回
const unsigned short ID_Gs2Ci_RequestActiveWorshipData_Reply=2646;
//膜拜/激活膜拜
const unsigned short ID_Ci2Gs_RequestWorship=2647;
//激活膜拜返回
const unsigned short ID_Gs2Ci_RequestWorship_Reply=2648;
//请求公会技能列表
const unsigned short ID_Ci2Gs_RequestGuildSkill=2649;
//请求公会技能列表返回
const unsigned short ID_Gs2Ci_RequestGuildSkill_Reply=2650;
//请求公会玩家技能列表
const unsigned short ID_Ci2Gs_RequestPlayerGuildSkill=2651;
//请求公会玩家技能列表返回
const unsigned short ID_Gs2Ci_RequestPlayerGuildSkill_Reply=2652;
//请求公会技能升级
const unsigned short ID_Ci2Gs_RequestGuildSkillUpgrade=2653;
//请求公会技能升级返回
const unsigned short ID_Gs2Ci_RequestGuildSkillUpgrade_Reply=2654;
//请求公会玩家技能升级
const unsigned short ID_Ci2Gs_RequestPlayerGuildSkillUpgrade=2655;
//请求公会玩家技能升级返回
const unsigned short ID_Gs2Ci_RequestPlayerGuildSkillUpgrade_Reply=2656;
//请求改变神兵位置
const unsigned short ID_Ci2Gs_RequestChangeGodWeaponLocation=2657;
//请求改变神兵位置返回
const unsigned short ID_Gs2Ci_RequestChangeGodWeaponLocation_Reply=2658;
//请求获取神兵位置
const unsigned short ID_Ci2Gs_RequestGetGodWeaponLocation=2659;
//请求获取神兵位置返回
const unsigned short ID_Gs2Ci_RequestGetGodWeaponLocation_Reply=2660;
//请求神兵信息
const unsigned short ID_Gs2Ci_RequestGodWeaponManagers=2661;
//请求神兵信息的返回
const unsigned short ID_Gs2Ci_RequestGodWeaponManagers_Reply=2662;
//神秘商店刷新次数恢复
const unsigned short ID_Ci2Gs_RequestMyticalTimesRestore=2663;
//神秘商店刷新次数恢复返回
const unsigned short ID_Gs2Ci_RequestMyticalTimesRestore_Reply=2664;
//腾讯应用宝认证信息上报
const unsigned short ID_Ci2Gs_RequestTxUserLoginInfo=2665;
//腾讯应用宝认证信息上报结果
const unsigned short ID_Gs2Ci_RequestTxUserLoginInfo_Reply=2666;
//同步玩家的货币
const unsigned short ID_Ci2Gs_RequestSyncMoneyInfo=2667;
//同步玩家的货币的结果
const unsigned short ID_Gs2Ci_RequestSyncMoneyInfo_Reply=2668;
//实时对战进入请求
const unsigned short ID_Ci2Gs_RequestRealTimePVPEnter=2669;
//实时对战进入请求返回
const unsigned short ID_Gs2Ci_RequestRealTimePVPEnter_Reply=2670;
//实时对战记录请求
const unsigned short ID_Ci2Gs_RequestRealTimePVPRecord=2671;
//实时对战记录请求返回
const unsigned short ID_Gs2Ci_RequestRealTimePVPRecord_Reply=2672;
//查询实时对战排行榜
const unsigned short ID_Ci2Gs_QueryRealTimePVPRankInfo=2673;
//查询实时对战排行榜返回
const unsigned short ID_Gs2Ci_QueryRealTimePVPRankInfo_Reply=2674;
//实时对战开始匹配请求
const unsigned short ID_Ci2Gs_RequestRealTimePVPStartMatch=2675;
//实时对战开始匹配请求返回
const unsigned short ID_Gs2Ci_RequestRealTimePVPStartMatch_Reply=2676;
//实时对战玩家开始请求
const unsigned short ID_Ci2Gs_RequestRealTimePVPPlayerStart=2677;
//实时对战玩家开始请求返回
const unsigned short ID_Gs2Ci_RequestRealTimePVPPlayerStart_Reply=2678;
//实时对战战斗结算
const unsigned short ID_Gs2Ci_RequestRealTimePVPPRewardResult=2679;
//实时对战匹配超时响应
const unsigned short ID_Gs2Ci_RealTimePVPPMatchTimeout_Reply=2680;
//实时对战更新面板请求
const unsigned short ID_Ci2Gs_RequestRealTimePVPUpdatePane=2681;
//实时对战更新面板返回
const unsigned short ID_Gs2Ci_RequestRealTimePVPUpdatePane_Reply=2682;
//玩家实时对战退出请求
const unsigned short ID_Ci2Gs_clientRequestRealTimePVPQuite=2683;
//请求矿战数据
const unsigned short ID_Ci2Gs_RequestQueryGoldStone=2684;
//返回矿战数据结果
const unsigned short ID_Gs2Ci_RequestQueryGoldStone_Reply=2685;
//请求抢矿
const unsigned short ID_Ci2Gs_RequestGrabGoldStone=2686;
//返回抢矿结果
const unsigned short ID_Gs2Ci_RequestGrabGoldStone_Reply=2687;
//请求抢矿列表
const unsigned short ID_Ci2Gs_RequestGrabGoldStoneList=2688;
//返回请求抢矿列表
const unsigned short ID_Gs2Ci_RequestGrabGoldStoneList_Reply=2689;
//请求玩家抢矿信息
const unsigned short ID_Ci2Gs_RequestPlayerGoldStone=2690;
//返回请求玩家抢矿信息
const unsigned short ID_Gs2Ci_RequestPlayerGoldStone_Reply=2691;
//购买抢矿次数
const unsigned short ID_Ci2Gs_RequestBuyGoldStoneTimes=2692;
//返回购买抢矿次数
const unsigned short ID_Gs2Ci_RequestBuyGoldStoneTimes_Reply=2693;
//提前收取
const unsigned short ID_Ci2Gs_RequestTakeGoldStone=2694;
//返回提前收取
const unsigned short ID_Gs2Ci_RequestTakeGoldStone_Reply=2695;
//上报客户端支付结果
const unsigned short ID_Ci2Gs_RequestClientProcessRechageInnerOrder=2696;
//返回客户端支付结果的上报结果
const unsigned short ID_Gs2Ci_RequestClientProcessRechageInnerOrder_Reply=2697;
//请求活跃活动的进度情况
const unsigned short ID_Ci2Gs_RequestOAEventsProcess=2698;
//请求活跃活动的进度情况返回
const unsigned short ID_Gs2Ci_RequestOAEventsProcess_Reply=2699;
//请求领取活跃活动的奖励
const unsigned short ID_Ci2Gs_RequestTakeOAEventsReward=2700;
//请求领取活跃活动的奖励返回
const unsigned short ID_Gs2Ci_RequestTakeOAEventsReward_Reply=2701;
//玩家所有任务变量同步
const unsigned short ID_Gs2Ci_PlayerAllVarsSync=2702;
//玩家单个任务变量同步
const unsigned short ID_Gs2Ci_PlayerOneVarSync=2703;
//玩家任务变量设置
const unsigned short ID_Ci2Gs_PlayerVarSet=2704;
//请求健康达人的进度情况
const unsigned short ID_Ci2Gs_RequestOAHealthyProcess=2705;
//请求健康达人的进度情况返回
const unsigned short ID_Gs2Ci_RequestOAHealthyProcess_Reply=2706;
//请求领取健康达人的奖励
const unsigned short ID_Ci2Gs_RequestTakeOAHealthyReward=2707;
//请求领取健康达人的奖励返回
const unsigned short ID_Gs2Ci_RequestTakeOAHealthyReward_Reply=2708;
//请求神兵商店数据
const unsigned short ID_Ci2Gs_RequestQueryGodShop=2709;
//请求神兵商店数据返回
const unsigned short ID_Gs2Ci_RequestQueryGodShop_Reply=2710;
//请求书刷新神兵商店
const unsigned short ID_Ci2Gs_RequestRefreshGodShop=2711;
//请求神兵商店数据返回
const unsigned short ID_Gs2Ci_RequestRefreshGodShop_Reply=2712;
//购买道具
const unsigned short ID_Ci2Gs_RequestBuyGodShopItem=2713;
//购买道具返回
const unsigned short ID_Gs2Ci_RequestBuyGodShopItem_Reply=2714;
//神兵商店回复次数
const unsigned short ID_Ci2Gs_RequestGodTimesRestore=2715;
//神兵商店回复次数
const unsigned short ID_Gs2Ci_RequestGodTimesRestore_Reply=2716;
//请求进行道具兑换情况
const unsigned short ID_Ci2Gs_RequestItemExchangeInfo=2717;
//请求进行道具兑换情况返回
const unsigned short ID_Gs2Ci_RequestItemExchangeInfo_Reply=2718;
//通知客户端活动开启的信息
const unsigned short ID_Gs2Ci_ServerNotityOptActivityStart=2719;
//通知客户端活动关闭的信息
const unsigned short ID_Gs2Ci_ServerNotityOptActivityEnd=2720;
//返回已经开始的活动信息给客户端
const unsigned short ID_Gs2Ci_ServerSendOptActivityList_Reply=2721;
//运营活动-客户端请求已开启的活动列表
const unsigned short ID_Ci2Gs_RequestOpertingActivityList=2722;
//运营活动-客户端请求当前团购道具的信息 <<第几期<<道具ID<<数量
const unsigned short ID_Ci2Gs_RequestGroupBuyingItemInfo=2723;
//返回客户端当前团购道具的信息  <<请求成功<<团购道具的数据结构(<<道具ID<<当前购买总数<<当前价格)<<今日剩余可购买次数<<当前团购积分<<当前团购券
const unsigned short ID_Gs2Ci_RequestGroupBuyingItemInfo_Reply=2724;
//运营活动-客户端请求团购道具 <<第几期<<道具ID<<道具数量<<抵用的团购券数量
const unsigned short ID_Ci2Gs_RequestGroupBuyItem=2725;
//返回客户端当前团购道具购买成功  <<请求成功<<团购道具的数据结构(<<道具ID<<当前购买总数<<当前价格)<<当前元宝<<当前团购积分<<当前团购券<<今日剩余可购买次数
const unsigned short ID_Gs2Ci_RequestGroupBuyItem_Reply=2726;
//客户端请求团购积分奖励 <<奖励序号<<第几期
const unsigned short ID_Ci2Gs_RequestGroupBuyScoreReward=2727;
//返回客户端当前团购积分奖励<<请求成功<<奖励领取状态<<奖励数量
const unsigned short ID_Gs2Ci_RequestGroupBuyScoreReward_Reply=2728;
//客户端请求团购积分奖励领取信息
const unsigned short ID_Ci2Gs_RequestGetScoreRewardInfo=2729;
//返回客户端当前团购积分奖励领取信息<<请求成功<<奖励领取状态
const unsigned short ID_Gs2Ci_RequestGetScoreRewardInfo_Reply=2730;
//请求转盘情况
const unsigned short ID_Ci2Gs_RequestTurnTableInfo=2731;
//请求转盘情况返回
const unsigned short ID_Gs2Ci_RequestTurnTableInfo_Reply=2732;
//请求转盘奖励
const unsigned short ID_Ci2Gs_RequestTurnTableReward=2733;
//请求转盘奖励返回
const unsigned short ID_Gs2Ci_RequestTurnTableReward_Reply=2734;
//客户端版本请求
const unsigned short ID_Ci2Gs_RequestClientVersin=2735;
//客户端版本请求返回
const unsigned short ID_Gs2Ci_RequestClientVersin_Reply=2736;
//客户端更新版本奖励请求
const unsigned short ID_Ci2Gs_RequestUpdateVersinReward=2737;
//客户端更新版本奖励返回
const unsigned short ID_Gs2Ci_RequestUpdateVersinReward_Reply=2738;
//金币副本扫荡请求
const unsigned short ID_Ci2Gs_RequestGoldLevelSaoDang=2739;
//金币副本扫荡请求返回
const unsigned short ID_Gs2Ci_RequestGoldLevelSaoDang_Reply=2740;
//经验副本扫荡请求
const unsigned short ID_Ci2Gs_RequestEXPLevelSaoDang=2741;
//经验副本扫荡请求
const unsigned short ID_Gs2Ci_RequestEXPLevelSaoDang_Reply=2742;
//真气副本扫荡请求
const unsigned short ID_Ci2Gs_RequestSPLevelSaoDang=2743;
//真气副本扫荡请求返回
const unsigned short ID_Gs2Ci_RequestSPLevelSaoDang_Reply=2744;
//请求赠送精力
const unsigned short ID_Ci2Gs_RequestPresentVigour=2745;
//请求赠送精力返回
const unsigned short ID_Gs2Ci_RequestPresentVigour_Reply=2746;
//通知获赠精力
const unsigned short ID_Gs2Ci_NoticeAddRecieveVigour_Reply=2747;
//请求领取精力
const unsigned short ID_Ci2Gs_RequestRecieveVigour=2748;
//请求领取精力返回
const unsigned short ID_Gs2Ci_RequestRecieveVigour_Reply=2749;
//请求获赠精力列表信息
const unsigned short ID_Ci2Gs_RequestRecieveVigourList=2750;
//请求获赠精力列表信息返回
const unsigned short ID_Gs2Ci_RequestRecieveVigourList_Reply=2751;
//一键强化
const unsigned short ID_Ci2Gs_RequestOneKeyPartEnhance=2752;
//一键强化
const unsigned short ID_Gs2Ci_RequestOneKeyPartEnhance_Reply=2753;
//此消息永远放在最后
const unsigned short ID_Ci2Ls_MAX_SCENE_CMD_ID=2754;
} //end of namespace
#endif
