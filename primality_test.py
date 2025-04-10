import random
import math

N = 12357
runs = math.ceil(math.log2(N))
isPrime = True
for _ in range(runs):
    base = random.randint(2, N-1)
    exponent = N-1
    rem = pow(base, exponent, N)
    if rem != 1:
        print(f"{N} is not prime")
        isPrime = False
    else:
        while exponent % 2 == 0:
            exponent /= 2
            rem = pow(base, exponent, N)
            if rem != 1:
                if rem != -1:
                    print(f"{N} is not prime")
                    isPrime = False
                break
    if not isPrime:
        break

if isPrime:
    print(f"{N} is prime")
