import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12

Dialog {
    height: 400
    width: 300

    anchors.centerIn: parent

    title: qsTr("Help")

    Column {
        anchors {
            fill: parent
            margins: 16
        }

        spacing: 16

        GroupBox {
            title: qsTr("Keyboard Shortcuts")

            Grid {
                columns: 2
                columnSpacing: 32
                rowSpacing: 8
                Label {
                    text: qsTr("Key")
                    font.bold: true
                }
                Label {
                    text: qsTr("Function")
                    font.bold: true
                }

                Label {
                    text: qsTr("Space")
                }
                Label {
                    text: qsTr("Save and go to next Image")
                }
                Label {
                    text: qsTr("q")
                }
                Label {
                    text: qsTr("Previous image")
                }
                Label {
                    text: qsTr("e")
                }
                Label {
                    text: qsTr("Next image")
                }
                Label {
                    text: qsTr("r")
                }
                Label {
                    text: qsTr("Clear labels in current image")
                }
                Label {
                    text: qsTr("d")
                }
                Label {
                    text: qsTr("Clear labels in current image")
                }
                Label {
                    text: qsTr("1")
                }
                Label {
                    text: qsTr("Select parcel")
                }
                Label {
                    text: qsTr("2")
                }
                Label {
                    text: qsTr("Select shopper")
                }
            }
        }

    }
}
