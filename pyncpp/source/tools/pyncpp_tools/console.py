import sys, code
from PySide2.QtCore import Qt
from PySide2.QtGui import QKeySequence
from PySide2.QtWidgets import QApplication, QLabel, QLineEdit, QScrollArea, QShortcut, QVBoxLayout, QWidget


_logo = """
                            888 8888888                  d8b
                            888   888                    Y8P
                            888   888
88888b.d88b.   .d88b.   .d88888   888   88888b.  888d888 888  8888b.
888 "888 "88b d8P  Y8b d88" 888   888   888 "88b 888P"   888     "88b
888  888  888 88888888 888  888   888   888  888 888     888 .d888888
888  888  888 Y8b.     Y88b 888   888   888  888 888     888 888  888
888  888  888  "Y8888   "Y88888 8888888 888  888 888     888 "Y888888
"""


class CommandLine(QLineEdit):

    def __init__(self):
        super().__init__()
        self.prompt = ""
        self.history = [""]
        self.historyIndex = 0
 
    def keyPressEvent(self, event):
        key = event.key()
        if key == Qt.Key_Down or key == Qt.Key_Up:
            if key == Qt.Key_Down:
                self.historyIndex = min(self.historyIndex + 1, len(self.history) - 1)
            else:
                self.historyIndex = max(self.historyIndex - 1, 0)
            self.setText(self.history[self.historyIndex])
        elif key == Qt.Key_Return:
            self.historyIndex = len(self.history) - 1
            line = self.line()
            self.history[self.historyIndex] = line
            if line.strip():
                self.history.append("")
                self.historyIndex += 1
        if key != Qt.Key_Backspace or self.cursorPosition() > len(self.prompt):
            super().keyPressEvent(event)
        else:
            event.accept()

    def line(self):
        return super().text()[len(self.prompt):]

    def setText(self, text=""):
        super().setText(self.prompt + text)

        
class Console(QWidget):

    def __init__(self, size=(800, 600), title="Python console"):
        super().__init__()
        self._initWindow(size, title)
        self._initOutputWidget()
        self._initInputWidget()
        self._initFonts()
        self._initPrompt()
 
    def _initWindow(self, size, title):
        self.setWindowTitle(title)
        layout = QVBoxLayout()
        layout.setSpacing(0)
        self.setLayout(layout)
        self.resize(*size)

    def _initOutputWidget(self):
        self.outputWidget = QLabel()
        self.outputWidget.setAlignment(Qt.AlignBottom)
        self.outputWidget.setWordWrap(True)
        self.outputWidget.setTextFormat(Qt.PlainText)
        self.scrollArea = QScrollArea()
        self.scrollArea.setWidget(self.outputWidget)
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.horizontalScrollBar().setEnabled(False)
        verticalScrollBar = self.scrollArea.verticalScrollBar()
        verticalScrollBar.rangeChanged.connect(lambda _, max : verticalScrollBar.setValue(max))
        self.layout().addWidget(self.scrollArea)

    def _initInputWidget(self):
        self.inputWidget = CommandLine()
        self.layout().addWidget(self.inputWidget)
        self.setFocusProxy(self.inputWidget)
        self.inputWidget.returnPressed.connect(self.handleInput)
        self.inputWidget.cursorPositionChanged.connect(self.ensureCursorAfterPrompt)

    def _initFonts(self):
        font = self.outputWidget.font()
        font.setFamily("Courier")
        self.outputWidget.setFont(font)
        self.inputWidget.setFont(font)
        self.inputWidget.setFrame(False)

    def _initPrompt(self):
        try:
            self.ps1 = sys.ps1
        except AttributeError:
            self.ps1 = ">>> "
        try:
            self.ps2 = sys.ps2
        except AttributeError:
            self.ps2 = "... "
        self.inputWidget.prompt = self.ps1
        self.inputWidget.setText()

    def setSize(self, width, height):
        self.resize(width, height)

    def setShortcut(self, keySequence):
        if not hasattr(self, 'shortcut'):
            mainWindow = QApplication.instance().property('MainWindow')
            self.shortcut = QShortcut(mainWindow)
            self.shortcut.setContext(Qt.ApplicationShortcut)
            self.shortcut.activated.connect(lambda : self.setVisible(not self.isVisible()))
        self.shortcut.setKey(QKeySequence(keySequence))

    def run(self):
        self.interpreter = code.InteractiveInterpreter(sys.modules['__main__'].__dict__)
        self.resetInputBuffer()
        sys.stdout = self
        sys.stderr = self
        self.printWelcomeText()

    def printWelcomeText(self):
        print(f"Python {sys.version} on {sys.platform}")
        print(_logo)
        print('Type "help", "copyright", "credits" or "license" for more information on the embedded Python.\n\n')
  
    def resetInputBuffer(self):
        self.inputBuffer = []

    def handleInput(self):
        print(self.inputWidget.text())
        line = self.inputWidget.line()
        self.inputBuffer.append(line)
        source = "\n".join(self.inputBuffer)
        try:
            more = self.interpreter.runsource(source)
            self.updateInputStatus(newCommand = not(more and line))
        except SystemExit:
            self.updateInputStatus()
            QApplication.instance().property('MainWindow').close()

    def updateInputStatus(self, newCommand=True):
        if newCommand:
            self.inputWidget.prompt = self.ps1
            self.resetInputBuffer()
        else:
            self.inputWidget.prompt = self.ps2
        self.inputWidget.setText()

    def ensureCursorAfterPrompt(self, oldPosition, newPosition):
        self.inputWidget.setCursorPosition(max(len(self.inputWidget.prompt), newPosition))

    def write(self, text):
        self.outputWidget.setText(f'{self.outputWidget.text()}{text}')

    def flush(self):
        pass

    def showEvent(self, event):
        self.setFocus()


#app = QApplication(sys.argv)
#console = Console((500, 500))
#console.show()
#console.run()
#app.exec_()
