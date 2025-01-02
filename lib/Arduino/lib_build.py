import os
from os.path import join, realpath
Import('env')

src_filter = [
    "+<port/*>",
    "+<RX8025T/*>",
    "+<wrapper/*>",
]

env.Replace(SRC_FILTER=src_filter)
env.Append(CPPPATH=[realpath("port")])
env.Append(CPPPATH=[realpath("RX8025T")])
env.Append(CPPPATH=[realpath("wrapper")])
# env.Append(CCFLAGS=["-xc++", "-std=c++11"])
# env.Append(CPPPATH=[realpath("easylogger/plugins/flash")])

# # 強制 RX8025T 庫內的檔案使用 C++ 編譯
# env.Append(
#     CXXFLAGS=["-std=c++11"],  # 使用 C++11 標準 (或根據需求設置 c++17)
#     CXXSRC_FILTER=["+<RX8025T/*>"],  # 針對 RX8025T 資料夾中的檔案
#     CCFLAGS=[]  # 防止被當作 C 編譯
# )




