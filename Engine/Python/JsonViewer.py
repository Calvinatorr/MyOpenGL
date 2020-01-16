# Try to import Qt libraries
try:
    from PySide2.QtCore import *
    from PySide2.QtGui import *
    from PySide2.QtWidgets import *
except ImportError:
    try:
        from PySide.QtCore import *
        from PySide.QtGui import *
    except:
        print("Failed to load PySide libraries")
        exit(-1)

import os, sys, traceback, json


########################################################################################################################


app = QApplication(sys.argv)


class JsonViewer(QTreeWidget):
    def __init__(self, parent=None):
        super(JsonViewer, self).__init__(parent)

        # Styling
        self.setColumnCount(2)
        self.setHeaderLabels(["Key", "Data"])
        self.setAlternatingRowColors(True)
        self.setSortingEnabled(False)
        self.setMinimumHeight(400)

        self.setColumnWidth(1, 48)


    def AddData(self, jsonData):
        for key in jsonData:
            data = jsonData[key]
            print(key, data)
            item = QTreeWidgetItem(self, [str(key) + " : " + str(data)])

            # Recursively add data
            if type(data) is dict:
                self.AddData(data)


    def LoadJsonFile(self, path):
        # Process file and check if it exists
        path = os.path.normpath(path)
        if not os.path.exists(path) or not os.path.isfile(path):
            raise IOError

        file = open(path, "r+")

        jsonData = json.load(file)
        print("json data", jsonData)

        file.close()

        self.AddData(jsonData)




# Main window
class Window(QDialog):
    def __init__(self, parent=None):
        super(Window, self).__init__(parent)

        # Window styling
        self.setWindowTitle("Json Viewer")
        self.setWindowFlags(self.windowFlags() ^ Qt.WindowContextHelpButtonHint) # Hide help
        self.setGeometry(600, 300, 450, 350)
        stdIcon = self.style().standardIcon
        #self.setWindowIcon(QIcon(":/GameExporter/SavePreset_32"))
        self.show() # Show early


        # Standard icon
        stdIcon = self.style().standardIcon


        # Master layout
        self.setLayout(QVBoxLayout())
        self.layout().setAlignment(Qt.AlignTop)


        # File selection
        fileLayout = QHBoxLayout()
        fileLayout.setAlignment(Qt.AlignTop)
        self.layout().addLayout(fileLayout)
        self.label = QLabel("File")
        # Text field
        self.file = QTextEdit("File goes here")
        self.file.setFixedHeight(24)
        self.file.setText(os.path.expanduser("~\Documents"))
        # Browse button
        self.button = QPushButton()
        self.button.setIcon(stdIcon(QStyle.SP_DirIcon))
        self.button.setToolTip("Search for file")
        self.button.pressed.connect(self.__BrowseFile__)
        fileLayout.addWidget(self.label)
        fileLayout.addWidget(self.file)
        fileLayout.addWidget(self.button)


        # Json viewer
        viewLayout = QVBoxLayout()
        viewLayout.setAlignment(Qt.AlignTop)
        self.layout().addLayout(viewLayout)
        self.label = QLabel("Json data")
        self.jsonViewer = JsonViewer()
        viewLayout.addWidget(self.label)
        viewLayout.addWidget(self.jsonViewer)


    def __BrowseFile__(self):
        try:
            initialDir = str(self.file.toPlainText())
            file = QFileDialog.getOpenFileName(self, u"Open Json file", initialDir, u"Json files (*.json)")[0]
            if file != None and len(file) > 0:
                self.file.setText(str(file))
                self.__UpdateJsonViewer__()
        except Exception as e:
            traceback.print_exc()


    def __UpdateJsonViewer__(self):
        self.jsonViewer.LoadJsonFile(self.file.toPlainText())


########################################################################################################################


def ShowUI():
    global myWindow
    try:
        myWindow.close()
    except:
        pass
    myWindow = Window()
    sys.exit(app.exec_())
    return myWindow


if __name__ == '__main__':
    ShowUI()