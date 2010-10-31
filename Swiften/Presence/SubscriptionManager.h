/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <map>

#include "Swiften/Base/String.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Base/boost_bsignals.h"
#include "Swiften/Elements/Presence.h"

namespace Swift {
	class StanzaChannel;

	class SubscriptionManager {
		public:
			SubscriptionManager(StanzaChannel* stanzaChannel);
			~SubscriptionManager();

			void cancelSubscription(const JID& jid);
			void confirmSubscription(const JID& jid);
			void requestSubscription(const JID& jid);

			/**
			 * This signal is emitted when a presence subscription request is received.
			 */
			boost::signal<void (const JID&, const String&)> onPresenceSubscriptionRequest;

			boost::signal<void (const JID&, const String&)> onPresenceSubscriptionRevoked;

		private:
			void handleIncomingPresence(Presence::ref presence);

		private:
			StanzaChannel* stanzaChannel;
	};
}
