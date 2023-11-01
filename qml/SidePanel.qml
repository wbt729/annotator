import QtQuick 2.15
import QtQuick.Controls 2.12

Drawer {
    id: root

    property string pageSource: "qrc:/PageHistory.qml"
    property string pageName: qsTr("Labeling")

    ListModel {
        id: pagesModel
        ListElement {
            name: qsTr("Labeling")
            icon: "qrc:/icons/label_black_24dp.svg"
            page: "qrc:/PageScan.qml"
        }
        ListElement {
            name: qsTr("Models")
            icon: "qrc:/icons/lightbulb_black_24dp.svg"
            page: "qrc:/qml/mobile/PageParameters.qml"
        }
        ListElement {
            name: qsTr("Analytics")
            icon: "qrc:/icons/analytics_black_24dp.svg"
            page: "qrc:/qml/mobile/PageParameters.qml"
        }
        ListElement {
            name: qsTr("About")
            icon: "qrc:/icons/info_black_24dp.svg"
            page: "qrc:/qml/mobile/PageAbout.qml"
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: pagesModel
        interactive: false

        delegate: ItemDelegate {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 16
            text: model.name
            icon.source: model.icon
            onClicked: {
                listView.currentIndex = index
                root.visible = false
                pageSource = model.page
                pageName = model.name
            }
        }
    }
}
