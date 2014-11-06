s = raw_input('Insertrgb: ')

v=s.split(',')

a,b,c,d,e,f,g,h,i=[int(i) for i in v.[:].split('-')]

print "#"+hex(a)+hex(b)+hex(c)
print "#"+hex(d)+hex(e)+hex(f)
print "#"+hex(g)+hex(h)+hex(i)
