import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.12

ApplicationWindow {
    width: 1280
    height: 800
    visible: true
    title: qsTr("SoC Labeling Tool")

    property string currentFileName
    property int currentLabel: 0
    property bool showExtraFunctions: false // angle and delete buttons

    Material.primary: "#000080"

    id: root


    header: ToolBar {
        Row {
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
                rightMargin: 16
            }

            ToolButton {
                icon.source: "qrc:/icons/menu_black_24dp.svg"
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    drawer.visible = !drawer.visible
                }
                icon.color: "white"
            }

            Label {
                text: drawer.pageName
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 12
                font.bold: true
                color: "white"
            }

            spacing: 32

            Label {
              text: fileModel.sourceDir
              anchors.verticalCenter: parent.verticalCenter
              color: "white"
            }
        }

        Row {
            spacing: 8

            anchors {
                right: parent.right
                verticalCenter: parent.verticalCenter
                rightMargin: 8
            }

            ToolButton {
                icon.source: "qrc:/icons/account_circle-24px.svg"
                icon.color: "white"
            }

            ToolButton {
                icon.source: "qrc:/icons/help_outline-24px.svg"
                icon.color: "white"
            }
        }
    }

    footer: StatusBar {}

    SidePanel {
        id: drawer
        edge: Qt.LeftEdge
        width: root.width * 0.25
        height: root.height - header.height
        y: header.height
    }

    FileListPane {
        id: fileListPane
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 16
        width: 300
    }

    Pane {
        id: imagePane
        anchors.left: fileListPane.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 16

        Image {
            id: image
            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
        }

        Label {
            anchors.centerIn: parent
            visible: image.source == ""
            text: qsTr("No image selected, please select a working directory to start labeling")
        }

        Item {
            anchors.centerIn: parent
            height: image.paintedHeight
            width: image.paintedWidth

            MouseArea {
                id: markerMouseArea
                anchors.fill: parent
                hoverEnabled: true

                property real originX: 0
                property real originY: 0

                onPressed: {
                    marker.active = true
                    originX = mouseX
                    originY = mouseY

                    marker.x = originX
                    marker.y = originY
                }
                onReleased: {
                    marker.active = false
                    var rect = Qt.rect(marker.x / image.paintedWidth,
                                   marker.y / image.paintedHeight,
                                   marker.width / image.paintedWidth,
                                   marker.height / image.paintedHeight)
                    fileModel.addBox(fileModel.currentIndex, rect, currentLabel)
                }

                onMouseXChanged: {
                    if(marker.active) {
                        if(mouseX < originX) {
                            marker.x = mouseX
                            marker.width = originX - mouseX
                        }
                        else {
                            marker.x = originX
                            marker.width = mouseX - originX
                        }
                    }
                }
                onMouseYChanged: {
                    if(marker.active) {
                        if(mouseY < originY) {
                            marker.y = mouseY
                            marker.height = originY - mouseY
                        }
                        else {
                            marker.y = originY
                            marker.height = mouseY - originY
                        }
                    }
                }
            }

            Repeater {
                id: repeater
                anchors.centerIn: parent
                height: image.paintedHeight
                width: image.paintedWidth

                Item {
                    x: model.rect.x * parent.width
                    y: model.rect.y * parent.height
                    width: model.rect.width * parent.width
                    height: model.rect.height * parent.height


                    Rectangle {
                        anchors.fill: parent
                        border.color: model.label == 0 ? "red" : "blue"
                        color: "transparent"
                        border.width: 2
                        clip: true

                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.RightButton
                            onClicked: {
                                if (mouse.button == Qt.RightButton) { // 'mouse' is a MouseEvent argument passed into the onClicked signal handler
                                    fileModel.deleteBox(fileModel.currentIndex, model.index)
                                }
                            }
                        }
                    }
                    ToolButton {
                        visible: showExtraFunctions
                        anchors.right: parent.right
                        icon.source: "qrc:/close-24px.svg"
                        onClicked: {
                            console.log(model)
                            fileModel.deleteBox(fileModel.currentIndex, model.index)
                        }
                    }
                }
            }

            Rectangle {
                property bool active: false
                id: marker
                visible: active
                color: "green"
                width: 100
                height: 100
                opacity: 0.3
            }


        }

        Label {
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            font.pointSize: 24
            id: boxesCountLabel
        }
    }

    function next() {
        console.log("next")
        if(fileModel.currentIndex < fileModel.rowCount - 1)
        fileModel.currentIndex += 1
    }

    function previous() {
        console.log("previous")
        if(fileList.currentIndex > 0)
            fileList.currentIndex -= 1
    }

    Shortcut {
        sequence: "1"
        onActivated: currentLabel = 0
    }

    Shortcut {
        sequence: "2"
        onActivated: currentLabel = 1
    }

    Shortcut {
        sequence: " "
        onActivated: {
            fileModel.save(fileModel.currentIndex)
            next()
        }
    }

    Shortcut {
        sequence: "d"
        onActivated: {
            fileModel.deleteAllBoxes(fileList.currentIndex)
        }
    }

    Shortcut {
        sequence: "q"
        onActivated: {
            previous()
        }
    }

    Shortcut {
        sequence: "e"
        onActivated: {
            next()
        }
    }

    HelpDialog {
        id: helpDialog
    }
}
