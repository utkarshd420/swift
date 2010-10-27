/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <boost/asio.hpp>

#include "Swiften/Network/ConnectionFactory.h"
#include "Swiften/Network/BoostConnection.h"

namespace Swift {
	class BoostConnection;

	class BoostConnectionFactory : public ConnectionFactory {
		public:
			BoostConnectionFactory(boost::asio::io_service*, EventLoop* eventLoop);

			virtual boost::shared_ptr<Connection> createConnection();

		private:
			boost::asio::io_service* ioService;
			EventLoop* eventLoop;
	};
}
