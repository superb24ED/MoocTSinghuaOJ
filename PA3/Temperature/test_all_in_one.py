import os
import  generate_test_data
import  result_verification
import test

if __name__ =="__main__":
        # 获取脚本所在目录
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # 切换工作目录到脚本所在目录
    os.chdir(script_dir)
    #generate_test_data.generate_test_data()
    test.test()
    result_verification.test_results()
