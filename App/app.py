import sys
import os

from PySide6 import QtCore, QtGui, QtWidgets

FIXED_SQ_SIZE = 640 // 8

LIGHT_SQUARE = '#EBECD0'
DARK_SQUARE = '#779556'

SQUARE_DICT = {
     0: 'a1',  1: 'b1',  2: 'c1',  3: 'd1',  4: 'e1',  5: 'f1',  6: 'g1',  7: 'h1',
     8: 'a2',  9: 'b2', 10: 'c2', 11: 'd2', 12: 'e2', 13: 'f2', 14: 'g2', 15: 'h2',
    16: 'a3', 17: 'b3', 18: 'c3', 19: 'd3', 20: 'e3', 21: 'f3', 22: 'g3', 23: 'h3',
    24: 'a4', 25: 'b4', 26: 'c4', 27: 'd4', 28: 'e4', 29: 'f4', 30: 'g4', 31: 'h4',
    32: 'a5', 33: 'b5', 34: 'c5', 35: 'd5', 36: 'e5', 37: 'f5', 38: 'g5', 39: 'h5',
    40: 'a6', 41: 'b6', 42: 'c6', 43: 'd6', 44: 'e6', 45: 'f6', 46: 'g6', 47: 'h6',
    48: 'a7', 49: 'b7', 50: 'c7', 51: 'd7', 52: 'e7', 53: 'f7', 54: 'g7', 55: 'h7',
    56: 'a8', 57: 'b8', 58: 'c8', 59: 'd8', 60: 'e8', 61: 'f8', 62: 'g8', 63: 'h8',
}

PIECE_IMAGES = {
    '0': "Chess_plt45.svg.png",  # WHITE_PAWNS_BB
    '1': "Chess_pdt45.svg.png",  # BLACK_PAWNS_BB
    '2': "Chess_nlt45.svg.png",  # WHITE_KNIGHTS_BB
    '3': "Chess_ndt45.svg.png",  # BLACK_KNIGHTS_BB
    '4': "Chess_blt45.svg.png",  # WHITE_BISHOPS_BB
    '5': "Chess_bdt45.svg.png",  # BLACK_BISHOPS_BB
    '6': "Chess_rlt45.svg.png",  # WHITE_ROOKS_BB
    '7': "Chess_rdt45.svg.png",  # BLACK_ROOKS_BB
    '8': "Chess_qlt45.svg.png",  # WHITE_QUEENS_BB
    '9': "Chess_qdt45.svg.png",  # BLACK_QUEENS_BB
    '10': "Chess_klt45.svg.png", # WHITE_KING_BB
    '11': "Chess_kdt45.svg.png", # BLACK_KING_BB
    '12': None                  # EMPTY_SQUARE
}

BASE_DIR = os.path.dirname(__file__)
PIECES_DIR = os.path.join(BASE_DIR, "Imgs", "Pieces")


class EngineController(QtCore.QObject):
    ready = QtCore.Signal()
    pos = QtCore.Signal(str)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.proc = QtCore.QProcess(self)
        self.proc.readyReadStandardOutput.connect(self._read)

    def start(self, path=f"{BASE_DIR}/engine_bin"):
        self.proc.start(path)

        if not self.proc.waitForStarted(3000):
            raise RuntimeError("Engine start failed")

        print("Engine started...")
        self._send("play")

    def send_isready(self):
        self._send("isready")

    def send_quit(self):
        self._send("quit")

        while self.proc.waitForFinished(1000) != True:
            print("Waiting Engine...")

        print("Engine quit...")
        self.proc.close()

    def _read(self):
        while self.proc.canReadLine():
            line = bytes(self.proc.readLine()).decode().rstrip()
            self._process_line(line)

    def _send(self, text: str):
        print("Sending: " + text)

        if self.proc.state() != QtCore.QProcess.ProcessState.Running:
            return

        self.proc.write(QtCore.QByteArray((text + "\n").encode()))

    def _process_line(self, line: str):
        print("Receiving: " + line)

        if line[:3] == "pos":
            self.pos.emit(line)
        elif line == "readyok":
            self.ready.emit()
 

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Chess GUI (PySide6)")

        self.engine = EngineController()
        self.engine.start()

        layout = QtWidgets.QHBoxLayout()
        chessBoard = ChessBoardWidget(self.engine)
        layout.addWidget(chessBoard, alignment=QtCore.Qt.AlignmentFlag.AlignCenter)

        widget = QtWidgets.QWidget()
        widget.setLayout(layout)
        self.setCentralWidget(widget)

        self.setStyleSheet("background-color: #161512")


    def closeEvent(self, e):
        self.engine.send_quit()
        e.accept()


class ChessBoardWidget(QtWidgets.QWidget):
    def __init__(self, engine):
        super().__init__()

        self.engine = engine

        self.board_state = []

        self.engine.pos.connect(self.parse_pos)

        self.source_square = -1 
        self.target_square = -1

        self.piece_pixmaps = {}
        for key, filename in PIECE_IMAGES.items():
            if filename is not None:
                path = os.path.join(PIECES_DIR, filename)
                pixmap = QtGui.QPixmap(path)
                if pixmap.isNull():
                    print(f"Imagen no encontrada: {filename}")
                self.piece_pixmaps[key] = pixmap
            else:
                self.piece_pixmaps[key] = None

        self.setSizePolicy(
            QtWidgets.QSizePolicy.Policy.MinimumExpanding,
            QtWidgets.QSizePolicy.Policy.MinimumExpanding
        )

        self.engine.send_isready()

        self.engine_ready_loop = QtCore.QEventLoop()
        self.engine.ready.connect(self.on_ready)
        self.engine_ready_loop.exec()

    def sizeHint(self):
        return QtCore.QSize(640, 640)

    def on_ready(self):
        print("Engine ready!")
        self.engine_ready_loop.quit()

    def on_illegal(self):
        self.illegal = True

    def parse_pos(self, line: str):
        self.board_state = [piece for piece in line[4:-1].split("-")]
        self.update()

    def paintEvent(self, e):
        painter = QtGui.QPainter(self)

        brush = QtGui.QBrush()
        brush.setStyle(QtCore.Qt.BrushStyle.SolidPattern)

        square_size = 640 // 8

        for rank in range(8):
            for file in range(8):
                square = ((7 - rank) * 8) + file
                square_rect = QtCore.QRect(file * square_size, rank * square_size, square_size, square_size)

                color = DARK_SQUARE if (rank + file) % 2 != 0 else LIGHT_SQUARE
                brush.setColor(QtGui.QColor(color))

                if square == self.source_square:
                    brush.setColor(QtGui.QColor(color).lighter(200))

                painter.fillRect(square_rect, brush)

                piece = self.board_state[square]
                pix = self.piece_pixmaps.get(piece)
                
                if pix is not None:
                    painter.drawPixmap(square_rect, pix)


    def mousePressEvent(self, e):
        if e.button() != QtCore.Qt.MouseButton.LeftButton:
            return

        x = int(e.position().x())
        y = int(e.position().y())

        file = x // FIXED_SQ_SIZE 
        rank = 7 - (y // FIXED_SQ_SIZE)
        square = (rank * 8) + file

        if self.source_square == -1:
            self.source_square = square
            self.update()
        elif self.source_square != square:
            uci_string = f"move {SQUARE_DICT[self.source_square]}{SQUARE_DICT[square]}"
            self.engine._send(uci_string)
            self.source_square = -1
        else:
            return

        # Consider not redrawing the board for highlighting the source square

app = QtWidgets.QApplication(sys.argv)

window = MainWindow()
window.show()

app.exec()
