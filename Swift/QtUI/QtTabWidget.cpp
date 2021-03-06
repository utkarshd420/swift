/*
 * Copyright (c) 2010 Isode Limited.
 * All rights reserved.
 * See the COPYING file for more information.
 */

#include "QtTabWidget.h"

namespace Swift {

QtTabWidget::QtTabWidget(QWidget* parent) : QTabWidget(parent) {

}

QtTabWidget::~QtTabWidget() {

}

QTabBar* QtTabWidget::tabBar() {
	return QTabWidget::tabBar();
}

void QtTabWidget::setTabBar(QTabBar* tabBar) {
	QTabWidget::setTabBar(tabBar);
}
}
