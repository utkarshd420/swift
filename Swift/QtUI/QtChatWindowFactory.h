/*
 * Copyright (c) 2010 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <Swift/Controllers/UIInterfaces/ChatWindowFactory.h>

#include <QObject>
#include <QSplitter>

#include <Swiften/JID/JID.h>
#include <Swift/QtUI/QtSettingsProvider.h>

namespace Swift {
	class QtChatTabs;
	class QtChatTheme;
	class UIEventStream;
	class QtUIPreferences;
	class QtSingleWindow;
	class QtChatWindowFactory : public QObject, public ChatWindowFactory {
		Q_OBJECT
		public:
			QtChatWindowFactory(QtSingleWindow* splitter, SettingsProvider* settings, QtSettingsProvider* qtSettings, QtChatTabs* tabs, const QString& themePath);
			~QtChatWindowFactory();
			ChatWindow* createChatWindow(const JID &contact, UIEventStream* eventStream);
		signals:
			void changeSplitterState(QByteArray);
		private slots:
			void handleWindowGeometryChanged();
			void handleSplitterMoved();
		private:
			QString themePath_;
			SettingsProvider* settings_;
			QtSettingsProvider* qtOnlySettings_;
			QtChatTabs* tabs_;
			QtChatTheme* theme_;
	};
}

