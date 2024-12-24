import os
from os.path import join, realpath

include_dirs = []
for root, dirs, files in os.walk("source/include"):
    if any(fname.endswith('.h') for fname in files):
        include_dirs.append(os.path.realpath(root))
        
print("Include dirs:", include_dirs)  # 確認實際包含的路徑