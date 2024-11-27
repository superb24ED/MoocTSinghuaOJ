import json,os

#global random_n, random_m, datas, queries, results
random_n:int = 0
random_m:int = 0
datas:list = []
queries:list = []
results:list = []
json_data=[]
def test():
    # 获取脚本所在目录
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # 切换工作目录到脚本所在目录
    os.chdir(script_dir)
    with open("./temperature.in", "r") as f:
        random_n, random_m = map(int, f.readline().split())
        for i in range(random_n):
            x, y, temp = map(int, f.readline().split())
            datas.append(
                {
                    "x": x,
                    "y": y,
                    "temperature": temp
                }
            )
        for i in range(random_m):
            x1, y1, x2, y2 = map(int, f.readline().split())
            queries.append(
                (x1, y1, x2, y2)
            )
    for query in queries:
        sum, count = 0, 0
        for data in datas:
            if query[0] <= data["x"] <= query[2] and query[1] <= data["y"] <= query[3]:
                count += 1
                sum += data["temperature"]
        results.append(
            [sum, count, int(sum / count) if count else 0]
        )
    with open("./temperature.out.python", "w") as f:
        for result in results:
            f.writelines("average: {}  sum: {}  count: {}\n".format(result[2], result[0], result[1]))
            
    
    for i in range(len(results)):
        json_data.append(
        {
            "index":i,
            "average:": results[i][2],
            "sum": results[i][0],
            "count": results[i][1]
        } )



    with open("temperature.json", "w", encoding="utf-8") as f:
        json.dump(json_data, f, ensure_ascii=False, indent=4)
    with open("temperature_test_data.json","w") as f:
         json.dump(datas, f, ensure_ascii=False, indent=4)
    with open("temperature_test_query.json","w") as f:
         json.dump([{
             "index": i,
             "query":query
             } for i,query in enumerate(queries)], f, ensure_ascii=False, indent=4)
if __name__=="__main__":
    test()

