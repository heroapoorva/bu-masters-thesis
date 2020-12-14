import numpy as np

d = np.genfromtxt('test.out', delimiter='')
#d=np.random.rand(120,2)

seen=0
correct=0
print(d)
for i in range(len(d)):
    if(seen==0):
        m1=i
        m2=i
    if(d[i,0]<d[m1,0]):
        m1=i
    if(d[i,1]<d[m2,1]):
        m2=i
    if(seen==23):
        seen=0
        if(m1==m2):
            correct+=1
    else:
        seen+=1
print(len(d)/24,correct)
