import math

def binary_tree(element_count : int) -> int:
    return math.log(element_count, 2)

def b_tree(element_count : int, base : int) -> int:
    return math.log(element_count, base)

print(binary_tree(1000))
print(b_tree(1000, 3))
