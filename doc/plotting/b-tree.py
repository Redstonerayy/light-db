import math
import matplotlib.pyplot as plt

# comparison is equality and less/more check
def binary_tree(depth):
    element_count = 2 ** depth
    max_search_comparisons = depth - 1
    # print(element_count, 2, max_search_comparisons)
    return element_count, max_search_comparisons

def b_tree(depth, branching):
    element_count = branching ** depth + sum([branching ** i for i in range(depth)])
    max_node_comparisons = int(math.log(branching, 2))
    max_search_comparisons = max_node_comparisons * depth
    # print(element_count, max_node_comparisons, max_search_comparisons)
    return element_count, max_search_comparisons

def b_tree_reverse(elements, branching):
    depth = math.log(elements, branching)
    max_node_comparisons = int(math.log(branching, 2))
    # max_node_comparisons = math.log(branching, 2)
    max_search_comparisons = max_node_comparisons * depth
    # print(element_count, max_node_comparisons, max_search_comparisons)
    return elements, max_search_comparisons


b_tree_reverse(13, 3)

maxi = 110
elements = [1 * 10 ** 5, 2 * 10 ** 5, 3 * 10 ** 5, 4 * 10 ** 5, 5 * 10 ** 5]
binary_tree_plot = [binary_tree(d) for d in range(1, maxi)]
# b_tree_16_plot = [b_tree_reverse(elements[i], 16) for i in range(len(elements))]
# b_tree_32_plot = [b_tree_reverse(elements[i], 32) for i in range(len(elements))]
# b_tree_64_plot = [b_tree_reverse(elements[i], 64) for i in range(len(elements))]
# b_tree_128_plot = [b_tree_reverse(elements[i], 128) for i in range(len(elements))]
# b_tree_256_plot = [b_tree_reverse(elements[i], 256) for i in range(len(elements))]
# b_tree_512_plot = [b_tree_reverse(elements[i], 512) for i in range(len(elements))]
b_tree_16_plot = [b_tree(i, 16) for i in range(1, 20)]
b_tree_32_plot = [b_tree(i, 32) for i in range(1, 20)]
b_tree_64_plot = [b_tree(i, 64) for i in range(1, 20)]
b_tree_128_plot = [b_tree(i, 128) for i in range(1, 10)]
b_tree_256_plot = [b_tree(i, 256) for i in range(1, 10)]
b_tree_512_plot = [b_tree(i, 512) for i in range(1, 10)]

fig, ax = plt.subplots()

ax.plot([i[0] for i in binary_tree_plot], [i[1] for i in binary_tree_plot], color='blue', label='binary_tree')

ax.plot([i[0] for i in b_tree_16_plot], [i[1] for i in b_tree_16_plot], color='green', label='btree_16')
ax.plot([i[0] for i in b_tree_32_plot], [i[1] for i in b_tree_32_plot], color='violet', label='btree_32')
ax.plot([i[0] for i in b_tree_64_plot], [i[1] for i in b_tree_64_plot], color='purple', label='btree_64')
ax.plot([i[0] for i in b_tree_128_plot], [i[1] for i in b_tree_128_plot], color='red', label='btree_128')
ax.plot([i[0] for i in b_tree_256_plot], [i[1] for i in b_tree_256_plot], color='orange', label='btree_256')
ax.plot([i[0] for i in b_tree_512_plot], [i[1] for i in b_tree_512_plot], color='black', label='btree_512')

ax.set_xlabel("Elements")
ax.set_ylabel("Comparisons")

ax.legend()

plt.tight_layout()
plt.show()
