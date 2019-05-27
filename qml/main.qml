import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import org.qtproject.example 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    minimumWidth: 400
    minimumHeight: 300

    title: document.documentTitle + " - Morse Example"

    MessageDialog {
        id: aboutBox
        title: "About Text"
        text: "This is a basic text editor \nwritten with Qt Quick Controls"
        icon: StandardIcon.Information
    }

    Action {
        id: cutAction
        text: "Cut"
        shortcut: "ctrl+x"
        iconSource: "images/editcut.png"
        iconName: "edit-cut"
        onTriggered: textArea.cut()
    }

    Action {
        id: copyAction
        text: "Copy"
        shortcut: "Ctrl+C"
        iconSource: "images/editcopy.png"
        iconName: "edit-copy"
        onTriggered: textArea.copy()
    }

    Action {
        id: pasteAction
        text: "Paste"
        shortcut: "ctrl+v"
        iconSource: "images/editpaste.png"
        iconName: "edit-paste"
        onTriggered: textArea.paste()
    }

    FileDialog {
        id: fileDialog
        nameFilters: ["Text files (*.txt)"]
        onAccepted: {
            if (fileDialog.selectExisting)
                document.fileUrl = fileUrl
            else
                document.saveAs(fileUrl, selectedNameFilter)
        }
    }

    FileDialog {
        id: fileDialogTranslate
        nameFilters: ["Text files (*.txt)"]
        onAccepted: {
            document.saveTranslateAs(fileUrl, selectedNameFilter, textArea2.text)
        }
    }

    Action {
        id: fileOpenAction
        iconSource: "images/fileopen.png"
        iconName: "document-open"
        text: "Open"
        onTriggered: {
            fileDialog.selectExisting = true
            fileDialog.open()
        }
    }

    Action {
        id: fileSaveAsAction
        iconSource: "images/filesave.png"
        iconName: "document-save"
        text: "Save As…"
        onTriggered: {
            fileDialog.selectExisting = false
            fileDialog.open()
        }
    }

    Action {
        id: fileTranslateSaveAsAction
        iconSource: "images/filesave.png"
        iconName: "document-save"
        text: "Save Translate As…"
        onTriggered: {
            fileDialogTranslate.open()
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem { action: fileOpenAction }
            MenuItem { action: fileSaveAsAction }
            MenuItem { action: fileTranslateSaveAsAction }
            MenuItem { text: "Quit"; onTriggered: Qt.quit() }
        }
        Menu {
            title: "&Edit"
            MenuItem { action: copyAction }
            MenuItem { action: cutAction }
            MenuItem { action: pasteAction }
        }
    }

    toolBar: ToolBar {
        id: mainToolBar
        width: parent.width
        RowLayout {
            anchors.fill: parent
            spacing: 0
            ToolButton { action: fileOpenAction }
            ToolButton { action: copyAction }
            ToolButton { action: cutAction }
            ToolButton { action: pasteAction }

            Item { Layout.fillWidth: true }
        }
    }

    TextArea {
        Accessible.name: "document"
        id: textArea
        frameVisible: false
        width: parent.width / 2
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 10
        baseUrl: "qrc:/"
        text: document.text
        textFormat: Qt.PlainText
        font.pointSize: 14
        Component.onCompleted: forceActiveFocus()
    }

    TextArea {
        Accessible.name: "document"
        id: textArea2
        frameVisible: false
        width: parent.width / 2
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        baseUrl: "qrc:/"
        text: document.textMorze
        textFormat: Qt.PlainText
        font.pointSize: 14
    }

    MessageDialog {
        id: errorDialog
    }

    DocumentHandler {
        id: document
        target: textArea
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        Component.onCompleted: document.fileUrl = "qrc:/example.txt"
        onError: {
            errorDialog.text = message
            errorDialog.visible = true
        }
    }

    Connections {
        target: textArea
        onTextChanged: {
            //textArea2.text = document.toMorse(textArea.getText(0, textArea.text.length))
            textArea2.text = document.translate(textArea.getText(0, textArea.text.length))
        }
    }
}
