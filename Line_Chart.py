import matplotlib.pyplot as plt
import numpy as np

x = [1,2,4,6,8]
y = []
for i in x:
    path = "output/" + str(i) + ".txt"
    with open(path, 'r') as f:
        y.append(float(f.read()))

fig = plt.figure(figsize=(6,6),dpi=200)
plt.title("Parallel SW execution time at different working cluster nodes")
plt.xlabel("Number of working cluster nodes")
plt.ylabel("Execution time (sec.)")
plt.plot(x,y,c='red')
plt.scatter(x,y,c='red')
plt.grid(True,linestyle='-',alpha=0.5)
plt.show()
fig.savefig("image/Line_Chart")