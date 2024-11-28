import torch
import torch.nn as nn
import torch.optim as optim
import data

INPUT_SIZE = 64 * 64 * 10

device = (
    "cuda"
    if torch.cuda.is_available()
    else "mps"
    if torch.backends.mps.is_available()
    else "cpu"
)

print(f"Using {device} device")
class NNUE(nn.Module):
    def __init__(self):
        super(NNUE, self).__init__()

        # Primera capa para cada mitad (40,960 -> 256)
        self.fc1_player = nn.Linear(INPUT_SIZE, 256)
        self.fc1_opponent = nn.Linear(INPUT_SIZE, 256)
        
        # Segunda capa (256+256 -> 32)
        self.fc2 = nn.Linear(256 + 256, 32)
        
        # Tercera capa (32 -> 32)
        self.fc3 = nn.Linear(32, 32)
        
        # Capa de salida (32 -> 1)
        self.output = nn.Linear(32, 1)
        
        # Activación ReLU 1
        self.activation = nn.ReLU()

    def forward(self, input_player, input_opponent):
        # Procesar entrada del jugador actual
        out_player = self.activation(self.fc1_player(input_player))
        out_player = torch.clamp(out_player, min=0.0, max=1.0)
        
        # Procesar entrada del oponente
        out_opponent = self.activation(self.fc1_opponent(input_opponent))
        out_opponent = torch.clamp(out_opponent, min=0.0, max=1.0)

        # Combinar las salidas
        combined = torch.cat((out_player, out_opponent))
        
        # Segunda capa
        out_combined = self.activation(self.fc2(combined))
        out_combined = torch.clamp(out_combined, min=0.0, max=1.0)

        # Tercera capa
        out_hidden = self.activation(self.fc3(out_combined))
        out_hidden = torch.clamp(out_hidden, min=0.0, max=1.0)

        
        # Capa de salida
        output = self.activation(self.output(out_hidden))
        output = torch.clamp(output, min=0.0, max=1.0)
        return output
    
    def train_loop(self, dataTrain, loss_fn, optimizer):
        size = len(dataTrain)
        print(size)
        self.train()

        i = 0
        for (input_p, input_o, y) in dataTrain:
            # Calculamos el resultado del modelo y la pérdida
            pred = self.forward(input_p, input_o)
            y = y.unsqueeze(0)
            
            loss = loss_fn(pred, y)

            # Retropropagación
            loss.backward()
            optimizer.step()
            optimizer.zero_grad()
            print("Iteracion train: ", i)
            i += 1


    def test_loop(self, dataTest, loss_fn):
        size = len(dataTest)  # Tamaño total de los datos de prueba
        self.eval()  # Establecer el modelo en modo de evaluación (sin dropout, etc.)

        num_batches = len(dataTest)  # Número total de lotes de prueba
        test_loss = 0.0  # Inicializar la pérdida total

        total_error = 0.0  # Inicializar el error total (para calcular MAE o MSE)

        with torch.no_grad():  # Desactivar gradientes durante la evaluación
            for (input_p, input_o, y) in (dataTest):
                # Pasar las entradas por el modelo
                pred = self.forward(input_p, input_o)
                y = y.unsqueeze(0)
                print(y)
                print(pred)
                # Sumar la pérdida en cada lote
                loss = loss_fn(pred, y)
                test_loss += loss.item()

                # Si prefieres el error cuadrático medio (MSE):
                total_error += ((pred - y) ** 2).sum().item()  # Para MSE

        # Promediar la pérdida total
        test_loss /= num_batches

        # Promediar el error
        avg_error = (total_error / size) ** 0.5  # Para MSE, toma la raíz cuadrada

        print(f"Test Error: \n Avg loss: {test_loss:>8f} \n Avg error (MAE): {avg_error:>8f}")


db = data.ChessDatabase("chess_nn.db", "/usr/bin/stockfish")
# db.crear_db()
# db.load_games_to_db("./Games/lichess_elite_2024-08.pgn")
db.close_engine()

train_dataset = data.ChessDataset(db_name='chess_nn.db', is_test=False)
train_loader = train_dataset.load_data(batch_size=32)

test_dataset = data.ChessDataset(db_name='chess_nn.db', is_test=True)
test_loader = test_dataset.load_data(batch_size=32)

model = NNUE()
print(model)

# Definimos los hiperparametros del entrenamiento
learning_rate = 1e-1
batch_size = 32
epochs = 4

# Definimos la función de pérdida
loss_fn = nn.MSELoss()

# Definimos el algoritmo de optimización
optimizer = optim.SGD(model.parameters(), lr=learning_rate)

for t in range(epochs):
     print(f"Epoch {t+1}\n-------------------------------")
     model.train_loop(train_dataset, loss_fn, optimizer)
     model.test_loop(test_dataset, loss_fn)
print("Done!")
def summarize_parameters(model):
    for name, param in model.named_parameters():
        if param.requires_grad:
            print(f"{name}: Mean = {param.data.mean():.4f}, Std = {param.data.std():.4f}")

# Después de entrenar el modelo
summarize_parameters(model)
train_dataset.close()
test_dataset.close()
