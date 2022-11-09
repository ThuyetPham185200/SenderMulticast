import QtQuick 2.9
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.3
import thuyet.sender.com 1.0
//import io.dev.qt 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Sender")



    Rectangle {
        anchors {fill: parent; margins: 50}
        color: "transparent"

        Sender {
               id: sender
        }

        GridLayout {
            width: parent.width/2
            height: parent.height/2
            anchors.centerIn: parent
            columns: 1

            TextField {
                id: txtIP
                text: "239.255.43.21"
            }

            TextField {
                id: txtPort
                text: "45454"
            }
            TextField {
                id: txtFile
                text: "/home/thuyet/Desktop/logs/15-05-28.bin"
            }


            Button {
                text: "Send"
                onClicked: {
                   sender.sendData(txtIP.text, parseInt(txtPort.text))
                }
            }

            Button {
                text: "Chose file"
                onClicked: {
                    sender.selectFile(txtFile.text)
                }
            }

        }
    }
}
