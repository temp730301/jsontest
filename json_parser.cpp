#include <memory>
#include <string>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

class jsonParser {
	public:
		jsonParser(const char *jStr)
		{
			m_document.Parse(jStr);
			
			if (m_document.IsObject() == false) {
				printf("jstr was not Json\n");
			}
		}
		
		jsonParser(Value &value)
		{
#if 1
rapidjson::StringBuffer sb;
rapidjson::Writer<StringBuffer> writer(sb);
value.Accept(writer);
printf("jsonParser: Value=[%s]\n", sb.GetString());
#endif
			m_document.CopyFrom(value, m_document.GetAllocator());
			
			if (m_document.IsObject() == false) {
				printf("value was not Json value\n");
			}
		}
		
		~jsonParser() {printf("destructor\n");}
		
		static std::shared_ptr<jsonParser> creatJsonParser(const char *jsonStr)
		{
			return std::shared_ptr<class jsonParser> (new jsonParser(jsonStr));
		}
		
		std::shared_ptr<jsonParser> creatAryObj(const char *keywd, int index)
		{
			rapidjson::Value::ConstMemberIterator itr = m_document.FindMember(keywd);
			if (itr != m_document.MemberEnd()) {
				if (itr->value.IsArray() == true) {
					Value &tmpVal = m_document[keywd];
					Value &oneAry = tmpVal[index];
					return std::shared_ptr<class jsonParser> (new jsonParser(oneAry));
				} else {
					printf("could not create.\n");
				}
			} else {
				printf("not found.\n");
			}
			
			return (nullptr);
		}
		
		bool getAryNum(const char *keywd, int &aryNum)
		{
			bool result = false;
			aryNum = -1;
			if (m_document.IsObject() == true) {
				rapidjson::Value::ConstMemberIterator itr = m_document.FindMember(keywd);
				if (itr != m_document.MemberEnd()) {
					if (itr->value.IsArray() == true) {
						aryNum = itr->value.Size();
						result = true;
					} else {
						printf("not ary\n");
					}
				} else {
					printf("invalid key\n");
				}
			} else {
				printf("not obj\n");
			}
			
			return (result);
		}
		
		bool getVale(const char *keywd, int &retVal)
		{
			bool result = false;
			if (m_document.IsObject() == true) {
				rapidjson::Value::ConstMemberIterator itr = m_document.FindMember(keywd);
				if (itr != m_document.MemberEnd()) {
					if (itr->value.IsInt() == true) {
						retVal = itr->value.GetInt();
						result = true;
					} else {
						printf("not int\n");
						
					}
				} else {
					printf("getVale:Invalid key\n");
				}
			} else {
				printf("getVale:Invalid object\n");
			}
			
			return (result);
		}
		
		bool getVale(const char *keywd, std::string &retVal)
		{
			bool result = false;
			if (m_document.IsObject() == true) {
				rapidjson::Value::ConstMemberIterator itr = m_document.FindMember(keywd);
				if (itr != m_document.MemberEnd()) {
					if (itr->value.IsString() == true) {
						retVal = itr->value.GetString();
						result = true;
					} else {
						printf("not string\n");
					}
				} else {
					printf("getVale:Invalid key\n");
				}
			} else {
				printf("getVale:Invalid object\n");
			}
			
			return (result);
		}		
	private:
		Document m_document;
		
};

int main(void)
{
	const char jsonStr[] = "{ \"name\" : \"name01\", \"list\" : [ { \"item1\" : 1, \"name\" : \"name01\", \"age\" : 100, \"list2\":[{\"item2\":100, \"name2\": \"list2-name01\"}, {\"item2\":101, \"name2\": \"list2-name02\"}, {\"item2\":102, \"name2\": \"list2-name03\"}]}, { \"item1\" : 2, \"name\" : \"name02\", \"age\" : 200, \"list2\":[{\"item2\":200, \"name2\": \"list2-name10\"}, {\"item2\":201, \"name2\": \"list2-name11\"}, {\"item2\":202, \"name2\": \"list2-name12\"}]}] } ";
	
	std::shared_ptr<jsonParser> jParser = jsonParser::creatJsonParser(jsonStr);
	int aryNum;
	jParser->getAryNum("list", aryNum);
	printf("aryNum list=%d\n", aryNum);
	printf("------------------------\n");
	
//	class jsonParser *ararser = jParser->creatAryObj("list", 0);
	std::shared_ptr<jsonParser>ararser = jParser->creatAryObj("list", 0);
	ararser->getAryNum("list2", aryNum);
	printf("aryNum list2=%d\n", aryNum);
	
	
//	class jsonParser *ararser2 = ararser->creatAryObj("list2", 0);
	std::shared_ptr<jsonParser>ararser2 = ararser->creatAryObj("list2", 0);

	int retVal;
	ararser2->getVale("item2", retVal);
	printf("item2-0:val=%d\n", retVal);

//	class jsonParser *ararser3 = ararser->creatAryObj("list2", 1);
	std::shared_ptr<jsonParser>ararser3 = ararser->creatAryObj("list2", 1);
	ararser3->getVale("item2", retVal);
	printf("item2-1:val=%d\n", retVal);

	std::string retValStr;
	ararser3->getVale("name2", retValStr);
	printf("name2-1:val=%s\n", retValStr.c_str());
	
	return (0);
	
}

