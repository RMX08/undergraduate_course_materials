def vigenere_decrypt(ciphertext, key):
    alphabet = "abcdefghijklmnopqrstuvwxyz"
    plaintext = ""
    key = key.lower()
    key_index = 0

    for char in ciphertext:
        if char.lower() in alphabet:
            c_idx = alphabet.find(char.lower())
            k_idx = alphabet.find(key[key_index % len(key)])

            # 解密公式: P = (C - K) mod 26
            p_idx = (c_idx - k_idx) % 26

            res_char = alphabet[p_idx]
            plaintext += res_char if char.islower() else res_char.upper()
            key_index += 1
        else:
            plaintext += char
    return plaintext


ciphertext = """
Hmnn qf vcvav rdag jcijk. Qc qf jcckqqwo n zclarbumfa, dlkaqwo bv. Mun tiamabq lezm xv gppoqkp 
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
troubcd smbq bum pazmiwb pwjonw wo pbxc.
"""


key2 = "weijiniya"

print(f"最终确认密钥: {key2}")
print("-" * 30)
print(vigenere_decrypt(ciphertext, key2))