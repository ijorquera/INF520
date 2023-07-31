import numpy as np

def get_H0_entropy(arr):
    uniques = np.sort(np.unique(arr))
    n = arr.shape[0]
    h0_entropy = 0

    for unique_value in uniques:
        n_i = np.count_nonzero(arr == unique_value)
        p_i = n_i / n
        p_i_inv = n / n_i

        h0_entropy_i = p_i * np.log2(p_i_inv)

        h0_entropy += h0_entropy_i

    return np.array([h0_entropy])
def get_H0_entropy_2(arr):
    n = arr.shape[0]
    h0_entropy = 0
    _, counts = np.unique(arr, return_counts=True)

    for n_i in counts:
        p_i = n_i / n
        p_i_inv = n / n_i

        h0_entropy_i = p_i * np.log2(p_i_inv)

        h0_entropy += h0_entropy_i

    return np.array([h0_entropy])

def get_gap(arr, file, mode, in_dir, out_dir, ext):
    gap_vector = np.array([])
    prev = 0
    gapEntropy = 0

    for value in arr:
        gap_entropy_i = 1
        gap_value_i = value - prev
        prev = value + 1

        if (gap_value_i > 0):
            gap_entropy_i = np.floor(np.log2(gap_value_i)) + 1

        gapEntropy += gap_entropy_i
        gap_vector = np.append(gap_vector, gap_value_i)

    np.savetxt(f"{in_dir}/{file}/{out_dir}/gapVector_{mode}_{file}{ext}", gap_vector, fmt='%i')
    np.savetxt(f"{in_dir}/{file}/{out_dir}/gapEntropy_{mode}_{file}{ext}", np.array([gapEntropy]), fmt='%i')
    np.savetxt(f"{in_dir}/{file}/{out_dir}/gapH0Entropy_{mode}_{file}{ext}", get_H0_entropy(gap_vector), fmt='%i')
    return

def get_rle(arr, file, mode, in_dir, out_dir, ext):
    rle_vector = np.array([])
    
    rleZerosEntropy = 0
    rleOnesEntropy = 0

    prev = 0
    count = 0

    for value in arr:
        rle_value_i = value - prev

        if (value == prev):
            # Cant de ceros
            z_i = 0
            rleZerosEntropy += z_i

            rle_vector = np.append(rle_vector, count)
            count += 1
        
        elif ((prev == 0 and value == 1 and count == 0) or (rle_value_i > 1)):
            if (count > 0):
                # Cant de unos
                l_i = 1

                if (count > 1) :
                    l_i = np.floor(np.log2(count - 1)) + 1

                rleOnesEntropy += l_i
                rle_vector = np.append(rle_vector, count)

            n_zeros = rle_value_i
            if (rle_value_i > 1):
                n_zeros = rle_value_i - 1

            z_i = 1
            if (n_zeros > 1):
                z_i = np.floor(np.log2(n_zeros - 1)) + 1

            rleZerosEntropy += z_i
            rle_vector = np.append(rle_vector, n_zeros)

            count = 1
        
        else:
            count += 1
        
        prev = value
        
    if (count > 0):
        # Cant de unos
        l_i = 1
        if (count > 1):
            l_i = np.floor(np.log2(count - 1)) + 1
        
        rleOnesEntropy += l_i
        rle_vector = np.append(rle_vector, count)

    np.savetxt(f"{in_dir}/{file}/{out_dir}/rleVector_{mode}_{file}{ext}", rle_vector, fmt='%i')
    np.savetxt(f"{in_dir}/{file}/{out_dir}/rleEntropy_{mode}_{file}{ext}", np.array([rleZerosEntropy, rleOnesEntropy, rleZerosEntropy + rleOnesEntropy]), fmt='%i')
    np.savetxt(f"{in_dir}/{file}/{out_dir}/rleH0Entropy_{mode}_{file}{ext}", get_H0_entropy(rle_vector), fmt='%i')
    return

def binaryToDecimal(binary):
    decimal, i = 0, 0
    for i, bit in enumerate(binary[::-1]):
        decimal += (int(bit) * 2) ** i
    return decimal

def decimalToBinary(decimal):
    bit_vector = ""

    while decimal > 0:
        if decimal % 2 == 0:
            bit_vector = "0" + bit_vector
        else:
            bit_vector = "1" + bit_vector
        decimal = decimal // 2

    return bit_vector

def get_trie(arr, file, mode, in_dir, out_dir, ext):
    trie_vector = np.array([])
    idx = 0
    trieEntropy = 0
    current_bit_sec = ""

    n_bits = np.floor(np.log2(arr[-1])) + 1
    
    for value in arr:
        flag = False
        trieEntropy_i = 0

        if (idx == 0):
            for _ in range(int(n_bits) - len(decimalToBinary(value))):
                current_bit_sec += "0"
            
            current_bit_sec += decimalToBinary(value)

            trieEntropy_i = len(current_bit_sec)
            trie_vector = np.append(trie_vector, value)
        
        else:
            aux = decimalToBinary(value)
            new_bit_sec = ""
            compressed_sec = ""
            dif = len(current_bit_sec) - len(aux)

            for i in range(dif):
                new_bit_sec += current_bit_sec[i]
            
            for i in range(dif, len(current_bit_sec)):
                new_bit_sec += aux[i - dif]

                if current_bit_sec[i] != aux[i - dif]:
                    flag = True

                if flag:
                    trieEntropy_i += 1
                    compressed_sec += aux[i - dif]
                    # trie_vector = np.append(trie_vector, binaryToDecimal(int(aux[i - dif])))
                    # trie_binary_list.append(aux[i - dif])
            # if flag:                  
            trie_vector = np.append(trie_vector, binaryToDecimal(compressed_sec))
            current_bit_sec = new_bit_sec
        
        idx += 1
        trieEntropy += trieEntropy_i

    np.savetxt(f"{in_dir}/{file}/{out_dir}/trie_nbits_{mode}_{file}{ext}", np.array([n_bits]), fmt='%i')
    np.savetxt(f"{in_dir}/{file}/{out_dir}/trieVector_2_{mode}_{file}{ext}", trie_vector, fmt='%i')
    np.savetxt(f"{in_dir}/{file}/{out_dir}/trieEntropy_{mode}_{file}{ext}", np.array([trieEntropy]), fmt='%i')
    np.savetxt(f"{in_dir}/{file}/{out_dir}/trieH0Entropy_{mode}_{file}{ext}", get_H0_entropy_2(trie_vector), fmt='%i')

    return
