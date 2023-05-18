import numpy as np
import math


# soc = 101.1475 + (1.010924 - 101.1475)/(1 + (voltage/31.86812)**81.69155)**5299956
  
soc = 0.4
k0 = 3.4002
k1 = 0.0080
k2 = 0.0785
k3 = -0.2150
k4 = -1.3032
k5 = 0.0891
a1 = -14
a2 = -18
a3 = 28
a4 = 40
b1 = 0
b2 = 0

for soc in range(0,11):
    soc = soc/10
    f1 = (k1 * (1/(1+math.exp(a1*(soc-b1)))))
    f2 = (k2 * (1/(1+math.exp(a2*(soc-b2)))))
    f3 = (k3 * (1/(1+math.exp(a3*(soc-1)))))
    f4 = (k4 * (1/(1+math.exp(a4*soc)))) 

    # print(k0, f1,f2,f3,f4, k5*soc)

    ocv = k0 +f1 +f2 +f3 +f4 + k5*soc
    print(soc, ocv)

