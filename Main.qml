import QtQuick
import QtQuick.Window

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    CustomView3D {
        id: view3D
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 40

        transform: [
            Rotation { id: rotation; axis.x: 0; axis.z: 0; axis.y: 1; angle: 0; origin.x: view3D.width / 2; origin.y: view3D.height / 2; },
            Translate { id: txOut; x: -view3D.width / 2; y: -view3D.height / 2 },
            Scale { id: scale; },
            Translate { id: txIn; x: view3D.width / 2; y: view3D.height / 2 }
        ]
    }

    SequentialAnimation {
            PauseAnimation { duration: 2000 }
            ParallelAnimation {
                NumberAnimation { target: scale; property: "xScale"; to: 0.6; duration: 1000; easing.type: Easing.InOutBack }
                NumberAnimation { target: scale; property: "yScale"; to: 0.6; duration: 1000; easing.type: Easing.InOutBack }
            }
            NumberAnimation { target: rotation; property: "angle"; to: 80; duration: 1000; easing.type: Easing.InOutCubic }
            NumberAnimation { target: rotation; property: "angle"; to: -80; duration: 1000; easing.type: Easing.InOutCubic }
            NumberAnimation { target: rotation; property: "angle"; to: 0; duration: 1000; easing.type: Easing.InOutCubic }
            NumberAnimation { target: view3D; property: "opacity"; to: 0.5; duration: 1000; easing.type: Easing.InOutCubic }
            PauseAnimation { duration: 1000 }
            NumberAnimation { target: view3D; property: "opacity"; to: 0.8; duration: 1000; easing.type: Easing.InOutCubic }
            ParallelAnimation {
                NumberAnimation { target: scale; property: "xScale"; to: 1; duration: 1000; easing.type: Easing.InOutBack }
                NumberAnimation { target: scale; property: "yScale"; to: 1; duration: 1000; easing.type: Easing.InOutBack }
            }
            running: true
            loops: Animation.Infinite
        }

    Rectangle {
        id: labelFrame
        anchors.margins: -10
        radius: 5
        color: "white"
        border.color: "black"
        opacity: 0.8
        anchors.fill: label
    }

    Text {
        id: label
        anchors.bottom: view3D.bottom
        anchors.left: view3D.left
        anchors.right: view3D.right
        anchors.margins: 20
        wrapMode: Text.WordWrap
        text: qsTr("The blue rectangle with the vintage 'Q' is an FBO, rendered by the application on the scene graph rendering thread. The FBO is managed and displayed using the QQuickFramebufferObject convenience class.")
    }
}
