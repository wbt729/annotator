import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.2

Pane {
    id: root
    Material.elevation: 3

    Item {
        id: header
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 32

        Label {
            text: qsTr("Files")
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }

        Row {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            layoutDirection: Qt.RightToLeft

            ToolButton {
                icon.source: "qrc:/icons/folder_open_black_24dp.svg"
                onClicked: fileDialog.visible = true
            }
        }
    }

    ScrollView {
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.contentItem: Rectangle {
            color: "darkgrey"
            opacity: 0.3
            width: 8
            radius: 4
        }
        ScrollBar.vertical.width: 8

        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: footer.top
        }

        ListView {
            id: fileList
            model: fileModel

            highlight: Rectangle { color: "lightgrey" }
            highlightMoveDuration: 120

            anchors.fill: parent

            clip: true

            Connections {
                target: fileModel
                onCurrentIndexChanged: {
                    console.log("current index changed")
                    fileList.currentIndex = fileModel.currentIndex
                }
            }

            delegate: Item {
                height: 50
                width: ListView.view.width

                Row {
                    anchors.fill: parent
                    spacing: 16
                    Image {
                        anchors { top: parent.top; bottom: parent.bottom; margins: 8 }
                        source: "file:///" + model.imageFileName
                        fillMode: Image.PreserveAspectFit
                    }

                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: model.name
                    }

                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: model.boxesModel.rowCount
                    }
                }
                Rectangle {
                    height: 1
                    color: "lightgrey"
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        print("clicked")
//                        fileList.currentIndex = model.index
                        fileModel.currentIndex = model.index
                    }
                }
            }
            onCurrentIndexChanged: {
                image.source = "file:///" + fileModel.getData(currentIndex, 1)
                console.log(image.source)
                currentFileName = fileModel.getData(currentIndex, 1)
                repeater.model = fileModel.getBoxesModel(currentIndex)
            }
        }
    }

    Item {
        id: footer
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 16
        Row {
            anchors.fill: parent
            Label {
                text: qsTr("File") + " " + fileModel.currentIndex + " " + qsTr("of") + " " + fileList.count
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Label {
        anchors.centerIn: parent
        visible: fileList.count == 0
        text: qsTr("List Empty")
    }

    FileDialog {
        id: fileDialog
        selectFolder: true
        onAccepted: {
            fileModel.sourceDir = fileDialog.fileUrl.toString()
            fileList.currentIndex = 0
        }
    }
}
