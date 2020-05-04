import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.14

import Qt.labs.platform 1.1

import PowerObserver 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 300
    height: 140
    title: qsTr("Power Guard")
    Material.theme: Material.Light
    Material.accent: Material.LightBlue

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20

        Image {
            fillMode: Image.PreserveAspectFit
            source: "qrc:/icons/shield.png"
            Layout.preferredWidth: 100
            Layout.preferredHeight: 100
            clip: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter

            Label {
                text: "Notify if charge less than:"
            }
            SpinBox {
                Layout.fillWidth: true
                from: 5
                to: 99
                value: observer.threshold
                onValueChanged: observer.threshold = value
            }
        }
    }

    SystemTrayIcon {
        id:tray
        visible: true
        icon.source: "qrc:/icons/shield.png"

        onActivated: {
            window.show()
            window.raise()
            window.requestActivate()
        }

        menu: Menu {
            MenuItem {
                text: qsTr("Show status")
                onTriggered: {
                    showMessage();
                }
            }
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit()
            }
        }
    }

    function showMessage() {
        tray.showMessage(
                    observer.source === PowerObserver.AC ? "AC power" : "Battery power",
                    "Current charge: " + observer.charge,
                    100000
                    );
    }

    Connections {
        target: observer
        onSourceChanged: showMessage();
        onThresholdPassed: showMessage();
    }

}
