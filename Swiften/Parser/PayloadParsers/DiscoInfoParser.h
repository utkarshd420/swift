/*
 * Copyright (c) 2010 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/API.h>
#include <Swiften/Elements/DiscoInfo.h>
#include <Swiften/Parser/GenericPayloadParser.h>

namespace Swift {
	class SWIFTEN_API DiscoInfoParser : public GenericPayloadParser<DiscoInfo> {
		public:
			DiscoInfoParser();

			virtual void handleStartElement(const std::string& element, const std::string&, const AttributeMap& attributes);
			virtual void handleEndElement(const std::string& element, const std::string&);
			virtual void handleCharacterData(const std::string& data);

		private:
			enum Level { 
				TopLevel = 0, 
				PayloadLevel = 1
			};
			int level_;
			FormParser* formParser_;
	};
}
