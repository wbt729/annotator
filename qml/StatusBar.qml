import QtQuick 2.15
import QtQuick.Controls 2.12

Item {
    height: 32
    property int currentFileIndex: -1
    property int fileCount: 0
    Row {
        anchors.fill: parent
        spacing: 16
        Label {
            anchors.verticalCenter: parent.verticalCenter
            text: fileModel.sourceDir
        }

        ToolSeparator {anchors.verticalCenter: parent.verticalCenter}

        Label {
            anchors.verticalCenter: parent.verticalCenter
            text: "Current Marker:"
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            color: currentLabel == 0 ? "red" : "blue"
            height: 2
            width: 32
        }
        ToolSeparator {anchors.verticalCenter: parent.verticalCenter}

        Label {
            anchors { verticalCenter: parent.verticalCenter }
            text: "Image " + fileModel.currentIndex + " of " + fileModel.rowCount
        }
    }
}
