/*
 * Copyright (c) 2011 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#include <Swiften/Client/ClientBlockListManager.h>

#include <boost/bind.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <cassert>

#include <Swiften/Client/BlockListImpl.h>

using namespace Swift;

namespace {
	class BlockResponder : public SetResponder<BlockPayload> {
		public:
			BlockResponder(boost::shared_ptr<BlockListImpl> blockList, IQRouter* iqRouter) : SetResponder<BlockPayload>(iqRouter), blockList(blockList) {
			}

			virtual bool handleSetRequest(const JID& from, const JID&, const std::string& id, boost::shared_ptr<BlockPayload> payload) {
				if (getIQRouter()->isAccountJID(from)) {
						if (payload) {
							blockList->addItems(payload->getItems());
						}
						sendResponse(from, id, boost::shared_ptr<BlockPayload>());
				}
				else {
					sendError(from, id, ErrorPayload::NotAuthorized, ErrorPayload::Cancel);
				}
				return true;
			}

		private:
			boost::shared_ptr<BlockListImpl> blockList;
	};

	class UnblockResponder : public SetResponder<UnblockPayload> {
		public:
			UnblockResponder(boost::shared_ptr<BlockListImpl> blockList, IQRouter* iqRouter) : SetResponder<UnblockPayload>(iqRouter), blockList(blockList) {
			}

			virtual bool handleSetRequest(const JID& from, const JID&, const std::string& id, boost::shared_ptr<UnblockPayload> payload) {
				if (getIQRouter()->isAccountJID(from)) {
					if (payload) {
						if (payload->getItems().empty()) {
							blockList->removeAllItems();
						}
						else {
							blockList->removeItems(payload->getItems());
						}
					}
					sendResponse(from, id, boost::shared_ptr<UnblockPayload>());
				}
				else {
					sendError(from, id, ErrorPayload::NotAuthorized, ErrorPayload::Cancel);
				}
				return true;
			}

		private:
			boost::shared_ptr<BlockListImpl> blockList;
	};
}

ClientBlockListManager::ClientBlockListManager(IQRouter* iqRouter) : iqRouter(iqRouter) {

}

ClientBlockListManager::~ClientBlockListManager() {
	if (blockList && blockList->getState() == BlockList::Available) {
		unblockResponder->stop();
		blockResponder->stop();
	}
}

boost::shared_ptr<BlockList> ClientBlockListManager::getBlockList() {
	if (!blockList) {
		blockList = boost::make_shared<BlockListImpl>();
		blockList->setState(BlockList::Init);
	}
	return blockList;
}

boost::shared_ptr<BlockList> ClientBlockListManager::requestBlockList() {
	if (!blockList) {
		blockList = boost::make_shared<BlockListImpl>();
	}
	blockList->setState(BlockList::Requesting);
	boost::shared_ptr<GenericRequest<BlockListPayload> > getRequest = boost::make_shared< GenericRequest<BlockListPayload> >(IQ::Get, JID(), boost::make_shared<BlockListPayload>(), iqRouter);
	getRequest->onResponse.connect(boost::bind(&ClientBlockListManager::handleBlockListReceived, this, _1, _2));
	getRequest->send();
	return blockList;
}

GenericRequest<BlockPayload>::ref ClientBlockListManager::createBlockJIDRequest(const JID& jid) {
	return createBlockJIDsRequest(std::vector<JID>(1, jid));
}

GenericRequest<BlockPayload>::ref ClientBlockListManager::createBlockJIDsRequest(const std::vector<JID>& jids) {
	boost::shared_ptr<BlockPayload> payload = boost::make_shared<BlockPayload>(jids);
	return boost::make_shared< GenericRequest<BlockPayload> >(IQ::Set, JID(), payload, iqRouter);
}

GenericRequest<UnblockPayload>::ref ClientBlockListManager::createUnblockJIDRequest(const JID& jid) {
	return createUnblockJIDsRequest(std::vector<JID>(1, jid));
}

GenericRequest<UnblockPayload>::ref ClientBlockListManager::createUnblockJIDsRequest(const std::vector<JID>& jids) {
	boost::shared_ptr<UnblockPayload> payload = boost::make_shared<UnblockPayload>(jids);
	return boost::make_shared< GenericRequest<UnblockPayload> >(IQ::Set, JID(), payload, iqRouter);
}

GenericRequest<UnblockPayload>::ref ClientBlockListManager::createUnblockAllRequest() {
	return createUnblockJIDsRequest(std::vector<JID>());
}


void ClientBlockListManager::handleBlockListReceived(boost::shared_ptr<BlockListPayload> payload, ErrorPayload::ref error) {
	if (error || !payload) {
		blockList->setState(BlockList::Error);
	}
	else {
		blockList->setItems(payload->getItems());
		blockList->setState(BlockList::Available);
		if (!blockResponder) {
			blockResponder = boost::make_shared<BlockResponder>(blockList, iqRouter);
			blockResponder->start();
		}
		if (!unblockResponder) {
			unblockResponder = boost::make_shared<UnblockResponder>(blockList, iqRouter);
			unblockResponder->start();
		}
	}
}

