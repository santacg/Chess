import sqlite3
import chess
import chess.pgn
import numpy as np
import torch
from bitarray import bitarray
from torch.utils.data import Dataset, DataLoader
from chess.engine import Limit
from chess.engine import SimpleEngine

def get_input_idx(x, y, z):
    return (x * 64 * 10) + (y * 10) + z
from chess.engine import SimpleEngine

def get_input_bits(board, turn):
    input = bitarray(64*64*10)
    king_sq = board.king(turn)

    for color in chess.COLORS:
        for piece_type in chess.PIECE_TYPES:
            if piece_type != chess.KING:
                piece_sqs = board.pieces(piece_type, color)
                for piece_sq in piece_sqs:
                    input[get_input_idx(king_sq, piece_sq, piece_type)] = 1

    return input


def get_input(board):
    turn = board.turn
    input_current = get_input_bits(board, turn)
    input_opponent = get_input_bits(board, not turn)

    return input_current, input_opponent


class ChessDatabase():
    def __init__(self, db_name: str, stockfish_path: str):
        self.db_name = db_name
        self.engine = SimpleEngine.popen_uci(stockfish_path)

    def crear_db(self):
        conn = sqlite3.connect(self.db_name)
        cursor = conn.cursor()

        cursor.execute('''
            CREATE TABLE IF NOT EXISTS games (
                id INTEGER PRIMARY KEY,
                input_current BLOB,
                input_opponent BLOB,
                evaluation REAL
            )
        ''')

        conn.commit()
        conn.close()

    def get_stockfish_eval(self, board):
        # Evaluar la posición con Stockfish
        info = self.engine.analyse(board, Limit(depth=15))  # Evaluación rápida
        eval = info["score"].relative.score()

        # Verificar si es una evaluación de mate
        if isinstance(info["score"], chess.engine.Mate):
            return None

        return eval 


    def load_games_to_db(self, pgn_file_path: str):
        # Nos conectamos a la base de datos
        conn = sqlite3.connect(self.db_name)
        cursor = conn.cursor()

        # Abrimos el archivo en formato PGN
        pgn_file = open(pgn_file_path)

        # Procesamos todas las partidas del archivo
        i = 0
        for i in range(1000):
            print("Iteración: ", i)
            game = chess.pgn.read_game(pgn_file)
            # Cuando no se encuentre partida, salimos del bucle
            if game is None:
                break

            # Cargamos el tablero para obtener los datos relevantes
            board = game.board()

            moves = game.mainline_moves()
            n_moves = len(list(moves))

            # Ejecutamos un número aleatorio de movimientos
            rand = np.random.rand()

            if rand < 0.15:  # 15% para Aperturas
                rng = int(np.random.uniform(0, n_moves // 4))  # Seleccionar las primeras jugadas
            elif rand < 0.65:  # 50% para Medio Juego
                rng = int(np.random.uniform(n_moves // 4, 3 * n_moves // 4))  # Seleccionar jugadas en el medio juego
            else:  # 35% para Finales
                rng = int(np.random.uniform(3 * n_moves // 4, n_moves))  # Seleccionar las últimas jugadas


            for idx, move in enumerate(moves):
                if idx == rng:
                    break
                board.push(move)

            # Obtenemos los inputs
            input_current, input_opponent = get_input(board)

            # Evaluación de Stockfish
            eval = self.get_stockfish_eval(board)

            if eval is None:
                continue

            # Guardamos los datos en la base de datos
            cursor.execute('''
                INSERT INTO games (input_current, input_opponent, evaluation)
                VALUES (?, ?, ?)
            ''', (input_current.tobytes(), input_opponent.tobytes(), eval))
            i +=1

        conn.commit()
        conn.close()

    def close_engine(self):
        self.engine.quit()
    

class ChessDataset(Dataset):
    def __init__(self, db_name: str, is_test=False, test_size=0.2):
        self.db_name = db_name
        self.is_test = is_test
        self.test_size = test_size
        self.conn = sqlite3.connect(db_name)
        self.cursor = self.conn.cursor()

        # Cargar los índices de las partidas
        self.cursor.execute('SELECT COUNT(*) FROM games')
        self.num_games = self.cursor.fetchone()[0]

        # Dividir los índices en conjuntos de entrenamiento y prueba
        self.train_indices, self.test_indices = self._split_data()

    def _split_data(self):
        # Generar índices de las partidas
        all_indices = list(range(1, self.num_games + 1))
        np.random.shuffle(all_indices)

        # Dividir en entrenamiento y prueba
        test_size = int(self.num_games * self.test_size)
        test_indices = all_indices[:test_size]
        train_indices = all_indices[test_size:]

        return train_indices, test_indices

    def __len__(self):
        # Retorna el número de partidas en el conjunto de datos de test o de entrenamiento
        if self.is_test:
            return len(self.test_indices)
        else:
            return len(self.train_indices)

    def __getitem__(self, index):
        # Seleccionar el índice correspondiente según si es entrenamiento o prueba
        if self.is_test:
            index = self.test_indices[index]
        else:
            index = self.train_indices[index]

        # Obtener las entradas para el índice dado
        self.cursor.execute('SELECT input_current, input_opponent, evaluation FROM games WHERE id = ?', (index,))
        row = self.cursor.fetchone()

        input_current_blob, input_opponent_blob, eval = row
        input_current = bitarray()
        input_current.frombytes(input_current_blob)

        input_opponent = bitarray()
        input_opponent.frombytes(input_opponent_blob)

        # Convertir los bitarrays a tensores
        input_current = torch.tensor(input_current, dtype=torch.float32)
        input_opponent = torch.tensor(input_opponent, dtype=torch.float32)

        eval = torch.tensor(eval, dtype=torch.float32)

        return input_current, input_opponent, eval

    def load_data(self, batch_size=32):
        # Crear el DataLoader para entrenamiento o prueba según el parámetro is_test
        data_loader = DataLoader(self, batch_size=batch_size, shuffle=not self.is_test)
        return data_loader

    def close(self):
        self.conn.close()

