import os
from os import listdir
from os.path import isfile, join
import numpy as np
#np.random.seed(100)
import tensorflow as tf

def format_file(op, file_name):
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
    for i in range(24):
        order.append(0)
    cost=[]
    for i in range(len(ar)):
        temp=i%50
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
                    op.append([])
                    op[-1].extend(entropy)
                    op[-1].extend(data_size)
                    op[-1].extend(order)
                    op[-1].extend(cost)
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

def reinforcement_learning(fin_train):
    
    data_points = len(fin_train)/24
    bp = int(data_points*0.7)

    bp = bp*24

    x_tr=fin_train[:bp,:-2]
    y1_tr=fin_train[:bp,-1]
    y2_tr=fin_train[:bp,-2]
    
    x_te=fin_train[bp:,:-2]
    y1_te=fin_train[bp:,-1]
    y2_te=fin_train[bp:,-2]
    
    cp='/home/ubuntu/bu-masters-thesis/code/State_Machine/model/'
    cd = os.path.dirname(cp)

    cp_callback = tf.keras.callbacks.ModelCheckpoint(filepath=cp,
                                                     save_weights_only=True,
                                                     verbose=1)

    model = tf.keras.Sequential()
    model.add(tf.keras.layers.Dense(34,activation='relu'))
    model.add(tf.keras.layers.Dense(34,activation='relu'))
    model.add(tf.keras.layers.Dense(34,activation='relu'))
    model.add(tf.keras.layers.Dense(34,activation='relu'))
    model.add(tf.keras.layers.Dense(1))
    model.compile(optimizer='adam', loss='mse')
    model.fit(x_tr, y1_tr, batch_size=32, epochs=1, callbacks=[cp_callback])


    ans=model.predict(x_te)
    t=np.stack((ans[:,0],y1_te[:]),axis=1)
    print(t)
    np.savetxt('test.out', t)
    
def main():
    mypath="/home/ubuntu/bu-masters-thesis/code/State_Machine/Outputs"
    onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]
    fin_train=[]
    read_times=0
    for i in range(len(onlyfiles)):
        cur_file = mypath+"/"+onlyfiles[i]
        format_file(fin_train, cur_file)
        print(len(fin_train))
        read_times+=1
#        if(read_times==3):
#            break
    
    print(len(fin_train))
    
    fin_train = np.asarray(fin_train)
    np.random.shuffle(fin_train)
    reinforcement_learning(fin_train)
    
    
    
if __name__ == '__main__':
    '''
    
    '''
    main()
