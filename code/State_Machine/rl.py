import os
from os import listdir
from os.path import isfile, join
import numpy as np
import random
random.seed(100)
np.random.seed(100)
import tensorflow as tf
import sys
from sklearn.linear_model import LinearRegression

def format_file(file_name):
    f=open(file_name)
    print(file_name)
    ar=f.readlines()
    ar=[i[:-1] for i in ar]
    ar=[i.split() for i in ar]
    ar=[[float(i) for i in j] for j in ar]
    #print("Converted to float")
    entropy=[]
    data_size=[]
    order=[]
    op=[[]]
    for i in range(24):
        order.append(0)
    cost=[]
    
    for i in range(len(ar)):
        temp=i%50
        g=[]
        if(temp==0):
            entropy=ar[i]
        else:
            if(temp==1):
                data_size=ar[i]
            else:
                if(temp%2==0):
                    order[(temp/2)-1]=1
                else:
                    cost=ar[i]
                    if(len(op[-1])==24):
                        op.append([])
                    op[-1].append([])
                    op[-1][-1].extend(entropy)
                    op[-1][-1].extend(data_size)
                    op[-1][-1].extend(order)
                    op[-1][-1].extend(cost)
                    order[(temp-3)/2]=0
    #print("ready to return", len(op))
    #Error checking
    '''
    not_equal=0
    for i in range(len(op)):
        if(len(op[i])!=36):
            not_equal=1
            print(i)
            print(op[i])
            break
    '''
    f.close()
    return op

def reinforcement_learning(fin_train,save_loc,epoch_number):
    
    data_points = len(fin_train)/24
    bp = int(data_points*0.7)

    bp = bp*24

    x_tr=fin_train[:bp,:-2]
    #time
    y1_tr=fin_train[:bp,-1]
    #operations
    y2_tr=fin_train[:bp,-2]
    
    x_te=fin_train[bp:,:-2]
    y1_te=fin_train[bp:,-1]
    y2_te=fin_train[bp:,-2]
    
    cp=save_loc
    cd = os.path.dirname(cp)

    cp_callback = tf.keras.callbacks.ModelCheckpoint(filepath=cp,
                                                     save_weights_only=True,
                                                     verbose=1)
    
    model = tf.keras.Sequential()
    model.add(tf.keras.layers.Dense(34,activation='relu'))
    model.add(tf.keras.layers.Dense(68,activation='relu'))
    model.add(tf.keras.layers.Dense(136,activation='relu'))
    model.add(tf.keras.layers.Dense(68,activation='relu'))
    model.add(tf.keras.layers.Dense(34,activation='relu'))
    model.add(tf.keras.layers.Dense(1))
    model.compile(optimizer='adam', loss='mse')
    
    
    '''
    model.fit(x_tr, y2_tr, batch_size=32, epochs=2, callbacks=[cp_callback])
    ans=model.predict(x_te)
    t=np.stack((ans[:,0],y2_te[:]),axis=1)
    print(t)
    np.savetxt('test.out', t)
    '''
    times = int(epoch_number)
    for i in range(times):
        history=model.fit(x_tr, y1_tr, batch_size=100, epochs=1, callbacks=[cp_callback])
        print(history.history)
        ans=model.predict(x_te)
        seen=0
        correct=0
        for j in range(len(ans)):
            if(seen==0):
                m1=j
                m2=j
            if(ans[j]<ans[m1]):
                m1=j
            if(y1_te[j]<y1_te[m2]):
                m2=j
            if(seen==23):
                seen=0
                if(m1==m2):
                    correct+=1
            else:
                seen+=1
        print(len(ans)/24,correct)
    #'''    

def main(argv):
    mypath=argv[0]
    #mypath="/home/ubuntu/bu-masters-thesis/code/State_Machine/Outputs"
    onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]
    fin_train=[]
    read_times=0
    max_read=int(argv[1])
#    print(len(onlyfiles))
    onlyfiles.sort()
    for i in range(len(onlyfiles)):
        cur_file = mypath+"/"+onlyfiles[i]
        op = format_file(cur_file)
        fin_train.extend(op)
#        print(len(fin_train))
        read_times+=1
#        print(read_times,max_read)
        if(read_times>=max_read):
            break
    op=[]
    random.shuffle(fin_train)
    print(len(fin_train),len(fin_train[0]),len(fin_train[0][0]))
    
    dataset=[]
    for i in fin_train:
        dataset.extend(i)
    dataset = np.asarray(dataset)
    fin_train=[]
    reinforcement_learning(dataset, argv[2], argv[3])
    
if __name__ == '__main__':
    '''
    input, is location to data files, and .
    '''
    if(len(sys.argv)!=5):
        print("Usage is")
        print('python rl.py "Path to data files" "number of files to read" "model saving location" "Number of epoches for neural network"')
        sys.exit()
    main(sys.argv[1:])
    
# Visualize data and of results
# Can we use simpler model? Any rule based?
# Evaluations need visualizations
#
