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

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import MuseScore.Ui 1.0
import MuseScore.UiComponents 1.0

ListView {
    id: root

    property QtObject editorModel: null

    property var contextMenuModel: [
        {id: "copy", title: qsTrc("mpe", "Copy pattern data")}
    ]

    height: 400
    width: 180

    spacing: 4

    ScrollBar.vertical: StyledScrollBar { }

    delegate: Item {
        id: delegateItem

        height: 32
        width: root.width

        function selectCurrentItem() {
            modelData.isSelected = true
        }

        Rectangle {
            id: backgroundRect

            anchors.fill: parent

            radius: 2

            color: ui.theme.backgroundPrimaryColor
        }

        RowLayout {
            anchors.fill: delegateItem
            anchors.margins: 4

            StyledTextLabel {
                id: label

                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.fillWidth: true

                horizontalAlignment: Qt.AlignLeft

                text: modelData.title

                MouseArea {
                    id: mouseArea

                    anchors.fill: label

                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    hoverEnabled: true

                    onClicked: {
                        if (mouse.button === Qt.LeftButton) {
                            delegateItem.selectCurrentItem()
                            return
                        }

                        if (mouse.button == Qt.RightButton && root.editorModel && !modelData.isSelected ) {
                            menuLoader.toggleOpened(root.contextMenuModel, mouseX, mouseY)
                        }
                    }
                }

                StyledMenuLoader {
                    id: menuLoader

                    onHandleMenuItem: {
                        if (itemId !== "copy" || !root.editorModel) {
                            return
                        }

                        root.editorModel.copyPatternDataFromItem(modelData)
                    }
                }
            }

            CheckBox {
                id: checkBox

                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                Layout.rightMargin: 12

                checked: modelData.isActive

                onClicked: {
                    modelData.isActive = !modelData.isActive
                    delegateItem.selectCurrentItem()
                }
            }
        }

        states: [
            State {
                name: "SELECTED"

                when: modelData.isSelected

                PropertyChanges {
                    target: backgroundRect

                    color: ui.theme.accentColor
                }
            },

            State {
                name: "HOVERED"
                when: (mouseArea.containsMouse || checkBox.hovered) && !modelData.isSelected && !mouseArea.containsPress

                PropertyChanges {
                    target: backgroundRect
                    color: ui.theme.buttonColor
                    opacity: ui.theme.buttonOpacityHover
                }
            },

            State {
                name: "PRESSED"
                when: (mouseArea.containsPress || checkBox.pressed) && !modelData.isSelected

                PropertyChanges {
                    target: backgroundRect
                    color: ui.theme.buttonColor
                    opacity: ui.theme.buttonOpacityHit
                }
            }
        ]
    }
}
