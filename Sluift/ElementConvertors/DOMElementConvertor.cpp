/*
 * Copyright (c) 2013 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include <Sluift/ElementConvertors/DOMElementConvertor.h>

#include <iostream>
#include <boost/smart_ptr/make_shared.hpp>
#include <lua.hpp>

#include <Swiften/Base/foreach.h>
#include <Swiften/Elements/RawXMLPayload.h>
#include <Swiften/Serializer/PayloadSerializer.h>
#include <Sluift/Lua/Check.h>
#include <Sluift/Lua/LuaUtils.h>
#include <Swiften/Parser/XMLParserClient.h>
#include <Swiften/Parser/XMLParser.h>
#include <Swiften/Parser/AttributeMap.h>
#include <Swiften/Parser/Attribute.h>
#include <Swiften/Serializer/XML/XMLElement.h>
#include <Swiften/Serializer/XML/XMLTextNode.h>
#include <Swiften/Serializer/XML/XMLRawTextNode.h>
#include <Sluift/Lua/Debug.h>

using namespace Swift;

namespace {
	class ParserClient : public XMLParserClient {
		public:
			ParserClient(lua_State* L) : L(L), currentIndex(1) {
			}

			virtual void handleStartElement(
					const std::string& element, const std::string& ns, 
					const AttributeMap& attributes) SWIFTEN_OVERRIDE {
				lua_pushnumber(L, currentIndex);
				lua_newtable(L);
				lua_pushstring(L, element.c_str());
				lua_setfield(L, -2, "tag");
				if (!ns.empty()) {
					lua_pushstring(L, ns.c_str());
					lua_setfield(L, -2, "ns");
				}
				if (!attributes.getEntries().empty()) {
					lua_newtable(L);
					int i = 1;
					foreach(const AttributeMap::Entry& entry, attributes.getEntries()) {
						lua_pushnumber(L, i);
						lua_newtable(L);
						lua_pushstring(L, entry.getAttribute().getName().c_str());
						lua_setfield(L, -2, "name");
						if (!entry.getAttribute().getNamespace().empty()) {
							lua_pushstring(L, entry.getAttribute().getNamespace().c_str());
							lua_setfield(L, -2, "ns");
						}
						lua_pushstring(L, entry.getValue().c_str());
						lua_setfield(L, -2, "value");
						lua_settable(L, -3);
						++i;
					}
					lua_setfield(L, -2, "attributes");
				}

				indexStack.push_back(currentIndex);
				currentIndex = 1;
				lua_newtable(L);
			}

			virtual void handleEndElement(
					const std::string&, const std::string&) SWIFTEN_OVERRIDE {
				lua_setfield(L, -2, "children");
				lua_settable(L, -3);
				currentIndex = indexStack.back();
				indexStack.pop_back();
				currentIndex++;
			}

			virtual void handleCharacterData(const std::string& data) SWIFTEN_OVERRIDE {
				lua_pushnumber(L, currentIndex);
				lua_pushstring(L, data.c_str());
				lua_settable(L, -3);
				currentIndex++;
			}

		private:
			lua_State* L;
			std::vector<int> indexStack;
			int currentIndex;
	};

	std::string serializeElement(lua_State* L) {
		std::string tag;
		lua_getfield(L, -1, "tag");
		if (lua_isstring(L, -1)) {
			tag = lua_tostring(L, -1);
		}
		lua_pop(L, 1);

		std::string ns;
		lua_getfield(L, -1, "ns");
		if (lua_isstring(L, -1)) {
			ns = lua_tostring(L, -1);
		}
		lua_pop(L, 1);

		XMLElement element(tag, ns);

		lua_getfield(L, -1, "attributes");
		if (lua_istable(L, -1)) {
			int index = Lua::absoluteOffset(L, -1);
			for (lua_pushnil(L); lua_next(L, index) != 0; ) {
				if (lua_istable(L, -1)) {
					std::string attributeName;
					lua_getfield(L, -1, "name");
					if (lua_isstring(L, -1)) {
						attributeName = lua_tostring(L, -1);
					}
					lua_pop(L, 1);

					std::string attributeValue;
					lua_getfield(L, -1, "value");
					if (lua_isstring(L, -1)) {
						attributeValue = lua_tostring(L, -1);
					}
					lua_pop(L, 1);

					if (!attributeName.empty()) {
						element.setAttribute(attributeName, attributeValue);
					}
				}
				lua_pop(L, 1); // value
			}
		}
		lua_pop(L, 1); // children

		lua_getfield(L, -1, "children");
		if (lua_istable(L, -1)) {
			int index = Lua::absoluteOffset(L, -1);
			for (lua_pushnil(L); lua_next(L, index) != 0; ) {
				if (lua_isstring(L, -1)) {
					element.addNode(boost::make_shared<XMLTextNode>(lua_tostring(L, -1)));
				}
				else if (lua_istable(L, -1)) {
					element.addNode(boost::make_shared<XMLRawTextNode>(serializeElement(L)));
				}
				lua_pop(L, 1); // value
			}
		}
		lua_pop(L, 1); // children

		return element.serialize();
	}
}

DOMElementConvertor::DOMElementConvertor() {
}

DOMElementConvertor::~DOMElementConvertor() {
}

boost::shared_ptr<Payload> DOMElementConvertor::convertFromLua(lua_State* L, int index, const std::string& type) {
	if (!lua_istable(L, index) || type != "dom") {
		return boost::shared_ptr<Payload>();
	}
	return boost::make_shared<RawXMLPayload>(serializeElement(L).c_str());
}

boost::optional<std::string> DOMElementConvertor::convertToLua(
		lua_State* L, boost::shared_ptr<Payload> payload) {
	// Serialize payload to XML
	PayloadSerializer* serializer = serializers.getPayloadSerializer(payload);
	assert(serializer);
	std::string serializedPayload = serializer->serialize(payload);

	lua_newtable(L);

	// Parse the payload again
	ParserClient parserClient(L);
	boost::shared_ptr<XMLParser> parser(parsers.createXMLParser(&parserClient));
	bool result = parser->parse(serializedPayload);
	assert(result);

	// There can only be one element, so stripping the list
	lua_pushnil(L);
	lua_next(L, -2);
	Lua::registerTableToString(L, -1);

	lua_replace(L, -3);
	lua_settop(L, -2);

	return std::string("dom");
}