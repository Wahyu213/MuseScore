/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "dockbase.h"

#include <QRect>
#include <QTimer>
#include <QAction>

#include "log.h"

#include "thirdparty/KDDockWidgets/src/DockWidgetQuick.h"
#include "thirdparty/KDDockWidgets/src/private/Frame_p.h"
#include "thirdparty/KDDockWidgets/src/private/FloatingWindow_p.h"

namespace mu::dock {
static QSize adjustSizeByContraints(const QSize& size, const QSize& min, const QSize& max)
{
    return size.expandedTo(min).boundedTo(max);
}

class DockWidgetImpl : public KDDockWidgets::DockWidgetQuick
{
public:
    DockWidgetImpl(const QString& uniqueName)
        : KDDockWidgets::DockWidgetQuick(uniqueName)
    {
        setObjectName(uniqueName);
    }

    QSize minimumSize() const override
    {
        return DockWidgetBase::minimumSize();
    }

    QSize maximumSize() const override
    {
        return DockWidgetBase::maximumSize();
    }
};
}

using namespace mu::dock;

DockBase::DockBase(QQuickItem* parent)
    : QQuickItem(parent), m_resizable(true), m_separatorsVisible(true)
{
}

QString DockBase::title() const
{
    return m_title;
}

int DockBase::minimumWidth() const
{
    return m_minimumWidth;
}

int DockBase::minimumHeight() const
{
    return m_minimumHeight;
}

int DockBase::maximumWidth() const
{
    return m_maximumWidth;
}

int DockBase::maximumHeight() const
{
    return m_maximumHeight;
}

int DockBase::contentWidth() const
{
    return m_contentWidth;
}

int DockBase::contentHeight() const
{
    return m_contentHeight;
}

QSize DockBase::preferredSize() const
{
    return QSize(width(), height());
}

int DockBase::locationProperty() const
{
    return m_location;
}

Location DockBase::location() const
{
    return static_cast<Location>(m_location);
}

QVariantList DockBase::dropDestinationsProperty() const
{
    return m_dropDestinations;
}

QList<DropDestination> DockBase::dropDestinations() const
{
    QList<DropDestination> result;

    for (const QVariant& obj : m_dropDestinations) {
        QVariantMap map = obj.toMap();

        DropDestination destination;
        destination.dock = map["dock"].value<DockBase*>();

        if (map.contains("dropLocation")) {
            destination.dropLocation = static_cast<Location>(map["dropLocation"].toInt());
        } else {
            destination.dropLocation = Location::Left;
        }

        if (map.contains("dropDistance")) {
            destination.dropDistance = map["dropDistance"].toInt();
        }

        result << destination;
    }

    return result;
}

bool DockBase::persistent() const
{
    return m_persistent;
}

bool DockBase::resizable() const
{
    return m_resizable;
}

bool DockBase::separatorsVisible() const
{
    return m_separatorsVisible;
}

bool DockBase::floating() const
{
    return m_floating.value_or(false);
}

KDDockWidgets::DockWidgetQuick* DockBase::dockWidget() const
{
    return m_dockWidget;
}

void DockBase::setTitle(const QString& title)
{
    if (title == m_title) {
        return;
    }

    m_title = title;
    emit titleChanged();
}

void DockBase::setMinimumWidth(int width)
{
    if (width == minimumWidth()) {
        return;
    }

    m_minimumWidth = width;
    emit minimumSizeChanged();
}

void DockBase::setMinimumHeight(int height)
{
    if (height == minimumHeight()) {
        return;
    }

    m_minimumHeight = height;
    emit minimumSizeChanged();
}

void DockBase::setMaximumWidth(int width)
{
    if (width == maximumWidth()) {
        return;
    }

    m_maximumWidth = width;
    emit maximumSizeChanged();
}

void DockBase::setMaximumHeight(int height)
{
    if (height == maximumHeight()) {
        return;
    }

    m_maximumHeight = height;
    emit maximumSizeChanged();
}

