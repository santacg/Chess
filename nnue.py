import torch
import torch.nn as nn
import torch.optim as optim
import data
from torch.utils.data import DataLoader, random_split

INPUT_SIZE = 64 * 64 * 10


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
        out_player = torch.clamp(out_player, max=1.0)
        
        # Procesar entrada del oponente
        out_opponent = self.activation(self.fc1_opponent(input_opponent))
        out_opponent = torch.clamp(out_opponent, max=1.0)

        # Combinar las salidas
        combined = torch.cat((out_player, out_opponent), dim=1)
        
        # Segunda capa
        out_combined = self.activation(self.fc2(combined))
        out_combined = torch.clamp(out_combined, max=1.0)

        # Tercera capa
        out_hidden = self.activation(self.fc3(out_combined))
        out_hidden = torch.clamp(out_hidden, max=1.0)

        # Capa de salida
        output = self.output(out_hidden)
        return output
    

def main():

    device = "cuda" if torch.cuda.is_available() else "cpu"
    print(f"Using {device} device")

    data = torch.load('data.pt')
    print("Archivo cargado correctamente en local.")
    return 
    #data.process_to_pt("sample_same_row.jsonl", "data_same.pt")

    dataset = data.ChessDataset("data.pt")
    dataset_size = len(dataset)
    print(f"Dataset Size = {dataset_size}")

    train_size = int(0.8 * dataset_size)
    test_size = dataset_size - train_size 

    train_dataset, test_datset = random_split(dataset, [train_size, test_size])

    # Definimos los hiperparametros del entrenamiento
    learning_rate = 1e-2
    batch_size = 64
    epochs = 5

    train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
    test_loader = DataLoader(test_datset, batch_size=batch_size)

    model = NNUE()
    model = model.to(device)

    print("Parámetros del modelo:")
    for name, param in model.named_parameters():
        print(f"Layer: {name} | Size: {param.size()} | Values : {param[:2]} \n")


    def train_loop(model, dataTrain, loss_fn, optimizer):
        size = len(dataTrain)
        model.train()

        for batch, (input, y) in enumerate(dataTrain):
            input_p, input_o = input['current'], input['opponent']
            input_p, input_o, y = input_p.to(device), input_o.to(device), y.to(device)

            pred = model(input_p, input_o)
            loss = loss_fn(pred, y.unsqueeze(1))

            loss.backward()
            optimizer.step()
            optimizer.zero_grad()

            if batch % 100 == 0:
                loss = loss.item()
                print(f"Loss at batch {batch}/{size}: {loss:.4f}")


    def test_loop(model, dataTest, loss_fn):
        model.eval()

        num_batches = len(dataTest)
        test_loss = 0

        with torch.no_grad():
            for (input, y) in (dataTest):
                input_p, input_o = input['current'], input['opponent']
                input_p, input_o, y = input_p.to(device), input_o.to(device), y.to(device)

                pred = model(input_p, input_o)

                batch_loss = loss_fn(pred, y.unsqueeze(1)).item()
                test_loss += batch_loss

        test_loss /= num_batches
        print(f"Test Error: \n Avg loss: {test_loss:>8f} \n")


    # Definimos la función de pérdida
    loss_fn = nn.MSELoss()

    # Definimos el algoritmo de optimización
    optimizer = optim.SGD(model.parameters(), lr=learning_rate)

    for t in range(epochs):
        print(f"Epoch {t+1}\n-------------------------------")
        train_loop(model, train_loader, loss_fn, optimizer)
        test_loop(model, test_loader, loss_fn)

    print("Done!")

    torch.save(model.state_dict(), "NNUE.pth")

if __name__ == "__main__":
    main()
