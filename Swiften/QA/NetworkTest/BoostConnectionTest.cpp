#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <boost/shared_ptr.hpp>

#include "Swiften/Base/String.h"
#include "Swiften/Base/sleep.h"
#include "Swiften/Network/BoostConnection.h"
#include "Swiften/Network/HostAddress.h"
#include "Swiften/Network/HostAddressPort.h"
#include "Swiften/Network/BoostIOServiceThread.h"
#include "Swiften/EventLoop/DummyEventLoop.h"

const unsigned char* address = reinterpret_cast<const unsigned char*>("\x41\x63\xde\x89");

using namespace Swift;

class BoostConnectionTest : public CppUnit::TestFixture {
		CPPUNIT_TEST_SUITE(BoostConnectionTest);
		CPPUNIT_TEST(testDestructor);
		CPPUNIT_TEST(testDestructor_PendingEvents);
		CPPUNIT_TEST(testWrite);
		CPPUNIT_TEST_SUITE_END();

	public:
		BoostConnectionTest() {}

		void setUp() {
			boostIOServiceThread_ = new BoostIOServiceThread();
			eventLoop_ = new DummyEventLoop();
			disconnected = false;
		}

		void tearDown() {
			delete eventLoop_;
			delete boostIOServiceThread_;
		}

		void testDestructor() {
			{
				boost::shared_ptr<BoostConnection> testling(new BoostConnection(&boostIOServiceThread_->getIOService()));
				testling->connect(HostAddressPort(HostAddress(address, 4), 5222));
			}
		}

		void testDestructor_PendingEvents() {
			{
				boost::shared_ptr<BoostConnection> testling(new BoostConnection(&boostIOServiceThread_->getIOService()));
				testling->connect(HostAddressPort(HostAddress(address, 4), 5222));
				while (!eventLoop_->hasEvents()) {
					Swift::sleep(10);
				}
			}
			eventLoop_->processEvents();
		}

		void testWrite() {
			boost::shared_ptr<BoostConnection> testling(new BoostConnection(&boostIOServiceThread_->getIOService()));
			testling->onConnectFinished.connect(boost::bind(&BoostConnectionTest::doWrite, this, testling.get()));
			testling->onDataRead.connect(boost::bind(&BoostConnectionTest::handleDataRead, this, _1));
			testling->onDisconnected.connect(boost::bind(&BoostConnectionTest::handleDisconnected, this));
			testling->connect(HostAddressPort(HostAddress("65.99.222.137"), 5222));
			while (receivedData.isEmpty()) {
				Swift::sleep(10);
				eventLoop_->processEvents();
			}
			testling->disconnect();
		}

		void doWrite(BoostConnection* connection) {
			connection->write(ByteArray("<stream:stream>"));
		}

		void handleDataRead(const ByteArray& data) {
			receivedData += data;
		}

		void handleDisconnected() {
			disconnected = true;
		}

	private:
		BoostIOServiceThread* boostIOServiceThread_;
		DummyEventLoop* eventLoop_;
		ByteArray receivedData;
		bool disconnected;
};

CPPUNIT_TEST_SUITE_REGISTRATION(BoostConnectionTest);