void DockBase::setContentWidth(int width)
{
    if (m_contentWidth == width) {
        return;
    }

    m_contentWidth = width;
    emit contentSizeChanged();
}

void DockBase::setContentHeight(int height)
{
    if (m_contentHeight == height) {
        return;
    }

    m_contentHeight = height;
    emit contentSizeChanged();
}

void DockBase::setLocation(int location)
{
    if (location == m_location) {
        return;
    }

    m_location = location;
    emit locationChanged();
}

void DockBase::setDropDestinations(const QVariantList& destinations)
{
    if (m_dropDestinations == destinations) {
        return;
    }

    m_dropDestinations = destinations;
    emit dropDestinationsChanged();
}

void DockBase::setPersistent(bool persistent)
{
    if (persistent == m_persistent) {
        return;
    }

    m_persistent = persistent;
    emit persistentChanged();
}

void DockBase::setResizable(bool resizable)
{
    if (resizable == m_resizable) {
        return;
    }

    m_resizable = resizable;
    emit resizableChanged();
}

void DockBase::setSeparatorsVisible(bool visible)
{
    if (visible == m_separatorsVisible) {
        return;
    }

    m_separatorsVisible = visible;
    emit separatorsVisibleChanged();
}

void DockBase::setFloating(bool floating)
{
    IF_ASSERT_FAILED(m_dockWidget) {
        return;
    }

    m_dockWidget->setFloating(floating);
}

void DockBase::init()
{
    IF_ASSERT_FAILED(m_dockWidget) {
        return;
    }

    setVisible(m_dockWidget->isOpen());
}

bool DockBase::isOpen() const
{
    IF_ASSERT_FAILED(m_dockWidget) {
        return false;
    }

    return m_dockWidget->isOpen();
}

void DockBase::open()
{
    TRACEFUNC;

    IF_ASSERT_FAILED(m_dockWidget) {
        return;
    }

    m_dockWidget->show();
    setVisible(true);
}

void DockBase::close()
{
    TRACEFUNC;

    IF_ASSERT_FAILED(m_dockWidget) {
        return;
    }

    m_dockWidget->forceClose();
    setVisible(false);
}

void DockBase::showHighlighting(const QRect& highlightingRect)
{
    if (highlightingRect == m_highlightingRect) {
        return;
    }

    m_highlightingRect = highlightingRect;
    writeProperties();
}

void DockBase::hideHighlighting()
{
    showHighlighting(QRect());
}

QRect DockBase::frameGeometry() const
{
    if (m_dockWidget && m_dockWidget->isVisible()) {
        return m_dockWidget->frameGeometry();
    }

    return QRect();
}

void DockBase::resetToDefault()
{
    setVisible(m_defaultVisibility);
}

void DockBase::componentComplete()
{
    TRACEFUNC;

    QQuickItem::componentComplete();

    auto children = childItems();
    IF_ASSERT_FAILED_X(children.size() == 1, "Dock must have only one child as its content!") {
        return;
    }

    QQuickItem* content = children.first();
    IF_ASSERT_FAILED(content) {
        return;
    }

    if (content->objectName().isEmpty()) {
        content->setObjectName(objectName() + "_content");
    }

    m_dockWidget = new DockWidgetImpl(objectName());
    m_dockWidget->setWidget(content);
    m_dockWidget->setTitle(m_title);

    writeProperties();
    listenFloatingChanges();

    connect(m_dockWidget, &KDDockWidgets::DockWidgetQuick::widthChanged, this, [this]() {
        if (m_dockWidget) {
            setWidth(m_dockWidget->width());
        }
    });

    connect(m_dockWidget, &KDDockWidgets::DockWidgetQuick::heightChanged, this, [this]() {
        if (m_dockWidget) {
            setHeight(m_dockWidget->height());
        }
    });

    connect(this, &DockBase::minimumSizeChanged, this, &DockBase::applySizeConstraints);
    connect(this, &DockBase::maximumSizeChanged, this, &DockBase::applySizeConstraints);

    m_defaultVisibility = isVisible();
}

