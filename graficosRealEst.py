import matplotlib.pyplot as plt

def plot_complexity_multiple(AV_values, data, labels):

    plt.figure(figsize=(10, 6))

    for label in labels:
        plt.plot(AV_values, data[label], marker='o', label=label)

    plt.title('Comparação entre valor estimado de iterações e valor real Vx')
    plt.xlabel('A*V (Número de Arestas * Número de Vértices)')
    plt.ylabel('Número de Iterações')
    plt.legend()
    plt.grid(True)
    plt.show()


AV_values = [63, 77, 84, 195, 375] 

data = {
    'Real': [51, 53, 54, 132, 175],
    'Estimado': [49, 49, 49, 169, 225],
    
}

labels = ['Real', 'Estimado']

plot_complexity_multiple(AV_values, data, labels)

