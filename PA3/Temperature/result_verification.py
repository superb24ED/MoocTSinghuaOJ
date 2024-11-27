import json,os

from twisted.mail.scripts.mailmail import failed

right_answer_filename = "./temperature.json"
test_filename = "./temperature.out"

failed_statistic=[]
def test_results():
        # 获取脚本所在目录
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # 切换工作目录到脚本所在目录
    os.chdir(script_dir)
    
    test = []
    with open(test_filename, "r") as f:
        for line in f.readlines():
            line = line.split()
            try:
                if len(line):
                    test.append(int(line[0]))
            except Exception as e:
                print(f"待测试数据读取失败: {e}")
                break
    global json_data
    with open(right_answer_filename, "r") as f:
        try:
            json_data = json.load(f)
        except Exception as e:
            print(f"json解析失败: {e}")

    if (len(test) != len(json_data)):
        print("数据长度不一致，测试失败")
        return
    for i in range(len(test)):
        if test[i] == json_data[i]["average:"]:
            print("测例{} : 通过".format(i))
        else:
            print("测例{} : 失败".format(i))
            failed_statistic.append(
                {
                    "index": i,
                    "right answer":json_data[i]["average:"],
                    "current answer":test[i]
                }
            )
    if len(failed_statistic):
        print("失败的测例: ")
        for i in failed_statistic:
            print(i)
        with open("failed_test.json","w") as f:
            json.dump(failed_statistic, f, ensure_ascii=False)

if __name__ == "__main__":
    test_results()