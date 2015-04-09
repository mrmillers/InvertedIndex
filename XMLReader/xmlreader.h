#ifndef XMLREADER
#define XMLREADER
#include <string>
#include <map>
#include <vector>
#include <iostream>

namespace NewYorkTime{
	const std::string SPACE = " \t\n";
	static void replace(std::string & s){
		/*
		&lt;	<	less than
		&gt;	>	greater than
		&amp;	&	ampersand
		&apos;	'	apostrophe
		&quot;	"	quotation mark
		*/
		static std::string from[] = { "&lt;", "&gt;", "&amp;", "&apos;", "&quot;" };
		static std::string to[] = { "<", ">", "&", "'", "\"" };

		for (int i = 0; i < 5; i++){
			int start = 0;
			while ((start = s.find(from[i], start)) != std::string::npos){
				s.replace(start, from[i].length(), to[i]);
				start++;
			}
		}
	}

	class XmlNode{
	public:
		XmlNode(std::string content) :isSimpleTag(true){
			int start = content.find("<");
			int end = content.find(">");

 			std::string name = content.substr(start, end - start + 1);
			int tag_start = name.find_first_not_of(SPACE, 1);
			int tag_end = name.find_first_of(SPACE + ">/", tag_start);
			this->tag = name.substr(tag_start, tag_end - tag_start);
			if (content[end - 1] != '/'){
				int tmp = content.find("</" + tag + ">");
				this->text = content.substr(end + 1, tmp - end - 1);
				isSelfClose = false;
			}
			else{
				isSelfClose = true;
			}

			int attr_start = tag_end;
			int eq;
			while ((eq = name.find("=", attr_start)) != std::string::npos){
				int attr_name_start = name.find_first_not_of(SPACE, attr_start);
				int attr_name_end = name.find_first_of(SPACE + "=", attr_name_start);
				int attr_value_start = name.find("\"", eq + 1) + 1;
				int attr_value_end = name.find("\"", attr_value_start);
				attr[name.substr(attr_name_start, attr_name_end - attr_name_start)] = name.substr(attr_value_start, attr_value_end - attr_value_start);
				attr_start = attr_value_end + 1;
			}

			while (text.find("<") != std::string::npos && text.find(">") != std::string::npos){
				isSimpleTag = false;
				XmlNode* tmp = new XmlNode(text);
				nodes.push_back( tmp );
				int end;
				if (tmp->isSelfClose) {
					end = text.find("/>") + 2;
				}
				else{
					end = text.find("</" + tmp->tag + ">") + 3 + tmp->tag.length();
				}
				text = text.substr(end);
			}
			if (!isSimpleTag) text = "";
			replace(text);

		}
		XmlNode(const XmlNode & x) :text(x.text), tag(x.tag), attr(x.attr){
			for (unsigned int i = 0; i < nodes.size();i++){
				nodes[i] = new XmlNode(*x.nodes[i]);
			}
		}
		~XmlNode(){
			for (unsigned int i = 0; i < nodes.size(); i++)
				delete nodes[i];
		}
		friend std::ostream & operator << (std::ostream & os, const XmlNode& xn) {
			os << "<" << xn.tag << " ";
			for (std::map<std::string, std::string>::const_iterator itr = xn.attr.begin(); itr != xn.attr.end(); ++itr){
				os << itr->first << "=" "\"" << itr->second << "\" ";
			}
			if (xn.isSelfClose) os << "/>\n";
			else{
				os << ">\n";
				if (xn.isSimpleTag){
					os << xn.text << "\n";
				}
				else{
					for (unsigned int i = 0; i < xn.nodes.size(); i++){
						os << *xn.nodes[i];
					}

				}
				os << "</" << xn.tag << ">\n";
			}
			return os;
		}
		
		std::string getText()const{ return text; }
		std::string getAllText()const {
			if (isSimpleTag)
				return text;
			else{
				std::string ret;
				for (unsigned int i = 0; i < nodes.size();i++){
					ret += nodes[i]->getAllText() + " ";
				}
				return ret;
			}
		}
		void getNodesByTag(std::string tag, std::vector<const XmlNode*>& v)const{
			if (this->tag == tag)
				v.push_back(this);
			else{
				for (int i = 0; i < this->nodes.size(); i++){
					nodes[i]->getNodesByTag(tag,v);
				}
			}
		}
		std::vector<const XmlNode*> getNodesByTag(std::string tag)const{
			std::vector<const XmlNode*>v;
			if (this->tag == tag)
				v.push_back(this);
			else{
				for (int i = 0; i < this->nodes.size(); i++){
					nodes[i]->getNodesByTag(tag, v);
				}
			}
			return v;
		}
		const XmlNode* getNodeByTag(const std::string& tag)const{
			std::vector<const XmlNode*>v;
			getNodesByTag(tag,v);
			if (v.size()>0)
				return v[0];
			return NULL;
		}
	private:
		std::string  text, tag;
		std::vector<XmlNode*>nodes;
		std::map<std::string, std::string> attr;
		bool isSimpleTag, isSelfClose;
	};
	class Xml{
	public:
		Xml(std::string content){
			int comment_start = 0;
			while ((comment_start = content.find("<!--")) != std::string::npos){
				int comment_end = content.find("-->", comment_start);
				content.replace(comment_start, comment_end + 3 - comment_start, "");
			}

			int pos = 0;
			int start = content.find("<?");
			int end = content.find("?>");
			if (start != std::string::npos && end != std::string::npos){
				xmlDecl = content.substr(start, end - start + 2);
				pos = end + 2;
			}
			start = content.find("<!", pos);
			end = content.find(">", pos);
			if (start != std::string::npos && end != std::string::npos){
				xmlDocType = content.substr(start, end - start + 1);
				pos = end + 2;
			}
			root = new XmlNode(content.substr(pos));
		}
		Xml(const Xml& x) :xmlDecl(x.xmlDecl), xmlDocType(x.xmlDocType), root(new XmlNode(*x.root)){}
		~Xml(){
			delete root;
		}
		const XmlNode& getRoot()const{ return *root; }
		friend std::ostream& operator << (std::ostream & os, const Xml& x){
			os << x.xmlDecl << "\n" << x.xmlDocType << "\n";
			os << *x.root;
			return os;
		}
	private:
		XmlNode *root;
		std::string xmlDecl;
		std::string xmlDocType;
	};

}

#endif  //XMLREADER