import matplotlib.pyplot as plt

def plot_complexity_multiple(AV_values, data, labels):
    plt.figure(figsize=(10, 6))

    for label in labels:
        plt.plot(AV_values, data[label], marker='o', label=label)

    
    plt.xlabel('A*V (Número de Arestas * Número de Vértices)')
    plt.ylabel('xxxxxxx')
    plt.legend()
    plt.grid(True)
    plt.show()


AV_values = [63, 77, 84, 195, 375]  


data = {
    'GraphTopoSortComputeV1': [51, 53, 54, 132, 175],
    'GraphTopoSortComputeV2': [58, 60, 61, 145, 190],
    'GraphTopoSortComputeV3': [30, 32, 33, 54, 70]
}

labels = ['GraphTopoSortComputeV1', 'GraphTopoSortComputeV2', 'GraphTopoSortComputeV3']

plot_complexity_multiple(AV_values, data, labels)

