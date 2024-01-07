import random, string, csv

record_count = 100000
letters = string.ascii_lowercase

def make_row(i) -> list:
    _id = i
    _name = ''.join(random.choice(letters) for i in range(16))
    
    _friends_list = [str(random.randint(0, record_count - 1)) for i in range(100)]
    while i in _friends_list:
        _friends_list.remove(i)
    _friends = '-'.join(_friends_list)
    
    _enemies_list = [str(random.randint(0, record_count - 1)) for i in range(100)]
    while i in _enemies_list:
        _enemies_list.remove(i)
    _enemies = '-'.join(_enemies_list)

    return [_id, _name, _friends, _enemies]

with open("records.csv", "w") as csvfile:
    csvwriter = csv.writer(csvfile)
    for i in range(record_count):
        csvwriter.writerow(make_row(i))
