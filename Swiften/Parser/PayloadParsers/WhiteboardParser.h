/*
 * Copyright (c) 2012 Mateusz Piękos
 * Licensed under the simplified BSD license.
 * See Documentation/Licenses/BSD-simplified.txt for more information.
 */

#pragma once

#include <Swiften/Elements/WhiteboardPayload.h>
#include <Swiften/Parser/GenericPayloadParser.h>
#include <Swiften/Elements/Whiteboard/WhiteboardElement.h>
#include <Swiften/Elements/Whiteboard/WhiteboardOperation.h>

namespace Swift {
	class WhiteboardParser : public Swift::GenericPayloadParser<WhiteboardPayload> {
	public:
		WhiteboardParser();

		virtual void handleStartElement(const std::string& element, const std::string&, const AttributeMap& attributes);
		virtual void handleEndElement(const std::string& element, const std::string&);
		virtual void handleCharacterData(const std::string& data);

	private:
		WhiteboardPayload::Type stringToType(const std::string& type) const;
		int opacityToAlpha(std::string opacity) const;

	private:
		bool actualIsText;
		int level_;
		std::string data_;
		WhiteboardElement::ref wbElement;
		WhiteboardOperation::ref operation;
	};
}
