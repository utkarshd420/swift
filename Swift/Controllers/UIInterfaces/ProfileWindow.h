/*
 * Copyright (c) 2010 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swiften/Base/boost_bsignals.h>
#include <boost/shared_ptr.hpp>

#include <Swiften/Elements/VCard.h>

namespace Swift {
	class JID;

	class ProfileWindow {
		public:
			virtual ~ProfileWindow() {}

			virtual void setJID(const JID& jid) = 0;
			virtual void setVCard(VCard::ref vcard) = 0;

			virtual void setEnabled(bool b) = 0;
			virtual void setProcessing(bool b) = 0;
			virtual void setError(const std::string&) = 0;
			virtual void setEditable(bool b) = 0;

			virtual void show() = 0;
			virtual void hide() = 0;

			boost::signal<void (VCard::ref)> onVCardChangeRequest;
			boost::signal<void (const JID&)> onWindowAboutToBeClosed;
	};
}
