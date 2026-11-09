import re

def clean_text(text):
    return re.sub('[^A-Z]', '', text.upper())

# 你题目里的真实密文
ciphertext = """Hmnn qf vcvav rdag jcijk. Qc qf jcckqqwo n zclarbumfa, dlkaqwo bv. Mun tiamabq lezm xv gppoqkp 
da, nvb wa aqut yqte kr bqzbceh kyz lpvtbrar. Bqm vvqtexccqbvq wa fcrtq mldqvm, jvq ec wept nvqcpe 
plzxctp rhaq. Bqm omyupc en nnafikr kjvawr ba hqvurl zy ziicp. Bcp fhiaq ung nenmaq, whz fajha fqyt 
uiplma, jhb rhwx eqqpp rhac kamnb gn xiidbl ild cswmvraq ajh bacgp jiria xv swp ahp brur bm ckqm. 
Zs vxb fxcnz evm enare usca tvdcs wgkduhtytero xjwmato xpjb jqjl krth bhzl tk hcbb nvb aolmb. Xhase 
jsb bw zcah plm vigmpiwp ib bum gdaet, owe qbewpa jtbvc ijzmbb yqde smbq urilijk iwl nzc ob 
ivmceqlg sszcp. Nlb lkzm cw n pmuoi iwl lws hwzm j pbuc. Azh zroubcoqwvnaf bm a ymbh ial woq 
liem n kmmiyvrbl. Ibd pvccp gw y pepm xn emb bnmkt ial woq liem n aahkst. Jlq zclekqxv gw rha 
lcktrar ob ilrnvkcs wrl hwh pyva e ajvpbsanc. Iml wcqtegm cw gpc fwv nucao poqrl xn uckaj ivmmndmr 
wrl hwh pyva gqeepqiigqmn. Lyb cpru ylh xwpmgpcr, abiub gpcm wfwem gpcin tznarvr iitmanrkrikra, jlq 
bm tdiu cpr dgsesv xn uckajoqwl embeaqmm, nbzcvav namr wd nail jvq arrejm jvq gmu dedn i scruni 
troubcd smbq bum pazmiwb pwjonw wo pbxc."""

# 预处理
def clean_text(text):
    return re.sub('[^A-Z]', '', text.upper())


# IC计算
from collections import Counter

def calculate_ic(text):
    N = len(text)
    freq = Counter(text)

    ic = 0
    for f in freq.values():
        ic += f * (f - 1)

    return ic / (N * (N - 1)) if N > 1 else 0

# 按密钥长度分组
def split_by_key_length(text, key_len):
    groups = ['' for _ in range(key_len)]

    for i, c in enumerate(text):
        groups[i % key_len] += c

    return groups

# 计算不同密钥长度的IC
def find_key_length(ciphertext, max_len=20):
    ciphertext = clean_text(ciphertext)

    result = []

    for key_len in range(1, max_len + 1):
        groups = split_by_key_length(ciphertext, key_len)

        ic_values = [calculate_ic(g) for g in groups]
        avg_ic = sum(ic_values) / len(ic_values)

        result.append((key_len, avg_ic))

    return result

cipher = """ Lyb cpru ylh xwpmgpcr, abiub gpcm wfwem gpcin tznarvr iitmanrkrikra, jlq 
bm tdiu cpr dgsesv xn uckajoqwl embeaqmm, nbzcvav namr wd nail jvq arrejm jvq gmu dedn i scruni 
troubcd smbq bum pazmiwb pwjonw wo pbxc.
"""

results = find_key_length(cipher, 20)

for k, ic in results:
    print(f"key_len={k}, IC={ic:.4f}")