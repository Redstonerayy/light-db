data = {}

with open("benchmarks.txt", "r") as file:
    lines = [line for line in file.readlines() if line.strip() != ""]

    for line in lines:
        key = line.split(" ")[0]
        if key in data:
            data[key].append(line.split(" ")[2])
        else:
            data[key] = [line.split(" ")[2]]
    
with open("data.txt", "w") as file:
    for key in data:
        file.write(" ".join(data[key]) + "\n")

with open("data.dat", "w") as file:
    file.write("NR $SA_F$ $SA_H$ $G_F$ $G_H$\n")
    first_key = list(data.keys())[0]
    for i in range(len(data[first_key])):
        file.write(f"{i + 1} ")
        for key in data:
            file.write(data[key][i] + " ")
        file.write("\n");
