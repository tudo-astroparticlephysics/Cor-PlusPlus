
# coding: utf-8

# In[1]:

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import csv


x = []
y = []

counter = 0
with open('time_macmini_o0.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(counter )
        y.append(row[0])
        counter = counter + 1000000

plt.plot(x,y, label='i5-4278U_O0')


x = []
y = []

counter = 0
with open('time_macmini_o3.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(counter )
        y.append(row[0])
        counter = counter + 1000000

plt.plot(x,y, label='i5-4278U_O3')

###################################

x = []
y = []

counter = 0
with open('time_X5650_o0.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(counter )
        y.append(row[0])
        counter = counter + 1000000

plt.plot(x,y, label='X5650_O0')

x = []
y = []

counter = 0
with open('time_X5650_o3.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(counter )
        y.append(row[0])
        counter = counter + 1000000

plt.plot(x,y, label='X5650_O3')


################################################

x = []
y = []

counter = 0
with open('time_E5450_o0.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(counter )
        y.append(row[0])
        counter = counter + 1000000

plt.plot(x,y, label='E5450_O0')

x = []
y = []

counter = 0
with open('time_E5450_o3.csv','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        x.append(counter )
        y.append(row[0])
        counter = counter + 1000000

plt.plot(x,y, label='E5450_O3')






plt.xlabel('# of random numbers')
plt.ylabel('Runtime in seconds')
plt.title('Runtime')
plt.legend()
plt.savefig('foo.png')
plt.show()


# In[ ]:



