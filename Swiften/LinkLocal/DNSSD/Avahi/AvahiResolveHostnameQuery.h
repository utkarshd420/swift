/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include "Swiften/Base/String.h"
#include "Swiften/LinkLocal/DNSSD/Avahi/AvahiQuery.h"
#include "Swiften/LinkLocal/DNSSD/DNSSDResolveHostnameQuery.h"
#include "Swiften/EventLoop/EventLoop.h"
#include "Swiften/Network/HostAddress.h"

#include <netinet/in.h>

namespace Swift {
	class AvahiQuerier;

	class AvahiResolveHostnameQuery : public DNSSDResolveHostnameQuery, public AvahiQuery {
		public: 
			AvahiResolveHostnameQuery(const String& hostname, int, boost::shared_ptr<AvahiQuerier> querier) : AvahiQuery(querier), hostname(hostname) {
				std::cout << "Resolving hostname " << hostname << std::endl;
			}

			void run() {
					eventLoop->postEvent(boost::bind(boost::ref(onHostnameResolved), boost::optional<HostAddress>(HostAddress(hostname))), shared_from_this());
			}

			void finish() {
			}

		private:
			HostAddress hostAddress;
			String hostname;
	};
}
