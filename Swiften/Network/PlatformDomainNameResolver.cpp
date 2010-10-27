/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#include "Swiften/Network/PlatformDomainNameResolver.h"

// Putting this early on, because some system types conflict with thread
#include "Swiften/Network/PlatformDomainNameServiceQuery.h"

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <algorithm>

#include "Swiften/Base/String.h"
#include "Swiften/Network/HostAddress.h"
#include "Swiften/EventLoop/EventLoop.h"
#include "Swiften/Network/HostAddressPort.h"
#include "Swiften/Network/DomainNameAddressQuery.h"

using namespace Swift;

namespace {
	struct AddressQuery : public DomainNameAddressQuery, public boost::enable_shared_from_this<AddressQuery>, public EventOwner {
		AddressQuery(const String& host, EventLoop* eventLoop) : hostname(host), eventLoop(eventLoop), thread(NULL), safeToJoin(false) {}

		~AddressQuery() {
			if (safeToJoin) {
				thread->join();
			}
			else {
				// FIXME: UGLYYYYY
			}
			delete thread;
		}

		void run() {
			safeToJoin = false;
			thread = new boost::thread(boost::bind(&AddressQuery::doRun, shared_from_this()));
		}
		
		void doRun() {
			//std::cout << "PlatformDomainNameResolver::doRun()" << std::endl;
			boost::asio::ip::tcp::resolver resolver(ioService);
			boost::asio::ip::tcp::resolver::query query(hostname.getUTF8String(), "5222");
			try {
				//std::cout << "PlatformDomainNameResolver::doRun(): Resolving" << std::endl;
				boost::asio::ip::tcp::resolver::iterator endpointIterator = resolver.resolve(query);
				//std::cout << "PlatformDomainNameResolver::doRun(): Resolved" << std::endl;
				if (endpointIterator == boost::asio::ip::tcp::resolver::iterator()) {
					//std::cout << "PlatformDomainNameResolver::doRun(): Error 1" << std::endl;
					emitError();
				}
				else {
					std::vector<HostAddress> results;
					for ( ; endpointIterator != boost::asio::ip::tcp::resolver::iterator(); ++endpointIterator) {
						boost::asio::ip::address address = (*endpointIterator).endpoint().address();
						results.push_back(address.is_v4() ? HostAddress(&address.to_v4().to_bytes()[0], 4) : HostAddress(&address.to_v6().to_bytes()[0], 16));
					}

					//std::cout << "PlatformDomainNameResolver::doRun(): Success" << std::endl;
					eventLoop->postEvent(
							boost::bind(boost::ref(onResult), results, boost::optional<DomainNameResolveError>()), 
							shared_from_this());
				}
			}
			catch (...) {
				//std::cout << "PlatformDomainNameResolver::doRun(): Error 2" << std::endl;
				emitError();
			}
			safeToJoin = true;
		}

		void emitError() {
			eventLoop->postEvent(boost::bind(boost::ref(onResult), std::vector<HostAddress>(), boost::optional<DomainNameResolveError>(DomainNameResolveError())), shared_from_this());
		}

		boost::asio::io_service ioService;
		String hostname;
		EventLoop* eventLoop;
		boost::thread* thread;
		bool safeToJoin;
	};

}

namespace Swift {

PlatformDomainNameResolver::PlatformDomainNameResolver(EventLoop* eventLoop) : eventLoop(eventLoop) {
}

boost::shared_ptr<DomainNameServiceQuery> PlatformDomainNameResolver::createServiceQuery(const String& name) {
	return boost::shared_ptr<DomainNameServiceQuery>(new PlatformDomainNameServiceQuery(getNormalized(name), eventLoop));
}

boost::shared_ptr<DomainNameAddressQuery> PlatformDomainNameResolver::createAddressQuery(const String& name) {
	return boost::shared_ptr<DomainNameAddressQuery>(new AddressQuery(getNormalized(name), eventLoop));
}

}
