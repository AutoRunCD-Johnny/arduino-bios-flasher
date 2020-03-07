def flash_gen(filename):
    fr=open(filename,"rb")
    fw=open(filename+"_flash.txt","w")
    a=list(fr.read())
##    a=[i.encode('hex') for i in a] 
##    print a
    fw.write("const PROGMEM byte ROM[] = {")
    for i in a[:-1]:
        fw.write("0x"+i.encode('hex')+",")
    fw.write("0x"+a[-1].encode('hex')+"}")
    fr.close()
    fw.close()
        
flash_gen("K42JAAS.300")
