/*
 * Copyright (c) 2010 Kevin Smith
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include "Swiften/Base/boost_bsignals.h"
#include <boost/shared_ptr.hpp>
#include <vector>

#include "Swiften/Network/BoostIOServiceThread.h"
#include "Swiften/Network/BoostTimerFactory.h"
#include "SwifTools/Idle/PlatformIdleQuerier.h"
#include "SwifTools/Idle/ActualIdleDetector.h"
#include "Swiften/Base/String.h"
#include "Swiften/Client/ClientError.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Elements/VCard.h"
#include "Swiften/Elements/DiscoInfo.h"
#include "Swiften/Elements/ErrorPayload.h"
#include "Swiften/Elements/Presence.h"
#include "Swift/Controllers/Settings/SettingsProvider.h"
#include "Swift/Controllers/ProfileSettingsProvider.h"
#include "Swiften/Elements/CapsInfo.h"
#include "Swift/Controllers/XMPPEvents/ErrorEvent.h"
#include "Swift/Controllers/UIEvents/UIEvent.h"

namespace Swift {
	class EventLoop;
	class Client;
	class ChatWindowFactory;
	class ChatController;
	class ChatsManager;
	class ChatListWindowFactory;
	class EventController;
	class MainWindowFactory;
	class MainWindow;
	class RosterController;
	class DiscoInfoResponder;
	class LoginWindow;
	class EventLoop;
	class LoginWindowFactory;
	class MUCController;
	class Notifier;
	class TogglableNotifier;
	class PresenceNotifier;
	class EventNotifier;
	class SystemTray;
	class SystemTrayController;
	class SoundEventController;
	class SoundPlayer;
	class XMLConsoleController;
	class UIEventStream;
	class XMLConsoleWidgetFactory;
	class EventWindowFactory;
	class EventWindowController;
	class MUCSearchController;
	class MUCSearchWindowFactory;
	class StatusTracker;
	class Dock;
	class Storages;
	class StoragesFactory;

	class MainController {
		public:
			MainController(
					EventLoop* eventLoop,
					ChatWindowFactory* chatWindowFactory,
					MainWindowFactory *mainWindowFactory,
					LoginWindowFactory *loginWindowFactory,
					EventWindowFactory* eventWindowFactory,
					SettingsProvider *settings,
					SystemTray* systemTray,
					SoundPlayer* soundPlayer,
					XMLConsoleWidgetFactory* xmlConsoleWidgetFactory,
					ChatListWindowFactory* chatListWindowFactory_,
					MUCSearchWindowFactory* mucSearchWindowFactory,
					StoragesFactory* storagesFactory,
					Dock* dock,
					Notifier* notifier,
					bool useDelayForLatency);
			~MainController();


		private:
			void resetClient();
			void handleConnected();
			void handleLoginRequest(const String& username, const String& password, const String& certificateFile, bool remember, bool loginAutomatically);
			void handleCancelLoginRequest();
			void handleChangeStatusRequest(StatusShow::Type show, const String &statusText);
			void handleError(const ClientError& error);
			void handleServerDiscoInfoResponse(boost::shared_ptr<DiscoInfo>, const boost::optional<ErrorPayload>&);
			void handleEventQueueLengthChange(int count);
			void handleVCardReceived(const JID& j, VCard::ref vCard);
			void handleUIEvent(boost::shared_ptr<UIEvent> event);
			void sendPresence(boost::shared_ptr<Presence> presence);
			void handleInputIdleChanged(bool);
			void logout();
			void signOut();
			void setReconnectTimer();
			void resetPendingReconnects();
			void resetCurrentError();
	
			void performLoginFromCachedCredentials();
			void reconnectAfterError();
			void setManagersOffline();
			void handleNotificationClicked(const JID& jid);

		private:
			EventLoop* eventLoop_;
			BoostIOServiceThread boostIOServiceThread_;
			BoostTimerFactory timerFactory_;
			PlatformIdleQuerier idleQuerier_;
			ActualIdleDetector idleDetector_;
			StoragesFactory* storagesFactory_;
			Storages* storages_;
			Client* client_;
			ChatWindowFactory* chatWindowFactory_;
			MainWindowFactory* mainWindowFactory_;
			LoginWindowFactory* loginWindowFactory_;
			EventWindowFactory* eventWindowFactory_;
			SettingsProvider *settings_;
			ProfileSettingsProvider* profileSettings_;
			Dock* dock_;
			TogglableNotifier* notifier_;
			PresenceNotifier* presenceNotifier_;
			EventNotifier* eventNotifier_;
			RosterController* rosterController_;
			EventController* eventController_;
			EventWindowController* eventWindowController_;
			LoginWindow* loginWindow_;
			DiscoInfoResponder* discoResponder_;
			UIEventStream* uiEventStream_;
			XMLConsoleController* xmlConsoleController_;
			ChatsManager* chatsManager_;
			boost::shared_ptr<CapsInfo> capsInfo_;
			JID jid_;
			JID boundJID_;
			SystemTrayController* systemTrayController_;
			SoundEventController* soundEventController_;
			String vCardPhotoHash_;
			String password_;
			String certificateFile_;
			ChatListWindowFactory* chatListWindowFactory_;
			boost::shared_ptr<ErrorEvent> lastDisconnectError_;
			bool useDelayForLatency_;
			MUCSearchController* mucSearchController_;
			MUCSearchWindowFactory* mucSearchWindowFactory_;
			int timeBeforeNextReconnect_;
			Timer::ref reconnectTimer_;
			StatusTracker* statusTracker_;
			bool myStatusLooksOnline_;
	};
}
