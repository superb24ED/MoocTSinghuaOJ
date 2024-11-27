import  random,os

pinned_n = 10
pinned_m = 5
pinned_data = '''0 0 1000
1 1 1300
2 2 1600
3 3 1100
4 5 3500
5 15 4322
0 8 1765
9 6 2459
1 5 3243
0 1 3424
'''
pinned_query ='''0 0 1 1
0 0 10 10
2 3 6 7
1 5 7 9
1 1 7 7
'''



random_data= []
random_query = []

#settings
random_n,random_m=10000,2000
pos_range = [-1000,1000]
temperature_range = [-5000,5000]
input_filename = "./temperature.in"
#output_filename = "./temperature.out.pytest"
#generate random test
def generate_test_data():
        # 获取脚本所在目录
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # 切换工作目录到脚本所在目录
    os.chdir(script_dir)
    
    for i in range(random_n):
        x, y = random.randint(pos_range[0], pos_range[1]), random.randint(pos_range[0], pos_range[1])
        temp = random.randint(temperature_range[0], temperature_range[1])
        random_data.append(
            {
                "pos": (x, y),
                "temperature": temp
            }
        )
    for i in range(random_m):
        x1, y1 = random.randint(pos_range[0], pos_range[1]), random.randint(pos_range[0], pos_range[1])
        x2, y2 = random.randint(pos_range[0], pos_range[1]), random.randint(pos_range[0], pos_range[1])
        if x1 > x2:
            x1, x2 = x2, x1
        if y1 > y2:
            y1, y2 = y2, y1
        random_query.append(
            [x1, y1, x2, y2]
        )
    with open(input_filename, "w") as f:
        f.writelines("{} {}\n".format(pinned_n + random_n, pinned_m + random_m))
        f.writelines(pinned_data)

        for data in random_data:
            f.writelines("{} {} {}\n".format(data["pos"][0], data["pos"][1], data["temperature"]))

        f.writelines(pinned_query)

        for query in random_query:
            f.writelines("{} {} {} {}\n".format(query[0], query[1], query[2], query[3]))

if __name__ == "__main__":
    generate_test_data()
    
    
    
    
'''
default:

10 5
1 1 1300
2 2 1600
3 3 1100
4 5 3500
5 15 4322
0 8 1765
9 6 2459
1 5 3243
0 1 3424
0 0 1 1
0 0 10 10
2 3 6 7
1 5 7 9
1 1 7 7

'''