import numpy as np
import matplotlib.pyplot as plt

class Sudoku:
    def __init__(self, s):
        self.x = np.zeros([9, 9], dtype=int)
        self.default = np.zeros([9, 9], dtype=bool)
        for i in range(9):
            for j in range(9):
                tmp = int(s.split()[i][j])
                self.x[i, j] = tmp
                self.default[i, j] = bool(tmp)
        self.draft = np.zeros([9, 9, 9], dtype=bool)
        
    def updateDraft(self):
        self.draft = np.ones([9, 9, 9], dtype=bool)
        for i in range(9):
            for j in range(9):
                if self.x[i, j]:
                    self.draft[i, :, self.x[i, j]-1] = False
                    self.draft[:, j, self.x[i, j]-1] = False
                    I, J = i // 3, j // 3
                    for k in range(3*I, 3*(I+1)):
                        for l in range(3*J, 3*(J+1)):
                            self.draft[k, l, self.x[i, j]-1] = False
        
    def plotTable(self):
        fig = plt.figure(figsize=(8, 8))
        plt.xticks([])
        plt.yticks([])
        plt.xlim([-0.5, 8.5])
        plt.ylim([8.5, -0.5])
        for i in range(1, 9):
            lw = .7 if i % 3 else 2.5
            plt.plot([-0.5, 8.5], [i-0.5, i-0.5], "k-", lw=lw)  # horizontal
            plt.plot([i-0.5, i-0.5], [-0.5, 8.5], "k-", lw=lw)  # vertical
        for i in range(9):
            for j in range(9):
                if self.x[i, j] != 0:
                    c = "black" if self.default[i, j] else "blue"
                    plt.text(j, i, str(self.x[i, j]), 
                             horizontalalignment="center", verticalalignment="center", fontsize=24, color=c)
                else:
                    for k in range(9):
                        if self.draft[i, j, k]:
                            plt.text(j+((k%3)-1)/4, i+((k//3)-1)/4, 
                                     str(k+1), 
                                     horizontalalignment="center", verticalalignment="center", fontsize=10, color="gray")

    def checkValidity(self, i, j, v):
        f = np.any(self.x[i, :] == v)
        f &= np.any(self.x[:, j] == v)
        I, J = i // 3, j // 3
        for k in range(3*I, 3*(I+1)):
            for l in range(3*J, 3*(J+1)):
                f &= (self.x[k, l] == v)
        return not f
    
    def fill(self, i, j, v):
        if self.checkValidity(i, j, v):
            self.x[i, j] = v
            self.updateDraft()
            print("Filled {} to [{},{}]".format(v, i, j))
        else:
            print("{} cannot be filled in [{}, {}]".format(v, i, j))
    
    def __call__(self):
        self.plotTable()
        