/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <boost/shared_ptr.hpp>

#include "Swiften/StreamStack/LowLayer.h"
#include "Swiften/Network/Connection.h"

namespace Swift {
	class ConnectionLayer : public LowLayer {
		public:
			ConnectionLayer(boost::shared_ptr<Connection> connection);
			~ConnectionLayer();

			void writeData(const ByteArray& data) {
				connection->write(data);
			}

		private:
			boost::shared_ptr<Connection> connection;
	};
}
