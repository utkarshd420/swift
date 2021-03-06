/*
 * Copyright (c) 2010 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#pragma once

#include <QLabel>

namespace Swift {
	class QtElidingLabel : public QLabel {
		Q_OBJECT
	public:
		QtElidingLabel(QWidget* parent = NULL, Qt::WindowFlags f = 0);
		QtElidingLabel(const QString &text, QWidget* parent = NULL, Qt::WindowFlags f = 0);
		void setText(const QString& text);
		virtual ~QtElidingLabel();
		
		virtual void paintEvent(QPaintEvent* event);

	private:
		void setSizes();
		bool dirty_;
		QString fullText_;
		QRect lastRect_;
	};
}
