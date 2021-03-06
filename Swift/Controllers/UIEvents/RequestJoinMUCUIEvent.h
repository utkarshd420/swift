/*
 * Copyright (c) 2010 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <string>

#include <Swift/Controllers/UIEvents/UIEvent.h>
#include <Swiften/JID/JID.h>

namespace Swift {
	class RequestJoinMUCUIEvent : public UIEvent {
		public:
			typedef boost::shared_ptr<RequestJoinMUCUIEvent> ref;

			RequestJoinMUCUIEvent(const JID& room = JID()) : room(room) {
			}

			const JID& getRoom() const {
				return room;
			}

		private:
			JID room;
	};
}