void DockBase::applySizeConstraints()
{
    if (!m_dockWidget) {
        return;
    }

    TRACEFUNC;

    int minWidth = m_minimumWidth > 0 ? m_minimumWidth : m_dockWidget->minimumWidth();
    int minHeight = m_minimumHeight > 0 ? m_minimumHeight : m_dockWidget->minimumHeight();
    int maxWidth = m_maximumWidth > 0 ? m_maximumWidth : m_dockWidget->maximumWidth();
    int maxHeight = m_maximumHeight > 0 ? m_maximumHeight : m_dockWidget->maximumHeight();

    QSize minimumSize(minWidth, minHeight);
    QSize maximumSize(maxWidth, maxHeight);

    if (!m_resizable) {
        maximumSize = minimumSize;
    }

    if (auto frame = m_dockWidget->frame()) {
        frame->setMinimumSize(minimumSize);
        frame->setMaximumSize(maximumSize);
    }

    m_dockWidget->setMinimumSize(minimumSize);
    m_dockWidget->setMaximumSize(maximumSize);

    if (auto window = m_dockWidget->floatingWindow()) {
        window->setMinimumSize(minimumSize);
        window->setMaximumSize(maximumSize);

        QSize winSize = adjustSizeByContraints(window->frameGeometry().size(), minimumSize, maximumSize);
        QRect winRect(window->dragRect().topLeft(), winSize);

        window->setGeometry(winRect);

        if (auto layout = window->layoutWidget()) {
            layout->setLayoutSize(winSize);
        }
    }
}

void DockBase::listenFloatingChanges()
{
    IF_ASSERT_FAILED(m_dockWidget) {
        return;
    }

    auto frameConn = std::make_shared<QMetaObject::Connection>();

    connect(m_dockWidget, &KDDockWidgets::DockWidgetQuick::parentChanged, this, [this, frameConn]() {
        if (frameConn) {
            disconnect(*frameConn);
            m_floating = std::nullopt;
        }

        if (!m_dockWidget || !m_dockWidget->parentItem()) {
            return;
        }

        const KDDockWidgets::Frame* frame = m_dockWidget->frame();
        if (!frame) {
            return;
        }

        //! NOTE: window will be available later
        //! So it is important to apply size constraints
        //! and emit floatingChanged() after that
        QTimer::singleShot(0, this, [this]() {
            if (!m_floating.has_value()) {
                updateFloatingStatus();
            }
        });

        *frameConn
            = connect(frame, &KDDockWidgets::Frame::isInMainWindowChanged, this, &DockBase::updateFloatingStatus, Qt::UniqueConnection);
    });

    connect(m_dockWidget->toggleAction(), &QAction::toggled, this, [this]() {
        if (!isOpen()) {
            m_floating = std::nullopt;
        }
    });
}

void DockBase::updateFloatingStatus()
{
    bool floating = m_dockWidget && m_dockWidget->floatingWindow();
    bool oldValueIsNull = (m_floating == std::nullopt);

    if (!oldValueIsNull && m_floating.value() == floating) {
        return;
    }

    TRACEFUNC;

    m_floating = floating;

    if (!oldValueIsNull || floating) {
        emit floatingChanged();
    }

    applySizeConstraints();
}

void DockBase::writeProperties()
{
    if (!m_dockWidget) {
        return;
    }

    DockProperties properties;
    properties.type = type();
    properties.location = location();
    properties.persistent = persistent();
    properties.separatorsVisible = separatorsVisible();
    properties.highlightingRect = m_highlightingRect;

    writePropertiesToObject(properties, *m_dockWidget);
}

bool DropDestination::operator==(const DropDestination& dest) const
{
    return dock == dest.dock && dropLocation == dest.dropLocation && dropDistance == dest.dropDistance;
}

bool DropDestination::isValid() const
{
    return dock != nullptr;
}

void DropDestination::clear()
{
    dock = nullptr;
    dropLocation = Location::Undefined;
    dropDistance = 0;
}
