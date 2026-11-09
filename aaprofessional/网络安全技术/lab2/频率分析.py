import re
from collections import Counter
import string

# 英文字母频率（标准）
ENGLISH_FREQ = {
    'A': 0.08167, 'B': 0.01492, 'C': 0.02782, 'D': 0.04253,
    'E': 0.12702, 'F': 0.02228, 'G': 0.02015, 'H': 0.06094,
    'I': 0.06966, 'J': 0.00153, 'K': 0.00772, 'L': 0.04025,
    'M': 0.02406, 'N': 0.06749, 'O': 0.07507, 'P': 0.01929,
    'Q': 0.00095, 'R': 0.05987, 'S': 0.06327, 'T': 0.09056,
    'U': 0.02758, 'V': 0.00978, 'W': 0.02360, 'X': 0.00150,
    'Y': 0.01974, 'Z': 0.00074
}


# 预处理
def clean_text(text):
    return re.sub('[^A-Z]', '', text.upper())


# 分组
def split_by_key_length(text, key_len):
    groups = ['' for _ in range(key_len)]
    for i, c in enumerate(text):
        groups[i % key_len] += c
    return groups


# 凯撒解密（单组）
def caesar_decrypt(text, shift):
    result = ""
    for c in text:
        p = (ord(c) - ord('A') - shift) % 26
        result += chr(p + ord('A'))
    return result


# 计算卡方值
def chi_squared(text):
    N = len(text)
    freq = Counter(text)

    chi2 = 0
    for letter in string.ascii_uppercase:
        observed = freq.get(letter, 0)
        expected = ENGLISH_FREQ[letter] * N
        if expected > 0:
            chi2 += (observed - expected) ** 2 / expected
    return chi2


# 找最佳shift（关键函数）
def find_best_shift(group):
    min_chi = float('inf')
    best_shift = 0

    for shift in range(26):
        decrypted = caesar_decrypt(group, shift)
        chi = chi_squared(decrypted)

        if chi < min_chi:
            min_chi = chi
            best_shift = shift

    return best_shift


# 还原密钥
def get_key(ciphertext, key_len):
    ciphertext = clean_text(ciphertext)
    groups = split_by_key_length(ciphertext, key_len)

    key = ""
    for g in groups:
        shift = find_best_shift(g)
        key += chr(shift + ord('A'))

    return key




# ================== 主程序 ==================
cipher = """ Zs vxb fxcnz evm enare usca tvdcs wgkduhtytero xjwmato xpjb jqjl krth bhzl tk hcbb nvb aolmb. Xhase 
jsb bw zcah plm vigmpiwp ib bum gdaet, owe qbewpa jtbvc ijzmbb yqde smbq urilijk iwl nzc ob 
ivmceqlg sszcp. Nlb lkzm cw n pmuoi iwl lws hwzm j pbuc. Azh zroubcoqwvnaf bm a ymbh ial woq 
liem n kmmiyvrbl. Ibd pvccp gw y pepm xn emb bnmkt ial woq liem n aahkst. Jlq zclekqxv gw rha 
lcktrar ob ilrnvkcs wrl hwh pyva e ajvpbsanc. Iml wcqtegm cw gpc fwv nucao poqrl xn uckaj ivmmndmr 
wrl hwh pyva gqeepqiigqmn. Lyb cpru ylh xwpmgpcr, abiub gpcm wfwem gpcin tznarvr iitmanrkrikra, jlq 
bm tdiu cpr dgsesv xn uckajoqwl embeaqmm, nbzcvav namr wd nail jvq arrejm jvq gmu dedn i scruni 
troubcd smbq bum pazmiwb pwjonw wo pbxc.
 """

key_len = 9  # 用你已经确定的长度

key = get_key(cipher, key_len)
print("Key =", key)
