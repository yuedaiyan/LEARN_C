import numpy as np

# 1. 自然指数映射 e^x
x = np.array([0, 1, 2, 3])
y_e = np.exp(x)

# 2. 以 2 为底的指数映射 2^x
y_2 = np.power(2, x)

print(f"e^x: {y_e}")
print(f"2^x: {y_2}")
