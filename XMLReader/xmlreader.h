#ifndef XMLREADER
#define XMLREADER
#include <string>
#include <map>
#include <vector>
#include <iostream>

namespace NewYorkTime{
	const std::string SPACE = " \t\n";
	void replace(std::string & s);

	class XmlNode{
	public:
		XmlNode(std::string content);
		XmlNode(const XmlNode & x);
		~XmlNode();

		std::string getText()const{ return text; }
		std::string getAllText()const;

		void getNodesByTag(std::string tag, std::vector<const XmlNode*>& v)const;
		std::vector<const XmlNode*> getNodesByTag(std::string tag)const;
		const XmlNode* getNodeByTag(const std::string& tag)const;

		std::vector<const XmlNode*> getNodesByAttr(const std::string & attr, const std::string & value, std::vector<const XmlNode*>* = NULL)const;
		const XmlNode* getNodeByAttr(const std::string & attr, const std::string & value)const;

		std::string getAttrValue(const std::string & attr)const;

		friend std::ostream & operator << (std::ostream & os, const XmlNode& xn);
	private:
		std::string  text, tag;
		std::vector<XmlNode*>nodes;
		std::map<std::string, std::string> attr;
		bool isSimpleTag, isSelfClose;
	};
	class Xml{
	public:
		Xml(std::string content);
		Xml(const Xml& x) :xmlDecl(x.xmlDecl), xmlDocType(x.xmlDocType), root(new XmlNode(*x.root)){}
		~Xml(){ delete root; }
		const XmlNode& getRoot()const{ return *root; }
		friend std::ostream& operator << (std::ostream & os, const Xml& x);
	private:
		XmlNode *root;
		std::string xmlDecl;
		std::string xmlDocType;
	};

}

#endif  //XMLREADER