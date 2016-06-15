/**
 * \file
 * \version  $Id: zXMLParser.h 13 2013-03-20 02:35:18Z  $
 * \author  ,okyhc@263.sina.com
 * \date 2004年11月03日 14时15分45秒 CST
 * \brief zXMLParser器定义文件
 *
 * 
 */

#ifndef _ZXMLPARSE_H_
#define _ZXMLPARSE_H_
#include <string>

struct _xmlNode;
struct _xmlDoc;
/**
 * \brief zXMLParser定义
 * 
 * 主要提供了节点的浏览,和其属性的得到.
 */
class zXMLParser
{
	public:
		zXMLParser();
		~zXMLParser();

		bool initFile(const std::string &xmlFile);
		bool initFile(const char *xmlFile);
		bool initStr(const std::string &xmlStr);
		bool initStr(const char *xmlStr);
		bool init();
		void final();
		std::string & dump(std::string & s, bool format=false);
		std::string & dump(_xmlNode* dumpNode,std::string & s,bool head=true);
		std::string & dumpNohead(std::string & s);
		_xmlNode* getRootNode(const char *rootName);
		_xmlNode* getChildNode(_xmlNode* parent, const char *childName);
		_xmlNode* getNextNode(_xmlNode* node,const char *nextName);
		unsigned int getChildNodeNum(_xmlNode* parent, const char *childName);

		_xmlNode* newRootNode(const char *rootName);
		_xmlNode* newChildNode(_xmlNode* parent, const char *childName, const char *content);
		bool newNodeProp(_xmlNode* node,const char *propName,const char *prop);
		bool newNodeProp(_xmlNode* node,const char *propName,const char *prop,const char* encoding);

		bool isNodeName(_xmlNode* node, const std::string& name);

		bool getNodePropNum(_xmlNode* node,const char *propName,void *prop,int propSize);
		bool getNodePropTime(_xmlNode* node,const char *propName,time_t &prop);
		bool getNodePropStr(_xmlNode* node,const char *propName,void *prop,int propSize);
		bool getNodePropStr(_xmlNode* node,const char *propName,std::string &prop);
		
		bool getNodeContentNum(_xmlNode* node,void *content, int contentSize);
		bool getNodeContentTime(_xmlNode* node,time_t &prop);
		bool getNodeContentStr(_xmlNode* node,void *content, int contentSize);
		bool getNodeContentStr(_xmlNode* node,std::string &content);
	private:
		unsigned char *charConv(unsigned char *in, const char *fromEncoding, const char *toEncoding);
		_xmlDoc* doc;
};
#endif
