import numpy as np

for voltage in np.arange(19,27.5,0.1):
    soc = 101.1475 + (1.010924 - 101.1475)/(1 + (voltage/31.86812)**81.69155)**5299956
    print(round(voltage,2), round(soc,2))
