import numpy
import sys
import csv


'''
x = numpy.load('update_order_btcusd.npy')
numpy.set_printoptions(suppress=True)
first = x[0][4]
last = x[x.shape[0]-1][4]
print(first, last)
y = x[0]
print(y)
'''



#converty init file to numpy array
with open(sys.argv[1]) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        x = numpy.array([0.00, 0.00, 0.00])
        if line_count != 0:
            if row[3] == "asks":
                x[0] = 1
            x[1] = row[1]
            x[2] = row[0]
        if line_count == 1:
            init = [x]
        elif line_count > 1 :
            init = numpy.append(init, [x], axis=0)
        line_count += 1
numpy.save("out", init) #change name of file depending on pair

#convert update file to numpy array
with open(sys.argv[2]) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        x = numpy.array([0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00])
        if line_count !=0:
            if row[9] == "bids":
                x[0] = 1
            else:
                x[0] = 2
            x[1] = row[6]#price
            x[2] = row[8]#remaining volume
            x[3] = row[5]#delta volume
            x[4] = row[2]#timestamp in seconds
            x[5] = row[3]#timestamp in milliseconds
            if row[7] == "cancel":
                x[6] = 1
            elif row[7] == "place":
                x[6] = 2
            elif row[7] == "place":
                x[6] = 3
            x[7] = row[1]#eventID
        if line_count == 1:
            update = [x]
        elif line_count > 1 :
            update = numpy.append(init, [x], axis=0)
        line_count += 1
numpy.save("out", update) #change the name of the output file depending on the pair
