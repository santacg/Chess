import chess
import json
import torch
from torch.utils.data import Dataset
from tqdm import tqdm
from bitarray import bitarray

MATE_SCORE = 100000


def get_input_idx(x, y, z):
    return (x * 64 * 10) + (y * 10) + z


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


def get_best_evaluation(position):
    # Seleccionar la evaluación con la mayor profundidad
    best_eval = max(position['evals'], key=lambda x: x['depth'])

    # Obtener el primer PV de la mejor evaluación
    best_pv = best_eval['pvs'][0]

    # Extraer la evaluación 
    eval_result = -1
    if 'cp' in best_pv:
        eval_result = best_pv['cp']  # Evaluación en centipawns
    elif 'mate' in best_pv:
        eval_result = MATE_SCORE / best_pv['mate']  # Evaluación en jugadas hasta el mate

    return eval_result


def process_to_pt(input_file, output_file):

    inputs_current = []
    inputs_opponent = []
    labels = []

    with open(input_file, "r") as f:
        for line in tqdm(f, desc="Procesando positiones"):
            # Cargamos la posición desde el JSONL
            position = json.loads(line)

            # Cargar tablero 
            board = chess.Board(position['fen'])

            # Generar la posición en formato HalfKP
            input_current, input_opponent = get_input(board)

            tensor_current = torch.tensor(
                input_current.tolist(),
                dtype=torch.float32
            )

            tensor_opponent = torch.tensor(
                input_opponent.tolist(),
                dtype=torch.float32
            )

            eval = get_best_evaluation(position) 

            inputs_current.append(tensor_current)
            inputs_opponent.append(tensor_opponent)
            labels.append(eval)

    inputs_current = torch.stack(inputs_current)
    inputs_opponent = torch.stack(inputs_opponent)
    labels = torch.tensor(labels, dtype=torch.float32)

    torch.save({
        'inputs_current': inputs_current,
        'inputs_opponent': inputs_opponent,
        'labels': labels
    }, output_file)

    print(f"Datos guardados en {output_file}")


class ChessDatasetPT(Dataset):

    def __init__(self, data_file):
        # Cargar datos preprocesados
        data = torch.load(data_file)
        self.inputs_current = data['inputs_current']
        self.inputs_opponent = data['inputs_opponent']
        self.labels = data['labels']


    def __len__(self):
       return len(self.labels)


    def __getitem__(self, index):
        inputs = {
            'current': self.inputs_current[index],
            'opponent': self.inputs_opponent[index]
        }
        label = self.labels[index]

        return inputs, label
