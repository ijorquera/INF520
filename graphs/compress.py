import numpy as np
import os
from compressors import get_gap, get_rle, get_trie

# files = ["email_eu_core", "facebook", "twitch", "pokec", "gplus"]
# files = ["gplus"]
# files = ["twitch", "pokec", "gplus"]
files = ["gplus"]
# files = ["test", "trie_test"]
modes = ["z", "col", "row"]
# modes = ["z"]
in_dir = "data_g"
out_dir = "output"
ext = ".txt"

for file in files:
    print(f"{file}:")
    for mode in modes:
        print(f"    {mode}")
        os.makedirs(f"{in_dir}/{file}/{out_dir}", exist_ok=True)

        arr = np.loadtxt(f"{in_dir}/{file}/{mode}_{file}{ext}")
        # print("        gap")
        # get_gap(arr, file, mode, in_dir, out_dir, ext)
        # print("        rle")
        # get_rle(arr, file, mode, in_dir, out_dir, ext)
        print("        trie")
        get_trie(arr, file, mode, in_dir, out_dir, ext)
    print()
