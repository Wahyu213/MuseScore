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

#include "voltasettingsmodel.h"

#include "translation.h"

#include "ui/view/iconcodes.h"

using namespace mu::inspector;

using IconCode = mu::ui::IconCode::Code;

VoltaSettingsModel::VoltaSettingsModel(QObject* parent, IElementRepositoryService* repository)
    : LineSettingsModel(parent, repository, Ms::ElementType::VOLTA)
{
    setModelType(InspectorModelType::TYPE_VOLTA);
    setTitle(qtrc("inspector", "Volta"));
    setIcon(ui::IconCode::Code::VOLTA);

    static const QList<HookTypeInfo> hookTypes {
        { Ms::HookType::NONE, IconCode::LINE_WITH_INVERTED_START_HOOK, qtrc("inspector", "Normal") },
        { Ms::HookType::HOOK_90, IconCode::LINE_WITH_TWO_INVERTED_HOOKS, qtrc("inspector", "Hooked 90") }
    };

    setPossibleEndHookTypes(hookTypes);

    createProperties();
}

PropertyItem* VoltaSettingsModel::repeatCount() const
{
    return m_repeatCount;
}

void VoltaSettingsModel::createProperties()
{
    LineSettingsModel::createProperties();

    m_repeatCount = buildPropertyItem(Ms::Pid::VOLTA_ENDING);

    placement()->setIsVisible(false);
    beginingTextHorizontalOffset()->setIsVisible(false);
    continiousTextHorizontalOffset()->setIsVisible(false);
}

void VoltaSettingsModel::loadProperties()
{
    LineSettingsModel::loadProperties();

    loadPropertyItem(m_repeatCount);
}

void VoltaSettingsModel::resetProperties()
{
    LineSettingsModel::resetProperties();

    m_repeatCount->resetToDefault();
}
